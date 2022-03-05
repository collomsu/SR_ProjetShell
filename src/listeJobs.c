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
   if(ElementADetruire->commandeLancementJob != NULL)
   {
      free(ElementADetruire->commandeLancementJob);
   }

   free(ElementADetruire);
   ElementADetruire = NULL;
}

void AjouterElementListeJobs(listeJobs *laListe, char *commandeLancementJob, etatsJobs etatJob)
  {
  struct elementListeJobs *elementInsere = NouvelElementListeJobs();

  //Copie de la chaine de caractères de la commande
  elementInsere->commandeLancementJob = malloc((strlen(commandeLancementJob) + 1) * sizeof(char));
  strcpy(elementInsere->commandeLancementJob, commandeLancementJob);

  elementInsere->etatJob = etatJob;
  if(laListe->tete == NULL) {
    elementInsere->numeroJob = 1;
    laListe->tete = elementInsere;
  } else {
    struct elementListeJobs *element = laListe->tete;
    elementInsere->numeroJob = numeroDernierJob(laListe) + 1;
    while(element->suivant != NULL){
      element = element->suivant;
    }
    element->suivant = elementInsere;
  }
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
      int indexElementParcouruSuivant = laListe->tete->numeroJob;
      //Suppression de la tête si le job à supprimé est le numéro 1
      if(numeroJob == indexElementParcouruSuivant)
      {
         ElementADetruire = laListe->tete;
         laListe->tete = laListe->tete->suivant;
         retour = 0;
      }
      else
      {
         struct elementListeJobs *elementParcoursListe = laListe->tete;

         while(retour == -1 && ElementADetruire->suivant != NULL)
         {
           indexElementParcouruSuivant = elementParcoursListe->numeroJob;
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
            indexElementParcouruSuivant++;
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
  if(laListe->tete == NULL) {
    numero = 0;
  } else {
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
        break;
      } else {
        element = element->suivant;
      }
    }
  }
  return element;
}
