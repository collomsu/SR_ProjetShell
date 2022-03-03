#include "traiter_cmd.h"

//Variables externes du MiniShell
extern int estCommandeForegroundEnCours;
extern listeInt* pidsCommandeForeground;

retoursTraitementCommande traiter_commande(struct cmdline *l) {
  retoursTraitementCommande retour = NORMAL;

  //Si la commande doit être effectuée au premier plan
  if(l->bg == 0)
  {
    //Mise à 1 de la variable globale indiquant si une commande est déjà en train d'être exécutée en foreground.
    estCommandeForegroundEnCours = 1;

    //Remise à zéro de la liste des pids des processus en foreground
    DetruireListeInt(pidsCommandeForeground);
    pidsCommandeForeground = NouvelleListeInt();
  }

  //Appel d'une fonction traitant la commande avec et sans pipe
  retour = executer_commande_pipe(l);

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

  exit(retour);
}

int verification_permissions_fichier(char* fichier) {
  int permissions;
  int fd[2], pidFils;
  pipe(fd);
  char sortie[3];
  char* commande[5] = {"stat","-c","%a",fichier,NULL};
  if((pidFils = Fork()) == 0) {
    close(fd[0]);
    Dup2(fd[1], 1);
    Dup2(fd[1], 2);
    close(fd[1]);
    execvp(commande[0],commande);
    exit(0);
  } else {
    close(fd[1]);
    read(fd[0], &sortie, 3);
    close(fd[0]);
    if(sortie[0] == 's') {
      return -1;
    } else {
      sscanf(sortie, "%d", &permissions);
    }
    Waitpid(pidFils, NULL, 0);
  }
  return permissions;
}

retoursTraitementCommande executer_commande_interne(char **commande)
{
  retoursTraitementCommande retour = COMMANDE_INTERNE_PAS_TROUVEE;

  //Commandes de sortie du Shell
  if(strcmp(commande[0], "exit") == 0 || strcmp(commande[0], "quit") == 0)
  {
    retour = FERMETURE_SHELL;
  }

  return retour;
}

//Fonction traitant une commande avec ou sans pipe
//->Appel à la fonction executer_commande_simple
retoursTraitementCommande executer_commande_pipe(struct cmdline *l) {
  retoursTraitementCommande retour = NORMAL;

  //Création de chacun des fils des sous-commandes de la série de commandes et exécution
  int pidFilsCree = 0;
  int i = 0;
  int fdIn = 0, fdOut = 1;
  int fdAFermerPere = 0; //fd qui a été ouvert pour le fils précédent (si aEteOuvertTuyauPrecedent == 1) à fermer chez le père une fois que le fils actuel sera créé
  int fd[2];
  int aEteOuvertTuyau, aEteOuvertTuyauPrecedent = 0;

  int permissions;

  while (l->seq[i] != NULL)
  {
    aEteOuvertTuyau = 0;
    
    //Si le fils actuellement créé a comme sortie un pipe
    if(l->seq[i + 1] != NULL)
    {
      if(pipe(fd) == -1) {
        perror("Pipeline failed");
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
          return ERREUR_EXECUTION_COMMANDE;
        } else if (permissions < 400) {
          printf("%s: Permission denied.\n", l->in);
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
        } else {
          fdOut = open(l->out, O_WRONLY | O_CREAT, S_IRWXU);
        }
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
      //Insertion du PID du fils créé dans la liste si la commande est en foreground
      if(l->bg == 0)
      {
        AjouterElementListeInt(pidsCommandeForeground, pidFilsCree);
      }

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

retoursTraitementCommande execvp_correct(int retourLancementCommande, char **commande) {
  retoursTraitementCommande retour = NORMAL;
  //Si la commande ne s'est pas lancée correctement
  if(retourLancementCommande == -1) {
    printf("Erreur lors du lancement de la commande \"");

    afficherCommande(commande);

    printf("\".\n");

    perror("exec");

    retour = ERREUR_EXECUTION_COMMANDE;
  }
  return retour;
}

void afficherCommande(char **commande)
{
  int i = 0;

  while (commande[i] != NULL)
    {
      printf("%s", commande[i]);

      i = i + 1;
    }
}