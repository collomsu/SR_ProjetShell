#ifndef __LISTE_INT_H
#define __LISTE_INT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct elementListeInt{
    int valeur;
    struct elementListeInt *suivant;
};
typedef struct {
    struct elementListeInt *tete
} listeInt;

listeInt* nouvelleListeCirculaireInt();

void detruireListeCirculaireInt(listeInt *liste);

void remiseAZeroListeCirculaireInt(listeInt *liste);

void insererListeCirculaireInt(listeInt *liste, int valeur);

//Retourne 0 si l'élément a bien été supprimé de la liste
int supprimerElementListeCirculaireInt(listeInt *liste, int valeur);

#endif
