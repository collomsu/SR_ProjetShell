#ifndef __VARIABLES_EXTERNES_H
#define __VARIABLES_EXTERNES_H

#include "listeInt.h"
#include "listeJobs.h"

//Ce fichier contient la déclaration des variables externes (partagées entre les différentes parties de ce programme).


//Entier (utilisé comme un booléen) indiquant si une commande exécutée implique la fermeture du MiniShell.
int finShell;

//PID du processus actuellement en exécution en foreground.
//Vaut -1 si aucun processus n'est actuellement en foreground
int numJobCommandeForeground;

//Liste des jobs de notre MiniShell (composé d'un numéro, de la commande, un état, et d'un pointeur vers le prochain élément).
listeJobs* listeJobsShell;

//Structure de données permettant de désigner un ou plusieurs types de signaux.
sigset_t structureDeSignaux;

#endif
