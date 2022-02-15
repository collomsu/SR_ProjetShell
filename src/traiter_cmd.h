#ifndef __TRAITER_CMD_H
#define __TRAITER_CMD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "readcmd.h"

//Enum utilisée pour gérer le retour du traitement de commandes
typedef enum {
    NORMAL,
    //Valeur indiquant que le Shell doit être fermé
    FERMETURE_SHELL
} retoursTraitementCommande;

retoursTraitementCommande traiter_commande(struct cmdline *l);

#endif
