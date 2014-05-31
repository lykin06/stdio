/***
 *
 * Re-ecriture des fonctions de base de stdio
 *
 *           Author: Erick Gallesio [eg@unice.fr]
 *    Creation date: 27-Oct-1992 09:28
 * Last file update: 11-May-2014 23:45 (eg)
 *
 */


/**** Ce qui suit est le fichier standard stdio.h de Sun OS 4.x
      (legerement modifie) ****/

# ifndef FILE
#define	BUFSIZ	1024
#define _SBFSIZ	8
extern	struct	_iobuf {
	int				_cnt; 			// nbr de caracteres dans le buffeur
	unsigned char*  _ptr;			// caractere courant
	unsigned char*  _base;			// adresse du debut du buffeur
	int				_bufsiz;		// taille du buffeur
	short			_flag;
	char			_file;			// fichier
} _IOB[];

// Flags en octal
#define _IOFBF	 0					// Full Bufferisation
#define	_IOREAD	 01
#define	_IOWRT	 02
#define	_IONBF	 04					// Non bufferise
#define	_IOMYBUF 010
#define	_IOEOF	 020 				// Fin de fichier
#define	_IOERR	 040 				// Erreur
#define	_IOSTRG	 0100				// Non utilise
#define	_IOLBF	 0200				// bufferisation par ligne
#define	_IORW	 0400
#define	NULL	 ((void*) 0)
#define	FILE	 struct _iobuf
#define	EOF	 (-1)

#define	stdin	 (&_IOB[0])
#define	stdout	 (&_IOB[1])
#define	stderr	 (&_IOB[2])

// utiliser son propre buffer
extern int _flsbuf(unsigned char c, FILE *f);
extern int _filbuf(FILE * f);

#ifdef lint	/* so that lint likes (void)putc(a,b) */
extern int putc();
extern int getc();
#else
#define	getc(p)		(--(p)->_cnt>=0? ((int)*(p)->_ptr++):_filbuf(p))
#define putc(x, p)	(--(p)->_cnt >= 0 ?\
	(int)(*(p)->_ptr++ = (unsigned char)(x)) :\
	(((p)->_flag & _IOLBF) && -(p)->_cnt < (p)->_bufsiz ?\
		((*(p)->_ptr = (unsigned char)(x)) != '\n' ?\
			(int)(*(p)->_ptr++) :\
			_flsbuf(*(unsigned char *)(p)->_ptr, p)) :\
		_flsbuf((unsigned char)(x), p)))
#endif
#define	getchar()	getc(stdin)
#define	putchar(x)	putc((x),stdout)
#define	feof(p)		(((p)->_flag&_IOEOF)!=0)
#define	ferror(p)	(((p)->_flag&_IOERR)!=0)
#define	fileno(p)	((p)->_file)
#define	clearerr(p)	(void) ((p)->_flag &= ~(_IOERR|_IOEOF))


FILE *fopen(const char *path, const char *mode);
FILE *fdopen(int fd, const char *mode);
FILE *freopen(const char *path, const char *mode, FILE *stream);
int fclose(FILE *fp);

FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);

FILE *tmpfile(void);

int fgetc(FILE *stream);
char *fgets(char *s, int size, FILE *stream);
char *gets(char *s);

int fputc(int c, FILE *stream);
int fputs(const char *s, FILE *stream);
int puts(const char *s);

int printf(const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);
int sprintf(char *str, const char *format, ...);

void setbuf(FILE *stream, char *buf);
int setvbuf(FILE *stream, char *buf, int mode, int size);
#endif
