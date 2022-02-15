#include "traiter_cmd.h"

int traiter_commande(struct cmdline *l) {
  if(strcmp(l->seq[0][0], "exit")) {
    return 1;
  } else {
    return 0;
  }
}
