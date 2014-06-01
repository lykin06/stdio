#include "stdio.h"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

int copy(char* s, char* d) {
	int n;
	FILE * source;
	FILE * dest;

	// Comparaison des deux chemins pour voir s'ils sont differents
	if (strcmp(s, d)) {
		printf("Erreur - chemins identiques.\n");
		return -1;
	}

	// On ouvre la source en lecture
	if ( (source = fopen(s, "r")) ) {
		printf("Erreur - Impossible d'ouvrir le fichier source.\n");
		return -1;
	}
	
	// On ouvre la destination en ecriture
	if ( (dest = fopen(s, "w")) ) {
		printf("Erreur - Impossible d'ouvrir le fichier destination.\n");
		return -1;
	}

	// Les fichiers sont bien ouvert, on peut copier
	while ( (n = getc(source)) ) {
		putc(source->_base, dest);
	}

	fclose(source); fclose(dest);
	return 0;
}

int main(int argc, char** argv) {
	char* dest;

	// On verifie que l'on est bien les chemins des deux fichiers en argument
	if(argc <= 2) {
		printf("pas assez d'arguments.\n");
		return EXIT_FAILURE;
	}

	dest = argv[argc--];
	if (copy(argv[argc], dest) != 0) {
		printf("erreur de copy.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}