#include "traiter_cmd.h"
#include "fonctions_utiles.h"

//Variables externes du MiniShell
extern int numJobCommandeForeground;
extern listeJobs* listeJobsShell;

retoursTraitementCommande traiter_commande(struct cmdline *l, int numeroJobCommande) {
  retoursTraitementCommande retour = NORMAL;

  int estCommandeForeground = (l->bg == 0);

  //Si la commande doit être effectuée au premier plan
  if(estCommandeForeground)
  {
    //Modification de la variable globale indiquant le PID du processus actuellement en foreground.
    numJobCommandeForeground = numeroJobCommande;
  }

  //Appel d'une fonction traitant la commande avec et sans pipe
  retour = executer_commande_pipe(l, numeroJobCommande);

  return retour;
}

//Fonction traitant une commande sans pipe
retoursTraitementCommande executer_commande_simple(char **commande, int fdIn, int fdOut)
{
  retoursTraitementCommande retour = NORMAL;

  //le processus est transformé en la commande passé en paramètre
  //On commence par effectuer les redirections
  Dup2(fdIn, 0);
  Dup2(fdOut, 1);

  //Ensuite, on contrôle si la commande n'est pas une commande interne au Shell
  retour = executer_commande_interne(commande);

  //Si la commande n'est pas une commande interne
  if(retour == COMMANDE_INTERNE_PAS_TROUVEE)
  {
    retour = execvp_correct(execvp(commande[0], commande), commande);
  }

  return retour;
}

retoursTraitementCommande executer_commande_interne(char **commande)
{
  retoursTraitementCommande retour = COMMANDE_INTERNE_PAS_TROUVEE;

  //Commandes de sortie du Shell
  if(strcmp(commande[0], "exit") == 0 || strcmp(commande[0], "quit") == 0)
  {
    retour = FERMETURE_SHELL;
  } else if(strcmp(commande[0], "jobs") == 0) {
    retour = NORMAL;

    //Parcours de la liste des jobs pour afficher leurs caractéristiques
    struct elementListeJobs *ElementParcoursListeJobs = listeJobsShell->tete;

    char *etatJob;

    while(ElementParcoursListeJobs != NULL)
    {
      switch (listeJobsShell->tete->etatJob) {
        case RUNNING:
          etatJob = "Running";
          break;
        case STOPPED:
          etatJob = "Stopped";
          break;
        case NON_DEFINI:
          etatJob = "Error, state not defined";
          break;
        default:
          break;
      }
      printf("[%d]  %s  %s\n",listeJobsShell->tete->numeroJob, etatJob, listeJobsShell->tete->commandeLancementJob);
    }
  } else if(strcmp(commande[0], "fg") == 0) {
    retour = NORMAL;

  } else if(strcmp(commande[0], "bg") == 0) {
    retour = NORMAL;

  }

  return retour;
}

//Fonction traitant une commande avec ou sans pipe
//->Appel à la fonction executer_commande_simple
retoursTraitementCommande executer_commande_pipe(struct cmdline *l, int numeroJobCommande) {
  retoursTraitementCommande retour = NORMAL;

  //Création de chacun des fils des sous-commandes de la série de commandes et exécution
  int pidFilsCree = 0;
  int i = 0;
  int fdIn = 0, fdOut = 1;
  int fdAFermerPere = 0; //fd qui a été ouvert pour le fils précédent (si aEteOuvertTuyauPrecedent == 1) à fermer chez le père une fois que le fils actuel sera créé
  int fd[2];
  int aEteOuvertTuyau, aEteOuvertTuyauPrecedent = 0;

  int permissions;

  listeInt* listePidsCommande = GetElementListeJobsByNumero(listeJobsShell, numeroJobCommande)->listePIDsJob;

  while (l->seq[i] != NULL)
  {
    aEteOuvertTuyau = 0;

    //Si le fils actuellement créé a comme sortie un pipe
    if(l->seq[i + 1] != NULL)
    {
      if(pipe(fd) == -1) {
        perror("Pipeline failed");
        fflush(stdout);
      }

      aEteOuvertTuyau = 1;
    }

    //Création du fils qui exécutera cette partie de la commande
    pidFilsCree = Fork();

    if(pidFilsCree == 0)
    {
      //Si il y a une redirection de l'entrée et que le fils actuellement créé est le premier de la série
      if(i == 0 && l->in != NULL) {
        permissions = verification_permissions_fichier(l->in);
        if(permissions == -1) {
          printf("%s: File not found.\n", l->in);
          fflush(stdout);
          retour = ERREUR_REDIRECTION_FICHIER;
        } else if (permissions < 400) {
          printf("%s: Permission denied.\n", l->in);
          fflush(stdout);
          retour = ERREUR_REDIRECTION_FICHIER;
        } else {
          fdIn = open(l->in, O_RDONLY, 0);
        }
      }
      //Si il y a une redirection de la sortie et que le fils actuellement créé est le dernier de la série
      //(pas incompatible avec le cas précédent)
      if(l->seq[i + 1] == NULL && l->out != NULL) {
        // Ajout d'une gestion des permissions sur redir sortie pour éviter un "bad file descriptor" (partie 5)
        int i = verification_permissions_fichier(l->out);
        if ((i >= 0 && i < 200) || (i >= 400 && i < 600)) {
          printf("%s: Permission denied.\n", l->out);
          fflush(stdout);
          retour = ERREUR_REDIRECTION_FICHIER;
        } else {
          fdOut = open(l->out, O_WRONLY | O_CREAT, S_IRWXU);
        }
      }

      //Si il y a eu un problème avec les redirections de fichiers, on exit le code d'erreur
      if(retour == ERREUR_REDIRECTION_FICHIER)
      {
        exit(retour);
      }

      //Si le fils actuellement créé a comme sortie un pipe
      if(aEteOuvertTuyau == 1)
      {
        fdOut = fd[1];
        close(fd[0]);
      }

      exit(executer_commande_simple(l->seq[i], fdIn, fdOut));
    }
    else
    {
      //Insertion du PID du fils créé dans la liste des PIDs du job de la commande
      AjouterElementListeInt(listePidsCommande, pidFilsCree);

      if(aEteOuvertTuyauPrecedent == 1)
      {
        close(fdAFermerPere);

        aEteOuvertTuyauPrecedent = 0;
      }

      //Préparation de l'entrée pour le prochain fils si son entrée est la sortie de l'une des commandes de la série (dans un tuyau)
      if(aEteOuvertTuyau == 1)
      {
        fdIn = fd[0];
        close(fd[1]);

        fdAFermerPere = fdIn;
      }

      fdOut = 1;
    }

    i = i + 1;
  }

  return retour;
}
