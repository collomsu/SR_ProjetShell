#include "traiter_cmd.h"

retoursTraitementCommande traiter_commande(struct cmdline *l) {
  retoursTraitementCommande retour = NORMAL;

  int i;

  //Si la commande est une commande simple (avec redirection ou non)
  //Un processus fils est créé pour l'exécution de cette commande
  if(l->seq[1] == NULL)
  {
    int fdEntreeCommande = 0, fdSortieCommande = 1;

    //Si il y a une redirection de l'entrée
    if(l->in != NULL)
    {
// *** Ajout d'une gestion des permissions sur redir entrée pour éviter un "bad file descriptor" (partie 5) ***
      i = verification_permissions_fichier(l->in);
      if(i == -1){
        printf("%s: File not found.\n", l->in);
        return ERREUR_EXECUTION_COMMANDE;
      } else if (i < 400) {
        printf("%s: Permission denied.\n", l->in);
      } else {
// *** FIN ***
        fdEntreeCommande = open(l->in, O_RDONLY, 0);
      }
    }

    //Si il y a une redirection de la sortie
    if(l->out != NULL)
    {
// *** Ajout d'une gestion des permissions sur redir sortie pour éviter un "bad file descriptor" (partie 5) ***
      i = verification_permissions_fichier(l->out);
      if ((i >= 0 && i < 200) || (i >= 400 && i < 600)) {
        printf("%s: Permission denied.\n", l->out);
      } else {
// *** FIN ***
        fdSortieCommande = open(l->out, O_WRONLY | O_CREAT, S_IRWXU);
      }
    }

    retour = executer_commande_simple(l->seq[0], fdEntreeCommande, fdSortieCommande);

    //Fermeture des fichiers de redirections si ils ont été créés
    if(l->in != NULL)
    {
      close(fdEntreeCommande);
    }
    if(l->out != NULL)
    {
      close(fdSortieCommande);
    }
  }
  //Si la commande contient un/des pipes
  else
  {
    retour = executer_commande_pipe(l, 0, 0);
  }

  return retour;
}

retoursTraitementCommande executer_commande_simple(char **commande, int fdIn, int fdOut)
{
  retoursTraitementCommande retour = NORMAL;

  // int i;

  //On créé un processus fils qui va exécuter la commande en question
  //Ses entrées/sorties seront celles indiquées par les arguments de la fonction
  int pidFils = Fork();

  if(pidFils == 0)
  {
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
  else
  {
    int statutFinProcessusFils;

    Waitpid(pidFils, &statutFinProcessusFils, 0);

    //Si le processus fils ne s'est pas terminé correctement
    if(WIFEXITED(statutFinProcessusFils) == 0)
    {
      //On ferme le shell
      retour = FERMETURE_SHELL;
    }
    //Si le processus fils s'est terminé correctement, on intercepte la valeur de son retour
    else
    {
      //Regarder si nos codes d'erreurs ne peuvent pas entrer en colision avec les erreurs déjà existantes
      retour = WEXITSTATUS(statutFinProcessusFils);
    }
  }

  return retour;
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

//Le paramètre pos est le numéro (position) de la commande dans la série des commandes
retoursTraitementCommande executer_commande_pipe(struct cmdline *l, int pos, int fdIn) {
  retoursTraitementCommande retour = NORMAL;
  //Si il y a une redirection de l'entrée et que la commande actuellement exécutée est la première de la série
  if(pos == 0 && l->in != NULL) {
    int i = verification_permissions_fichier(l->in);
    if(i == -1) {
      printf("%s: File not found.\n", l->in);
      return ERREUR_EXECUTION_COMMANDE;
    } else if (i < 400) {
      printf("%s: Permission denied.\n", l->in);
    } else {
      fdIn = open(l->in, O_RDONLY, 0);
    }
  }

  //Si c'est la dernière commande de la série
  if(l->seq[pos+1] == NULL) {
    int fdOut = 1;
    //Si il y a une redirection de la sortie
    if(l->out != NULL)
    {
      // Ajout d'une gestion des permissions sur redir sortie pour éviter un "bad file descriptor" (partie 5)
      int i = verification_permissions_fichier(l->out);
      if ((i >= 0 && i < 200) || (i >= 400 && i < 600)) {
        printf("%s: Permission denied.\n", l->out);
      } else {
        fdOut = open(l->out, O_WRONLY | O_CREAT, S_IRWXU);
      }
    }

    retour = executer_commande_simple(l->seq[pos], fdIn, fdOut);

    //Fermeture du pipe d'entrée
    close(fdIn);

    //Fermeture du fichier de sortie si il y a une redirection
    if(fdOut != 1)
    {
      close(fdOut);
    }

    return retour;
  } else {
    int fd[2];

    if(pipe(fd) == -1) {
      perror("Pipeline failed");
    }

    int pidFilsCommandeExecutee = Fork();

    if(pidFilsCommandeExecutee == 0)
    {
      int fdOut = fd[1];
      close(fd[0]);

      retour = executer_commande_simple(l->seq[pos], fdIn, fdOut);

      //Fermeture du pipe d'entrée
      close(fdIn);

      //Fermeture de la sortie
      close(fdOut);

      //Fin du processus
      exit(retour);
    }
    else
    {
      close(fd[1]);

      //Exécution de la prochaine commande de la série si la précédente n'a pas échoué
      int statutFinProcessusFilsCommandeExecutee;
      int aCommandeEchoue = 0;

      Waitpid(pidFilsCommandeExecutee, &statutFinProcessusFilsCommandeExecutee, 0);
      retour = WEXITSTATUS(statutFinProcessusFilsCommandeExecutee);

      //On considère qu'une commande à échoué si elle n'a pas pu être lancée ou
      //si le processus fils s'est terminé correctement mais que son retour est "COMMANDE_INTERNE_PAS_TROUVEE"
      //ou "ERREUR_EXECUTION_COMMANDE"
      if(WIFEXITED(statutFinProcessusFilsCommandeExecutee) == 0)
      {
        aCommandeEchoue = 1;
      }
      else
      {
        if(retour == COMMANDE_INTERNE_PAS_TROUVEE || retour == ERREUR_EXECUTION_COMMANDE)
        {
          aCommandeEchoue = 1;
        }
      }

      if(aCommandeEchoue == 1)
      {
        //On affiche une erreur et on ferme le shell
        printf("Erreur lors de l'exécution de la commande \"");
        afficherCommande(l->seq[pos]);
        printf("\", fermeture du MiniShell.\n");

        retour = FERMETURE_SHELL;
      }
      else
      {
        //Si le retour demande la fermeture du MiniShell alors que la commande n'est pas terminée, on prévient l'utilisateur
        //et on ferme le MiniShell.
        if(retour == FERMETURE_SHELL)
        {
          printf("L'exécution de la commande \"");
          afficherCommande(l->seq[pos]);
          printf("\" implique une fermeture du MiniShell alors qu'il reste des commandes à exécuter. Fermeture du MiniShell par sécurité.\n");
        }
        //Sinon, on exécute la prochaine commande
        else
        {
          retour = executer_commande_pipe(l, pos+1, fd[0]);
        }
      }

      close(fd[0]);
    }
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
