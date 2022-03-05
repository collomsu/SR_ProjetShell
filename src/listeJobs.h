#ifndef __LISTE_JOBS_H
#define __LISTE_JOBS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef enum {
    RUNNING,
    STOPPED,
    NON_DEFINI
} etatsJobs;

struct elementListeJobs{
    int numeroJob;
    char* commandeLancementJob;
    etatsJobs etatJob;
    struct elementListeJobs *suivant;
};
typedef struct {
    struct elementListeJobs *tete;
} listeJobs;

listeJobs* NouvelleListeJobs(void);

struct elementListeJobs* NouvelElementListeJobs(void);

void DetruireListeJobs(listeJobs* ListeADetruire);

void DetruireElementListeJobsEtSaDescendance(struct elementListeJobs* elementListeJobsADetruire);

void DetruireElementListeJobs(struct elementListeJobs *ElementADetruire);

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

int numeroDernierJob(listeJobs *laListe);

struct elementListeJobs* GetElementListeJobsByNumero(listeJobs *laListe, int numeroJobCommande);

#endif
