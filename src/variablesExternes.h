#ifndef __VARIABLES_EXTERNES_H
#define __VARIABLES_EXTERNES_H

#include "listeInt.h"

//Ce fichier contient la déclaration des variables externes (partagées entre les différentes parties de ce programme)


//Entier (utilisé comme un booléen) indiquant si une commande exécutée implique la fermeture du MiniShell.
int finShell;

//Entier (utilisé comme un booléen) indiquant si une commande est actuellement en train d'être exécutée en foreground.
int estCommandeForegroundEnCours;

//Liste (d'entiers) des PIDs de processus actuellement en exécution en foreground
listeInt* pidsCommandeForeground;

#endif
