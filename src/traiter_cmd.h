#ifndef __TRAITER_CMD_H
#define __TRAITER_CMD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "errno.h"
#include "readcmd.h"
#include "csapp.h"
#include "listeInt.h"
#include "listeJobs.h"
#include "variablesExternes.h"

//Enum utilisée pour gérer le retour du traitement de commandes
typedef enum {
    NORMAL,
    ATTENDRE_FIN_PROCESSUS_FOREGROUND,
    COMMANDE_INTERNE_PAS_TROUVEE,
    ERREUR_EXECUTION_COMMANDE,
    ERREUR_REDIRECTION_FICHIER,
    //Valeur indiquant que le Shell doit être fermé
    FERMETURE_SHELL
} retoursTraitementCommande;

retoursTraitementCommande traiter_commande(struct cmdline *l, int numeroJobCommande);
retoursTraitementCommande executer_commande_simple(char **commande, int fdIn, int fdOut);
retoursTraitementCommande executer_commande_interne(char **commande);
retoursTraitementCommande executer_commande_pipe(struct cmdline *l, int numeroJobCommande);


#endif
