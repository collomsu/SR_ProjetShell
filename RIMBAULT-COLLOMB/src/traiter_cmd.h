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
    COMMANDE_INTERNE_PAS_TROUVEE,
    ERREUR_EXECUTION_COMMANDE,
    ERREUR_REDIRECTION_FICHIER,
    //Valeur indiquant que le Shell doit être fermé
    FERMETURE_SHELL,

    //On a choisi de mettre en place l'astuce suivante pour connaitre le numéro du job devant être repris en frontground
    //->Si la commande appelée est fg, la valeur de son retour (qui est l'exit du processus de la commande) est:
    //  REPRISE_JOB_FOREGROUND + numéro job à reprendre
    REPRISE_JOB_FOREGROUND
    //Il ne peut donc pas y avoir de retours > REPRISE_JOB_FOREGROUND, c'est le dernier de la liste
} retoursTraitementCommande;

retoursTraitementCommande traiter_commande(struct cmdline *l);
retoursTraitementCommande executer_commande_simple(char **commande, int fdIn, int fdOut);
retoursTraitementCommande executer_commande_interne(char **commande);
retoursTraitementCommande executer_commande_pipe(struct cmdline *l);

int estCommande_interne(char *commande);


#endif
