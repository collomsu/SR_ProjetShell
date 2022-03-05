#ifndef __FONCTIONS_UTILES_H
#define __FONCTIONS_UTILES_H

#include "readcmd.h"
#include "traiter_cmd.h"

int verification_permissions_fichier(char* fichier);

//Fonction permettant d'afficher à l'écran une commande passée en paramètre
void afficherCommande(char **commande);

//Fonction permettant d'avoir la chaine de caractères correspondant à la commande tapée par l'utilisateur dans
//le Shell à partir d'un struct cmdline
//Une chaine de caractères est générée à chaque appel de fonction, elle doit donc être libérée par la suite
char* getChaineCommandeComplete(struct cmdline *l);


retoursTraitementCommande execvp_correct(int retourLancementCommande, char **commande);

#endif
