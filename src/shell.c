/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"
#include "traiter_cmd.c"

int main()
{
	struct cmdline *l;
	int i, j;
	int finShell = 0;

	//Boucle dont on sort lorsque l'on quitte le Shell
	while (finShell == 0)
	{
		printf("shell> ");
		//lecture et analyse (lexicale / syntaxique) de la commande
		l = readcmd();

		/* If input stream closed, normal termination */
		if (l == NULL) {
			finShell = 1;
		}
		else
		{
			if (l->err != NULL) {
				/* Syntax error, read another command */
				printf("error: %s\n", l->err);
			}
			//Si pas d'erreur, traitement de la commande
			else
			{
				if(traiter_commande(l)){
					finShell = 1;
				}
			}
		}
	}

	exit(0);
}
