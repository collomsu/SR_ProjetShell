/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"
#include "variablesExternes.h"
#include "traiter_cmd.h"
#include "listeInt.h"
#include "handlersSignaux.h"

extern int finShell;
//Variables externes du MiniShell
extern int estCommandeForegroundEnCours;
extern listeInt* pidsCommandeForeground;



//Fonction main du MiniShell
int main()
{
	//Initialisation des variables externes
	finShell = 0;
	estCommandeForegroundEnCours = 0;
	pidsCommandeForeground = NouvelleListeInt();

	//Mise en place des handlers associés aux signaux SIGCHLD, SIGINT et SIGTSTP
	setup_handler_SIGCHLD();
	setup_handler_SIGINT();
	setup_handler_SIGTSTP();

	struct cmdline *l;
	retoursTraitementCommande retourCommande;

	//Boucle dont on sort lorsque l'on quitte le Shell
	while (finShell == 0)
	{
		//Attente que la commande en foreground soit terminée
		while(estCommandeForegroundEnCours && finShell == 0)
		{

		}

		if(finShell == 0)
		{
			//Affichage à l'utilisateur que le MiniShell est disponible
			printf("shell> ");
			fflush(stdout);
			//Cet affichage est effectué à chaque lancement du MiniShell + à chaque fin de processus foreground

			//lecture et analyse (lexicale / syntaxique) de la commande
			l = readcmd();

			/* If input stream closed, normal termination */
			if (l == NULL) {
				finShell = 1;
				printf("exit\n");
				fflush(stdout);
			}
			else
			{
				if (l->err != NULL) {
					/* Syntax error, read another command */
					printf("error: %s\n", l->err);
					fflush(stdout);
				}
				//Si pas d'erreur, traitement de la commande
				else
				{
					printf("\nCommande écrite: %s\n\n", getChaineCommandeComplete(l));
					retourCommande = traiter_commande(l);

					if(retourCommande == FERMETURE_SHELL){
						finShell = 1;
					}
				}
			}
		}
	}

	exit(0);
}
