#include "traiter_cmd.h"

retoursTraitementCommande traiter_commande(struct cmdline *l) {
  retoursTraitementCommande retour = NORMAL;

  //Commandes de sortie du Shell
  if(strcmp(l->seq[0][0], "exit") == 0 || strcmp(l->seq[0][0], "quit") == 0)
  {
    retour = FERMETURE_SHELL;
  }
  else
  {
    
  }

  return retour;
}
