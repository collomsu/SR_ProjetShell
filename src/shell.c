/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"
#include "traiter_cmd.h"
#include "listeInt.h"
#include "variablesExternes.h"

int finShell = 0;
//Variables externes du MiniShell
extern int estCommandeForegroundEnCours;
extern listeInt* pidsCommandeForeground;

//Fonctions utilisées pour la gestions des processus fils du MiniShell terminés
void handler_SIGCHLD(int sig) {
	//Récupération du PID du processus fils terminé et de son retour
	int statutFinProcessusFilsTermine;

	int pidFilsTermine = waitpid(-1, &statutFinProcessusFilsTermine, WNOHANG | WUNTRACED);

	//Si un processus a bien été intercepté (car WNOHANG n'est pas bloquant)
	if(pidFilsTermine > 0)
	{
		retoursTraitementCommande retourProcessusFilsTermine = WEXITSTATUS(statutFinProcessusFilsTermine);

		int aCommandeEchoue = 0;

		//Si le processus fils a s'est terminé par une erreur
		if(WIFEXITED(statutFinProcessusFilsTermine) == 0)
		{
			aCommandeEchoue = 1;
		}
		else
		{
			if(retourProcessusFilsTermine == COMMANDE_INTERNE_PAS_TROUVEE
			|| retourProcessusFilsTermine == ERREUR_EXECUTION_COMMANDE)
			{
				aCommandeEchoue = 1;
			}
			else
			{
				//Si le processus terminé implique la fermeture du MiniShell
				if(retourProcessusFilsTermine == FERMETURE_SHELL)
				{
					finShell = 1;
					printf("exit\n");
				}
			}
		}

		//Si le processus terminé souffre d'une erreur, on affiche une erreur à l'utilisateur et on ferme le shell
		if(aCommandeEchoue == 1)
		{
			printf("Erreur lors de l'exécution de la commande, fermeture du MiniShell.\n");

			finShell = 1;
		}
		//Sinon, on retire de la liste des PID foreground le pid du processus qui vient de se terminer
		else
		{
			SupprimerElementListeInt(pidsCommandeForeground, pidFilsTermine);
			
			if(EstListeIntVide(pidsCommandeForeground))
			{
				estCommandeForegroundEnCours = 0;
			}
		}
	}
}

void gestion_SIGCHLD() {
  Signal(SIGCHLD, handler_SIGCHLD);
}

int main()
{
	//Initialisation des variables externes
	estCommandeForegroundEnCours = 0;
	pidsCommandeForeground = NouvelleListeInt();

	//Mise en place de la gestions des processus fils du MiniShell terminés
	gestion_SIGCHLD();

	struct cmdline *l;
	retoursTraitementCommande retourCommande;

	//Boucle dont on sort lorsque l'on quitte le Shell
	while (finShell == 0)
	{
		//Attente que la commande en foreground soit terminée
		while(estCommandeForegroundEnCours && finShell == 0);

		if(finShell == 0)
		{
			printf("shell> ");
			//lecture et analyse (lexicale / syntaxique) de la commande
			l = readcmd();

			/* If input stream closed, normal termination */
			if (l == NULL) {
				finShell = 1;
				printf("exit\n");
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
