#include "handlersSignaux.h"

//Variables externes du MiniShell
extern int finShell;
extern int numJobCommandeForeground;
extern listeJobs* listeJobsShell;
extern sigset_t structureDeSignaux;


//-1) Handlers de signaux

void handler_SIGCHLD(int sig) {
	//Masquage des signaux contenus dans la structure de données
	Sigprocmask(SIG_BLOCK, &structureDeSignaux, NULL);

	//Récupération du PID du processus fils terminé et de son retour
	int statutFinProcessusFilsTermine;

	int pidFilsTermine = waitpid(-1, &statutFinProcessusFilsTermine, WNOHANG | WUNTRACED);

	//Si un processus a bien été intercepté (car WNOHANG n'est pas bloquant)
	if(pidFilsTermine > 0)
	{
		//Si le processus fils est un processus fils terminé (il a exit)
		if(WIFSTOPPED(statutFinProcessusFilsTermine) == 0 && WIFCONTINUED(statutFinProcessusFilsTermine) == 0)
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
				|| retourProcessusFilsTermine == ERREUR_EXECUTION_COMMANDE
				|| retourProcessusFilsTermine == ERREUR_REDIRECTION_FICHIER)
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
						fflush(stdout);
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

			//Enfin, on retire de la liste des PID de processus le pid du processus qui vient de se terminer
			//On cherche à quel job appartenait le processus terminé
			struct elementListeJobs *ElementParcoursListeJobs = listeJobsShell->tete;
			while(SupprimerElementListeInt(ElementParcoursListeJobs->listePIDsJob, pidFilsTermine) == -1)
			{
				ElementParcoursListeJobs = ElementParcoursListeJobs->suivant;
			}

			//Si la liste des PIDs du job dont l'un des processus vient de se terminer est vide, on supprime ce job
			if(EstListeIntVide(ElementParcoursListeJobs->listePIDsJob))
			{
				SupprimerElementListeJobs(listeJobsShell, ElementParcoursListeJobs->numeroJob);
			}
			
			//Si dans la liste des jobs: le job qui, avant reception du SIGCHLD, était au premier plan n'existe plus
			//->Mise à jour de la variable numJobCommandeForeground
			if(GetElementListeJobsByNumero(listeJobsShell, numJobCommandeForeground) == NULL)
			{
				numJobCommandeForeground = -1;
			}

			//Si il n'y a pas de processus en foreground->affichage à l'utilisateur que le MiniShell est disponible
			if(numJobCommandeForeground == -1)
			{
				printf("shell> ");
				fflush(stdout);
			}
			//On n'effectue cet affichage que lorsque le processus terminé était en background pour éviter d'afficher "shell> "
			//plusieurs fois de suite à l'utilisateur (le message est affiché à la fin de chaque processus foreground, voir main())
		}
	}

	//Démasquage des signaux contenus dans la structure de données
	Sigprocmask(SIG_UNBLOCK, &structureDeSignaux, NULL);
}

void handler_SIGINT(int sig) {
	//Masquage des signaux contenus dans la structure de données
	Sigprocmask(SIG_BLOCK, &structureDeSignaux, NULL);

	//On envoie à chaque processus en cours d'exécution au premier plan le signal SIGINT
	if(numJobCommandeForeground != -1)
	{
		struct elementListeJobs* jobArrete = GetElementListeJobsByNumero(listeJobsShell, numJobCommandeForeground);
		struct elementListeInt *ElementParcoursPidsJobForeground = jobArrete->listePIDsJob->tete;

		while (ElementParcoursPidsJobForeground != NULL)
		{
			//Envoi du signal au processus via la fonction kill
			if(kill(ElementParcoursPidsJobForeground->valeur, SIGINT) == -1) {
				perror("kill");
				fflush(stdout);
			}

			ElementParcoursPidsJobForeground = ElementParcoursPidsJobForeground->suivant;
		}
	}

	//On imprime un retour à la ligne et "shell> " (pour la propreté de l'affichage)
	printf("\n");
	printf("shell> ");
	fflush(stdout);

	//Démasquage des signaux contenus dans la structure de données
	Sigprocmask(SIG_UNBLOCK, &structureDeSignaux, NULL);
}

void handler_SIGTSTP(int sig) {
	//Masquage des signaux contenus dans la structure de données
	Sigprocmask(SIG_BLOCK, &structureDeSignaux, NULL);

	//On envoie à chaque processus en cours d'exécution au premier plan le signal SIGTSTP
	if(numJobCommandeForeground != -1)
	{
		struct elementListeJobs* jobMisEnPause = GetElementListeJobsByNumero(listeJobsShell, numJobCommandeForeground);

		//On indique dans la liste des jobs que le processus qui était au premier plan passe en background
		jobMisEnPause->etatJob = STOPPED;

		struct elementListeInt *ElementParcoursPidsJobForeground = jobMisEnPause->listePIDsJob->tete;

		while (ElementParcoursPidsJobForeground != NULL)
		{
			//Envoi du signal au processus via la fonction kill
			if(kill(ElementParcoursPidsJobForeground->valeur, SIGTSTP) == -1) {
				perror("kill");
				fflush(stdout);
			}

			ElementParcoursPidsJobForeground = ElementParcoursPidsJobForeground->suivant;
		}

		//Il n'y a plus de commande au premier plan
		numJobCommandeForeground = -1;
	}

	//On imprime un retour à la ligne et "shell> " (pour la propreté de l'affichage)
	printf("\n");
	printf("shell> ");
	fflush(stdout);

	//Démasquage des signaux contenus dans la structure de données
	Sigprocmask(SIG_UNBLOCK, &structureDeSignaux, NULL);
}


//-2) Fonctions de mise en place des handlers

void setup_handler_SIGCHLD() {
  Signal(SIGCHLD, handler_SIGCHLD);
}

void setup_handler_SIGINT() {
  Signal(SIGINT, handler_SIGINT);
}

void setup_handler_SIGTSTP() {
  Signal(SIGTSTP, handler_SIGTSTP);
}

//-3) Fonction d'initialisation d'une structure de données de signaux

void setup_masque_signaux() {
	Sigemptyset(&structureDeSignaux);

	Sigaddset(&structureDeSignaux, SIGINT);
	Sigaddset(&structureDeSignaux, SIGCHLD);
	Sigaddset(&structureDeSignaux, SIGTSTP);

	Sigprocmask(SIG_UNBLOCK, &structureDeSignaux, NULL);
}
