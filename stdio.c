#include "stdio.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct _IOB[100] =
{
	{0,NULL,NULL,0,1,0},
	{0,NULL,NULL,0,1,1},
	{0,NULL,NULL,0,1,2}
};

/*
 * Alloue un buffer au fichier
 */
void alloc_buf(FILE * f) {
	if( (f->_base = malloc(BUFSIZ)) ) {
		// On a un buffer
		f->_flag |= _IOFBF;
		f->_bufsiz = BUFSIZ;
	} else {
		// On n'a pas de buffer
		f->_flag |= _IONBF;
	}

	// On fait poiter le premier element sur la base
	f->_ptr = f->_base;

	// On indique que l'on est au debut du buffer
	f->_cnt = 0;
}

/*
 * Libere le buffer
 */
 void free_buf(FILE * f) {
 	if (f->_flag & _IOFBF) {
 		free(f->_base);
 		f->_flag |= _IONBF;
 		f->_bufsiz = 0;
 		f->_cnt = 0;
 	}
 }


/*
 * Remplit le buffer avec les caracteres du fichier
 * Aloue un buffer si aucun n'est aloue.
 * Retourne le premier caractere a lire en cas de succes
 * Retourne EOF en cas d'echec, si le fichier n'est pas ouvert en lecture ou si on est en fin de fichier. 
 */
int _filbuf(FILE * f) {
	// Fichier ouvert en ecriture
	if (f->_flag & _IOWRT) {
		f->_flag |= _IOERR;
		return EOF;
	}

	// Aucun buffer aloue
	if (f->_flag & _IONBF) {
		alloc_buf(f);
	} else {
		// Sinon on place le caractere courant sur la base
		f->_ptr = f->_base;
	}

	f->_cnt = read(fileno(f), f->_ptr, f->_bufsiz);

	if(f->_cnt <= 0) {
		// Soit on a une erreure de lecture, soit on est en fin de fichier
		f->_flag |= f->_cnt ? _IOERR : _IOEOF;
		return EOF;
	}

	// Comme on retourne le premier caractere, il faut decrementer le compteur
	f->_cnt--;

	return *f->_ptr;
}

/*
 * Ecriture d'un caractere dans le fichier.
 * Retourne le caractere en hexa en cas de succes
 * Retourne -1 en cas d'echec
 */
 int _flsbuf(unsigned char c, FILE *f) {
 	// Compteur de caractere dans le buffer
 	int count;

 	// Compteur de caractere ecrit
 	int written;

 	// Le fichier
 	char file = fileno(f);

 	// On verefie que le fichier soit ouvert en ecriture
 	if (!(f->_flag & _IOWRT)) {
 		// On indique qu'il y a une erreur
 		f->_flag |= _IOERR;
 		return -1;
 	}

 	// Si le fichier est en lecture, on verifie qu'on soit bien au bout
 	if (f->_flag & _IOREAD) {
 		f->_cnt = 0;
 		if (f->_flag & _IOEOF) {
 			// On se replace sur la base
 			f->_ptr = f->_base;
 			f->_flag &= ~_IOREAD;
 		} else {
 			// On indique une erreur
 			f->_flag |= _IOERR;
 			return -1;
 		}
 	}

 	// On indique qu'on est en ecriture et en debut de fichier
 	f->_flag |= _IOWRT;
 	f->_flag &= ~_IOEOF;
 	written = count = f->_cnt = 0;

 	// On alloue un buffer s'il y en a pas
 	if (f->_flag & _IONBF) {
 		alloc_buf(f);
 	}

 	// On verifie que l'on soit en bufferisation totale
 	if (f->_flag & _IOFBF) {
 		// On voit s'il y a des caracteres dans le buffer
 		count = f->_ptr - f->_base;

 		// On avance le caractere courant
 		f->_ptr = f->_base + 1;

 		// On diminue le nombre de caractere dans le buffer de 1
 		f->_cnt = f->_bufsiz - 1;

 		// S'il reste des caracteres dans le buffer, on les ecrits dans le fichier
 		if (count > 0) {
 			written = write(file, f->_base, count);
 		}

 		// On ajoute le caractere a la base
 		*f->_base = c;
 	} else {
 		// Sinon on ecrit directement dans le fichier
 		count = 1;
 		written = write(file, &c, count);
 	}

 	// On verifie que l'ecriture s'est bien passee
 	if (written != count) {
 		f->_flag |= _IOERR;
 		return -1;
 	}

 	// On retourne le caractere ecrit en hexadecimale
 	return c & 0xff;
 }

 FILE *fopen(const char *filename, const char *mode) {
 	int fflag;
 	int oflag;
 	char file;

 	FILE *f = malloc(sizeof(FILE));

 	
 	// echec du malloc
 	if(!f) {
 		return NULL;
 	}

 	switch (*mode) {
 		case 'r':
 			fflag = _IOREAD;
 			oflag = O_RDONLY;
 			break;
 		case 'w':
 			fflag = _IOWRT;
 			oflag = O_WRONLY | O_CREAT | O_TRUNC;
 			break;
 		case 'a':
 			fflag = _IOWRT;
 			oflag = O_WRONLY | O_CREAT | O_APPEND;
 			break;
 		// Erreur on libere le malloc et on quitte
 		default:
 			free(f);
 			return NULL;
 	}

 	// Ouverture du fichier
 	if ((file = open(filename, oflag)) < 0) {
		free(f);
		return NULL;
	}

	f->_flag = fflag;
	f->_cnt = 0;
	f->_base = f->_ptr = NULL;
	f->_file = file;

 	return f;
 }

void setbuf(FILE *stream, char *buf) {
	int size = sizeof(buf);

	if (stream->_flag & _IOFBF) {
		free_buf(stream);
	}

	if( (stream->_base = malloc(sizeof(size))) ) {
		// On a un buffer
		stream->_flag |= _IOFBF;
		stream->_bufsiz = size;
	} else {
		// On n'a pas de buffer
		stream->_flag |= _IONBF;
	}
	
	stream->_base = stream->_ptr = buf;

	// On indique que l'on est au debut du buffer
	stream->_cnt = 0;
}
