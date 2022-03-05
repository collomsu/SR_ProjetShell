#include "fonctions_utiles.h"

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

void afficherCommande(char **commande)
{
  int i = 0;

  while (commande[i] != NULL)
  {
    printf("%s", commande[i]);

    i = i + 1;
  }

  fflush(stdout);
}

char* getChaineCommandeComplete(struct cmdline *l)
{
  char* retour = NULL;
  int longueurRetour = 0;
  int i, j;


  //Copie de la première (et potentiellement seule) commande de la série
  retour = malloc((strlen(l->seq[0][0]) + longueurRetour + 1) * sizeof(char));
  strcpy(retour, l->seq[0][0]);
  longueurRetour = longueurRetour + strlen(l->seq[0][0]);

  j = 1;
  while (l->seq[0][j] != NULL)
  {
    retour = realloc(retour, (strlen(" ") + longueurRetour + 1) * sizeof(char));
    strcat(retour, " ");
    longueurRetour = longueurRetour + strlen(" ");

    retour = realloc(retour, (strlen(l->seq[0][j]) + longueurRetour + 1) * sizeof(char));
    strcat(retour, l->seq[0][j]);
    longueurRetour = longueurRetour + strlen(l->seq[0][j]);

    j = j + 1;
  }


  //Si il y a une redirection de l'entrée,
  if(l->in != NULL) {
    //Copie du " < "
    retour = realloc(retour, (strlen(" < ") + longueurRetour + 1) * sizeof(char));
    strcat(retour, " < ");
    longueurRetour = longueurRetour + strlen(" < ");

    //Copie du nom du fichier redirigé
    retour = realloc(retour, (strlen(l->in) + longueurRetour + 1) * sizeof(char));
    strcat(retour, l->in);
    longueurRetour = longueurRetour + strlen(l->in);
  }


  //Si il y a d'autres commandes dans la série de commandes
  i = 1;
  while (l->seq[i] != NULL)
  {
    //Copie du " | "
    retour = realloc(retour, (strlen(" | ") + longueurRetour + 1) * sizeof(char));
    strcat(retour, " | ");
    longueurRetour = longueurRetour + strlen(" | ");

    //Copie de la commande
    j = 0;
    while (l->seq[i][j] != NULL)
    {
      retour = realloc(retour, (strlen(l->seq[i][j]) + longueurRetour + 1) * sizeof(char));
      strcat(retour, l->seq[i][j]);
      longueurRetour = longueurRetour + strlen(l->seq[i][j]);

      j = j + 1;
    }

    i = i + 1;
  }


  //Si il y a une redirection de la sortie
  if(l->out != NULL) {
    //Copie du " > "
    retour = realloc(retour, (strlen(" > ") + longueurRetour + 1) * sizeof(char));
    strcat(retour, " > ");
    longueurRetour = longueurRetour + strlen(" > ");

    //Copie du nom du fichier redirigé
    retour = realloc(retour, (strlen(l->out) + longueurRetour + 1) * sizeof(char));
    strcat(retour, l->out);
    longueurRetour = longueurRetour + strlen(l->out);
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

    fflush(stdout);

    perror("exec");

    fflush(stdout);

    retour = ERREUR_EXECUTION_COMMANDE;
  }
  return retour;
}
