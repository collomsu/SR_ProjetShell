#ifndef __LISTE_JOBS_H
#define __LISTE_JOBS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "listeInt.h"

typedef enum {
    RUNNING,
    STOPPED,
    NON_DEFINI
} etatsJobs;

struct elementListeJobs{
    int numeroJob;
    listeInt *listePIDsJob;
    char* commandeLancementJob;
    etatsJobs etatJob;
    struct elementListeJobs *suivant;
};
typedef struct {
    struct elementListeJobs *tete;
} listeJobs;

//Retourne le pointeur créer lors de l'allocation en mémoire d'une nouvelle liste de jobs
listeJobs* NouvelleListeJobs(void);

//Retourne le pointeur créer lors de l'allocation en mémoire d'un nouvel élément de type elementListeJobs
struct elementListeJobs* NouvelElementListeJobs(void);

//Libère l'espace mémoire de la liste de job entière
void DetruireListeJobs(listeJobs* ListeADetruire);

//Libère l'espace mémoiree alloué par le pointeur de type struct elementListeJobs passé en paramètre,
// ainsi que celui de tout ses suivants
void DetruireElementListeJobsEtSaDescendance(struct elementListeJobs* elementListeJobsADetruire);

//Libère l'espace mémoire alloué par le pointeur de type struct elementListeJobs passé en paramètre
void DetruireElementListeJobs(struct elementListeJobs *ElementADetruire);

//Ajout un un nouveau job dans la liste des jobs avec la commande et l'état donné en paramètre
//Un numéro de job lui est attribué et est retourné (-1 si erreur lors de l'ajout)
int AjouterElementListeJobs(listeJobs *laListe, char* commandeLancementJob, etatsJobs etatJob);

//Retourne 0 si l'état de l'élément a bien été modifié
//Si l'élément à modifier n'était pas dans la liste, -1 est retourné
int ModifierEtatElementListeJobs(listeJobs *laListe, int numeroJob, etatsJobs nouvelEtatJob);

//Retourne 0 si l'élément a bien été supprimé de la liste
//Si l'élément à supprimer n'était pas dans la liste, -1 est retourné
int SupprimerElementListeJobs(listeJobs *laListe, int numeroJob);

//Retourne 0 si la liste n'est pas vide
int EstListeJobsVide(listeJobs *laListe);

//!!!! Le pointeur vers la chaine de caractères retourné n'est pas le poiteur vers une nouvelle chaine, c'est une simple copie
//     ->Ne pas modifier la chaine de caractères sous peine de générer des erreurs de cohérence
//Le pointeur cherché est retourné via pointeurCommandeLancement
//Retourne 0 si l'élément dont la commande était cherchée été trouvé dans la liste, sinon -1
int getPointeurCommandeLancementJobByNumeroJob(listeJobs *laListe, int numeroJob, char* pointeurCommandeLancement);

//Retourne le numéro de dernier job créer dans la liste
int numeroDernierJob(listeJobs *laListe);

//Retourne le pointeur vers l'élément correspondant au numéro de job donné
//Si aucun element ne correspond au numéro de job donnée on retourne NULL
struct elementListeJobs* GetElementListeJobsByNumero(listeJobs *laListe, int numeroJobCommande);

#endif
