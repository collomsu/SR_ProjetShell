#ifndef __TRAITER_CMD_H
#define __TRAITER_CMD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "errno.h"
#include "readcmd.h"
#include "csapp.h"

//Enum utilisée pour gérer le retour du traitement de commandes
typedef enum {
    NORMAL,
    COMMANDE_INTERNE_PAS_TROUVEE,
    //Valeur indiquant que le Shell doit être fermé
    FERMETURE_SHELL
} retoursTraitementCommande;

retoursTraitementCommande traiter_commande(struct cmdline *l);
retoursTraitementCommande traiter_commande_interne(char **commande);

#endif
