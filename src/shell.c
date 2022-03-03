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

//Fonction utilisée pour la gestions des processus fils du MiniShell terminés (pour éviter l'invasion des zombis mais
//a d'autres utilités, nottament au niveau de l'affichage)
//Ce handler est mis en place via un appel à la fonction "gestion_SIGCHLD" au dessous
void handler_SIGCHLD(int sig) {
	//Récupération du PID du processus fils terminé et de son retour
	int statutFinProcessusFilsTermine;

	int pidFilsTermine = waitpid(-1, &statutFinProcessusFilsTermine, WNOHANG | WUNTRACED);

	//Si un processus a bien été intercepté (car WNOHANG n'est pas bloquant)
	if(pidFilsTermine > 0)
	{
		//On regarde comment s'est terminé le processus
		retoursTraitementCommande retourProcessusFilsTermine = WEXITSTATUS(statutFinProcessusFilsTermine);

		int processusAEuProblemeDExecution = 0;

		//Si le processus fils ne s'est pas terminé correctement
		if(WIFEXITED(statutFinProcessusFilsTermine) == 0)
		{
			processusAEuProblemeDExecution = 1;
		}
		//Si le processus fils s'est terminé correctement mais que le signal émis n'était pas le signal normal
		else
		{
			//Si le signal émis indique qu'il y a eu des problèmes lors de l'exécution de la commande
			if(retourProcessusFilsTermine == COMMANDE_INTERNE_PAS_TROUVEE
			|| retourProcessusFilsTermine == ERREUR_EXECUTION_COMMANDE)
			{
				processusAEuProblemeDExecution = 1;
			}
			else
			{
				//Si le signal émis indique de fermer le MiniShell
				if(retourProcessusFilsTermine == FERMETURE_SHELL)
				{
					finShell = 1;
					printf("exit\n");
				}
			}
		}

		//Si le processus terminé a eu des problèmes d'exécution
		//Pour le moment, l'action associée à cette situation n'est pas décidée.
		//->Là on ne fait rien de spécial mais deux comportements étaient envisagés:
		//  -Afficher une erreur à l'utilisateur et fermer le shell.
		//  -Envoyer un signal pour kill les processus foreground encore en cours d'exécution.
		if(processusAEuProblemeDExecution == 1)
		{
			/*printf("Erreur lors de l'exécution de la commande, fermeture du MiniShell.\n");

			finShell = 1;*/
		}


		//Variable permettant de savoir si le processus terminé était un processus background ou foreground
		int etaitProcessusTermineForeground = 0;

		//Enfin, on retire de la liste des PID foreground le pid du processus qui vient de se terminer
		if(SupprimerElementListeInt(pidsCommandeForeground, pidFilsTermine) == 0)
		{
			etaitProcessusTermineForeground = 1;
		}
			
		if(EstListeIntVide(pidsCommandeForeground))
		{
			estCommandeForegroundEnCours = 0;
		}

		//Si il n'y a pas de processus en foreground et que le processus terminé était en background (il a pu perturber l'affichage)
		//->affichage à l'utilisateur que le MiniShell est disponible
		if(estCommandeForegroundEnCours == 0 && etaitProcessusTermineForeground == 0)
		{
			printf("shell> ");
			fflush(stdout);
		}
		//On n'effectue cet affichage que lorsque le processus terminé était en background pour éviter d'afficher "shell> "
		//plusieurs fois de suite à l'utilisateur (le message est affiché à la fin de chaque processus foreground, voir main())
	}
}

//Fonction de mise en place du handler handler_SIGCHLD permettant, entre autres, d'éviter l'invasion des zombis.
void gestion_SIGCHLD() {
  Signal(SIGCHLD, handler_SIGCHLD);
}

//Fonction main du MiniShell
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
