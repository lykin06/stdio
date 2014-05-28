#inlcude "stdio.h"

struct IOB[100] =
{
	{0,NULL,NULL,0,1,0},
	{0,NULL,NULL,0,1,1},
	{0,NULL,NULL,0,1,2}
};

int _filbuf(FILE * f) {
	read(f->_file, f->_ptr, f->_bufsiz);
	return 0;
}


