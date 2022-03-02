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