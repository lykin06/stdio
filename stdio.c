#include "stdio.h"
#include <stdlib.h>
#include <unistd.h>

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
		f->_flag = _IOFBF;
		f->_bufsiz = BUFSIZ;
	} else {
		// On n'a pas de buffer
		f->_flag = _IONBF;
	}

	// On fait poiter le premier element sur la base
	f->_ptr = f->_base;

	// On indique que l'on a un buffer vide
	f->_cnt = 0;
}

/*
 * Libere le buffer
 */
 void free_buf(FILE * f) {
 	if (f->_flag == _IOFBF) {
 		free(f->_base);
 		f->_flag = _IONBF;
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
	if (f->_flag == _IOWRT) {
		f->_flag = _IOERR;
		return EOF;
	}

	// Aucun buffer aloue
	if (f->_flag == _IONBF) {
		alloc_buf(f);
	} else {
		// Sinon on place le caractere courant sur la base
		f->_ptr = f->_base;
	}

	f->_cnt = read(f->_file, f->_ptr, f->_bufsiz);

	if(f->_cnt <= 0) {
		// Soit on a une erreure de lecture, soit on est en fin de fichier
		f->_flag = f->_cnt ? _IOERR : _IOEOF;
		return EOF;
	}

	// Comme on retourne le premier caractere, il faut decrementer le compteur
	f->_cnt--;

	return *f->_ptr;
}


