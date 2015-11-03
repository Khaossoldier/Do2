#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <getopt.h>
#include "makeargv.h"

char*
arguments
(int argc, char *argv[], char* argument){
	int next_option, i;
	struct option options[5] =
	{
		{"and", no_argument, NULL, 'a'},
		{"or", no_argument, NULL, 'o'},
		{"cc", no_argument, NULL, 'c'},
		{"kill", no_argument, NULL, 'k'},
		{0,0,0,0}
	};

	i = 0;

	do
	{
		next_option = getopt_long(argc, argv, "", options, NULL);
		argument[i] = (char) next_option;
		i++;
	}
	while (next_option != -1);

	return argument;
}

int
recherche_dans_tableau
(char c, char* tab){
	int i;
	for (i = 0; i < sizeof(tab); i++){
		if (tab[i] == c)
			return 1;
	}
	return 0;
}

int
main
(int argc, char *argv[])
{
	int i, status, j, nb;
	char* options, argument;
	char** buf;

	nb = 1;
	options = arguments(argc, argv, &argument);
	printf("%c, %c, %c, %c\n", options[0], options[1], options[2], options[3]);
	if (recherche_dans_tableau('a', options))
		j = EXIT_SUCCESS;
	else
		j = EXIT_FAILURE;
	
	for (i = 1; i < argc; i++){
		if(argv[i][0] != '-'){
			switch(fork()){
					case -1: exit(EXIT_FAILURE);
					case 0: makeargv(argv[i], " ", &buf);
							execvp(buf[0], buf);
							freeargv(buf);
							exit(EXIT_FAILURE);
					default: break;
			}
		}
		else{
			nb++;
		}
	}
	for (i = nb; i < argc; i++){
		wait(&status);
		if (WIFEXITED(status)){
			if(recherche_dans_tableau('a', options)){
				j = !(!j && !(WEXITSTATUS(status)));
				if (recherche_dans_tableau('c', options) && j == 1){
					i = argc;
				}
			}
			else if(recherche_dans_tableau('o', options)){
				j = !(!j || !(WEXITSTATUS(status)));
				if (recherche_dans_tableau('c', options) && j == 0){
					i = argc;
				}
			}
		}
	}
	return j;
}