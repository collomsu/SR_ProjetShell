#include "readcmd.h"

int traiter_commande(cmdline *l) {
  if(strcmp(l->seq[0][0]), "exit")
    return 1;
  else
    return 0;
}
