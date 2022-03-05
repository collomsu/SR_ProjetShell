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
#include "listeJobs.h"
#include "fonctions_utiles.h"

extern int finShell;
//Variables externes du MiniShell
extern int numJobCommandeForeground;
listeJobs* listeJobsShell;
extern listeJobs* listeJobsShell;



//Fonction main du MiniShell
int main()
{
	//Initialisation des variables externes
	finShell = 0;
	numJobCommandeForeground = -1;
	listeJobsShell = NouvelleListeJobs();

	//Initialisation de la structure de données permettant de gérer les signaux
	setup_masque_signaux();


	//Mise en place des handlers associés aux signaux SIGCHLD, SIGINT et SIGTSTP
	setup_handler_SIGCHLD();
	setup_handler_SIGINT();
	setup_handler_SIGTSTP();

	struct cmdline *l;
	retoursTraitementCommande retourCommande;

	//Affichage à l'utilisateur que le MiniShell est disponible
	printf("shell> ");
	fflush(stdout);
	//Cet affichage est effectué à chaque lancement du MiniShell + à chaque fin de processus foreground

	//Boucle dont on sort lorsque l'on quitte le Shell
	while (finShell == 0)
	{
		//Attente que la commande en foreground soit terminée
		while(numJobCommandeForeground != -1 && finShell == 0)
		{

		}

		if(finShell == 0)
		{
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
				//Si pas d'erreur, ajout d'un nouveau job à la liste et traitement de la commande
				else
				{
					int numeroJobCommande = AjouterElementListeJobs(listeJobsShell, getChaineCommandeComplete(l), RUNNING);

					retourCommande = traiter_commande(l, numeroJobCommande);

					if(retourCommande == FERMETURE_SHELL){
						finShell = 1;
					}
				}
			}
		}
	}

	exit(0);
}
