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
    struct elementListeInt *tete;
} listeInt;

listeInt* NouvelleListeInt(void);

struct elementListeInt* NouvelElementListeInt(void);

void DetruireListeInt(listeInt* ListeADetruire);

void DetruireElementListeIntEtSaDescendance(struct elementListeInt* ElementListeIntADetruire);

void DetruireElementListeInt(struct elementListeInt *ElementADetruire);

void AjouterElementListeInt(listeInt *laListe, int valeur);

//Retourne 0 si l'élément a bien été supprimé de la liste
//Si l'élément cherché n'était pas dans la liste, -1 est retourné
int SupprimerElementListeInt(listeInt *laListe, int valeur);

//Retourne 0 si la liste n'est pas vide
int EstListeIntVide(listeInt *laListe);

#endif
