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
#include "variablesExternes.h"

//Enum utilisée pour gérer le retour du traitement de commandes
typedef enum {
    NORMAL,
    ATTENDRE_FIN_PROCESSUS_FOREGROUND,
    COMMANDE_INTERNE_PAS_TROUVEE,
    ERREUR_EXECUTION_COMMANDE,
    //Valeur indiquant que le Shell doit être fermé
    FERMETURE_SHELL
} retoursTraitementCommande;

int verification_permissions_fichier(char* fichier);
retoursTraitementCommande traiter_commande(struct cmdline *l);
retoursTraitementCommande executer_commande_simple(char **commande, int fdIn, int fdOut);
retoursTraitementCommande executer_commande_interne(char **commande);
retoursTraitementCommande executer_commande_pipe(struct cmdline *l);
retoursTraitementCommande execvp_correct(int retourLancementCommande, char **commande);

//Fonction permettant d'afficher à l'écran une commande passée en paramètre
void afficherCommande(char **commande);

#endif
