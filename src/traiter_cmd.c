#include "traiter_cmd.h"

int traiter_commande(struct cmdline *l) {
  if(strcmp(l->seq[0][0], "exit") == 0) {
    return 1;
  } else {
    return 0;
  }
}
