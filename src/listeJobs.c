#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "listeJobs.h"

listeJobs* NouvelleListeJobs(void)
{
   listeJobs* nouvelleListe = malloc(sizeof(listeJobs));

   nouvelleListe->tete = NULL;

   return nouvelleListe;
}

struct elementListeJobs* NouvelElementListeJobs(void)
{
   struct elementListeJobs* nouvelElement = malloc(sizeof(struct elementListeJobs));
   nouvelElement->numeroJob = 0;
   nouvelElement->listePIDsJob = NULL;
   nouvelElement->commandeLancementJob = NULL;
   nouvelElement->etatJob = NON_DEFINI;
   nouvelElement->suivant = NULL;

   return nouvelElement;
}

void DetruireListeJobs(listeJobs* ListeADetruire)
{
    if(ListeADetruire != NULL)
    {
         if(ListeADetruire->tete != NULL)
         {
            DetruireElementListeJobsEtSaDescendance(ListeADetruire->tete);
         }

         free(ListeADetruire);
         ListeADetruire = NULL;
    }
}

void DetruireElementListeJobsEtSaDescendance(struct elementListeJobs* ElementListeJobsADetruire)
{
   if(ElementListeJobsADetruire != NULL)
   {
      if(ElementListeJobsADetruire->suivant != NULL)
      {
         DetruireElementListeJobsEtSaDescendance(ElementListeJobsADetruire->suivant);
      }

      DetruireElementListeJobs(ElementListeJobsADetruire);
      ElementListeJobsADetruire = NULL;
   }
}

void DetruireElementListeJobs(struct elementListeJobs *ElementADetruire)
{
   if(ElementADetruire->listePIDsJob != NULL)
   {
      DetruireListeInt(ElementADetruire->listePIDsJob);
   }

   if(ElementADetruire->commandeLancementJob != NULL)
   {
      free(ElementADetruire->commandeLancementJob);
   }

   free(ElementADetruire);
   ElementADetruire = NULL;
}

int AjouterElementListeJobs(listeJobs *laListe, char *commandeLancementJob, etatsJobs etatJob)
{
  struct elementListeJobs *elementInsere = NouvelElementListeJobs();


  int numeroJobAJoute = -1;

  //Si aucun élément dans la liste, on ajoute en tete avece comme numéro de job le numéro 1
  if(laListe->tete == NULL) {
    elementInsere->numeroJob = 1;
    laListe->tete = elementInsere;

  //Sinon on ajoute en queue avec comme numero de job le numéro du dernier job + 1
  } else {
    struct elementListeJobs *element = laListe->tete;
    elementInsere->numeroJob = numeroDernierJob(laListe) + 1;
    while(element->suivant != NULL){
      element = element->suivant;
    }
    element->suivant = elementInsere;
  }

  //Création d'une liste de processus vide
  elementInsere->listePIDsJob = NouvelleListeInt();

  //Copie de la chaine de caractères de la commande
  elementInsere->commandeLancementJob = malloc((strlen(commandeLancementJob) + 1) * sizeof(char));
  strcpy(elementInsere->commandeLancementJob, commandeLancementJob);

  elementInsere->etatJob = etatJob;

  
  numeroJobAJoute = elementInsere->numeroJob;
  return numeroJobAJoute;
}

int ModifierEtatElementListeJobs(listeJobs *laListe, int numeroJob, etatsJobs nouvelEtatJob)
{
  int retour = -1;

  if(!EstListeJobsVide(laListe))
  {
    struct elementListeJobs *ElementAModifier = laListe->tete;

    int indexElementParcouru = 1;

    while(retour == -1 && ElementAModifier != NULL)
    {
      //Si on a trouvé notre élément
      if(indexElementParcouru == numeroJob)
      {
        retour = 0;
      }
      else
      {
        ElementAModifier = ElementAModifier->suivant;
        indexElementParcouru = indexElementParcouru + 1;
      }
    }

    if(retour == 0)
    {
      ElementAModifier->etatJob = nouvelEtatJob;
    }
  }

  return retour;
}

int SupprimerElementListeJobs(listeJobs *laListe, int numeroJob)
{
   int retour = -1;

   if(!EstListeJobsVide(laListe))
   {
      struct elementListeJobs *ElementADetruire = laListe->tete;
      //Suppression de la tête si le job à supprimé est égal au numéro de la tête
      if(laListe->tete->numeroJob == numeroJob)
      {
         ElementADetruire = laListe->tete;
         laListe->tete = laListe->tete->suivant;
         retour = 0;
      }
      //Sinon on cherche dans le reste de la liste l'élément correspondant au numéro de job à supprimer
      else
      {
         struct elementListeJobs *elementParcoursListe = laListe->tete;
         int indexElementParcouruSuivant;

         while(retour == -1 && ElementADetruire->suivant != NULL)
         {
            indexElementParcouruSuivant = elementParcoursListe->suivant->numeroJob;

            //Si on a trouvé notre élément
            if(indexElementParcouruSuivant == numeroJob)
            {
               ElementADetruire = elementParcoursListe->suivant;
               elementParcoursListe->suivant = elementParcoursListe->suivant->suivant;
               retour = 0;
            }
            else
            {
               elementParcoursListe = elementParcoursListe->suivant;
            }
         }
      }

      if(retour == 0)
      {
         DetruireElementListeJobs(ElementADetruire);
      }
   }

   return retour;
}

int EstListeJobsVide(listeJobs *laListe)
{
   return (laListe->tete == NULL);
}

int getPointeurCommandeLancementJobByNumeroJob(listeJobs *laListe, int numeroJob, char* pointeurCommandeLancement)
{
   int retour = -1;

   if(!EstListeJobsVide(laListe))
   {
      struct elementListeJobs *ElementCherche = laListe->tete;

      int indexElementParcouru = ElementCherche->numeroJob;

      while(retour == -1 && ElementCherche != NULL)
      {
         indexElementParcouru = ElementCherche->numeroJob;

         //Si on a trouvé notre élément
         if(indexElementParcouru == numeroJob)
         {
            retour = 0;
         }
         else
         {
            ElementCherche = ElementCherche->suivant;
         }
      }

      if(retour == 0)
      {
         pointeurCommandeLancement = ElementCherche->commandeLancementJob;
      }
   }

   return retour;
}

int numeroDernierJob(listeJobs *laListe) {
  struct elementListeJobs *suivant;
  int numero;
  //Si aucun élément dans la liste on renvoit 0
  if(laListe->tete == NULL) {
    numero = 0;
  //Sinon on cherche le numéro du dernier job de la liste
  } else {
    suivant = laListe->tete;
    while(suivant != NULL) {
      numero = suivant->numeroJob;
      suivant = suivant->suivant;
    }
  }

  return numero;
}

struct elementListeJobs* GetElementListeJobsByNumero(listeJobs *laListe, int numeroJobCommande) {
  struct elementListeJobs* element = NULL;
  if(!EstListeJobsVide(laListe)) {
    element = laListe->tete;
    while(element != NULL) {
      if(element->numeroJob == numeroJobCommande) {
        //Si l'élément a été trouvé on sort de la boucle
        break;
      } else {
        element = element->suivant;
      }
    }
  }
  
  return element;
}
