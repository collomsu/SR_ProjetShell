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
      fdEntreeCommande = open(l->in, O_RDONLY, 0);
    }

    //Si il y a une redirection de la sortie
    if(l->out != NULL)
    {
      fdSortieCommande = open(l->out, O_WRONLY | O_CREAT, S_IRWXU);
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
  else
  {

  }

  return retour;
}

retoursTraitementCommande executer_commande_simple(char **commande, int fdIn, int fdOut)
{
  retoursTraitementCommande retour = NORMAL;

  int i;

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
      int retourLancementCommande = execvp(commande[0], commande);

      //Si la commande ne s'est pas lancée correctement
      if(retourLancementCommande == -1)
      {
        i = 0;

        printf("Erreur lors du lancement de la commande \"");
        while (commande[i] != NULL)
        {
          printf("%s", commande[i]);

          i = i + 1;
        }
        printf("\".\n");

        perror("exec");

        retour = ERREUR_EXECUTION_COMMANDE;
      }
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