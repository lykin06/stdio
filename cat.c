/*							-*- coding: utf-8 -*-
 * complexcat.c			-- Un cat compliquÃ©
 *
 *           Author: Erick Gallesio [eg@unice.fr]
 *    Creation date:  4-Mar-2012 19:03 (eg)
 * Last file update:  4-Mar-2012 19:17 (eg)
 */

#include "stdio.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void copy_fd(int fd_src, int fd_dst)
{
  int n;
  char buff[100];

  /* On suppose que c'est OK. Pas vraiment de contÃ´le sur les retours
   * de read et write ici */
  while ((n = read(fd_src, buff, sizeof(buff))) > 0)
    write(fd_dst, buff, n);
}

void die(char *progname)
{
  //perror(progname);
  exit(1);
}

int main(int argc, char *argv[])
{
  int fd, p[2];

  /* ContrÃ´le de  l'argument 
  if (argc != 2) {
    fprintf(stderr, "Usage: %s file\n", *argv);
    exit(1);
  }*/

  /* CrÃ©ation du pipe */
  if (pipe(p))
    die(*argv);


  if (fork() == 0) {
    /* processus fils: lecture depuis le pipe vers stdout */
    close(p[1]);			/* fermer la branche de pipe inutile */
    copy_fd(p[0], 1);
    close(p[0]);
    exit(0);
  } else {
    /* processus pÃ¨re: lecture depuis le fichier vers stdout */
    if ((fd = open(argv[1], O_RDONLY)) < 0)
	die(*argv);
    close(p[0]);			/* fermer la branche de pipe inutile */
    copy_fd(fd, p[1]);
    close(p[1]);
  }
  return 0;
}