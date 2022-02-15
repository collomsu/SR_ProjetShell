#include "traiter_cmd.h"

retoursTraitementCommande traiter_commande(struct cmdline *l) {
  retoursTraitementCommande retour = NORMAL;

  int i;

  //Si la commande est une commande simple
  if(l->seq[1] == NULL)
  {
    //On commence par contrôler si la commande n'est pas une commande interne au Shell
    retour = traiter_commande_interne(l->seq[0]);
    
    //Si la commande n'est pas une commande interne
    if(retour == COMMANDE_INTERNE_PAS_TROUVEE)
    {
      //On créé un processus fils qui va exécuter la commande en question
      int pidFils = Fork();

      if(pidFils == 0)
      {
        retoursTraitementCommande retourLancementCommande = execvp(l->seq[0][0], l->seq[0]);

        if(retourLancementCommande == -1)
        {
          i = 0;

          printf("Erreur lors du lancement de la commande \"");
          while (l->seq[0][i] != NULL)
          {
            printf("%s", l->seq[0][i]);

            i = i + 1;
          }
          printf("\".\n");

          perror("exec");

          exit(-1);
        }
      }
      else
      {
        int statutFinProcessusFils;

        Waitpid(pidFils, &statutFinProcessusFils, 0);

        //Si le processus fils ne s'est pas terminé correctement
        if(WIFEXITED(statutFinProcessusFils) == 0)
        {
          //Si l'erreur n'est pas -1 (erreur déjà traitée) on ferme le shell
          if(WEXITSTATUS(statutFinProcessusFils) != -1)
          {
            printf("Erreur fin de processus fils inconnue, fermeture du Shell.\n");

            retour = FERMETURE_SHELL;
          }
        }
      }
    }
  }
  else
  {

  }

  return retour;
}

retoursTraitementCommande traiter_commande_interne(char **commande)
{
  retoursTraitementCommande retour = COMMANDE_INTERNE_PAS_TROUVEE;

  //Commandes de sortie du Shell
  if(strcmp(commande[0], "exit") == 0 || strcmp(commande[0], "quit") == 0)
  {
    retour = FERMETURE_SHELL;
  }

  return retour;
}