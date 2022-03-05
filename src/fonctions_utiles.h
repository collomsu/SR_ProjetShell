#ifndef __FONCTIONS_UTILES_H
#define __FONCTIONS_UTILES_H

int verification_permissions_fichier(char* fichier);
void afficherCommande(char **commande);
char* getChaineCommandeComplete(struct cmdline *l);
retoursTraitementCommande execvp_correct(int retourLancementCommande, char **commande);

#endif
