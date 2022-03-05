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

void AjouterElementListeJobs(listeJobs *laListe, int numeroJob, char* commandeLancementJob, etatsJobs etatJob)
{
   struct elementListeJobs* elementInsere = NouvelElementListeJobs();

   elementInsere->numeroJob = numeroJob;

   //Copie de la chaine de caractères de la commande
   elementInsere->commandeLancementJob = malloc((strlen(commandeLancementJob) + 1) * sizeof(char));
   strcpy(elementInsere->commandeLancementJob, commandeLancementJob);

   elementInsere->etatJob = etatJob;

   if(laListe->tete != NULL)
   {
      elementInsere->suivant = laListe->tete;
   }

   laListe->tete = elementInsere;
}

int ModifierEtatElementListeJobs(listeJobs *laListe, int numeroJob, etatsJobs nouvelEtatJob)
{
   int retour = -1;

   if(!EstListeJobsVide(laListe))
   {
      struct elementListeJobs *ElementAModifier = NULL;

      if(laListe->tete->numeroJob == numeroJob)
      {
         retour = 0;
      }
      else
      {
         struct elementListeJobs *elementParcoursListe = laListe->tete;
         
         while(retour == -1 && elementParcoursListe->suivant != NULL)
         {
            //Si on a trouvé notre élément
            if(elementParcoursListe->suivant->numeroJob == numeroJob)
            {
               ElementAModifier = elementParcoursListe->suivant;
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
      struct elementListeJobs *ElementADetruire = NULL;

      if(laListe->tete->numeroJob == numeroJob)
      {
         ElementADetruire = laListe->tete;
         laListe->tete = laListe->tete->suivant;
         retour = 0;
      }
      else
      {
         struct elementListeJobs *elementParcoursListe = laListe->tete;
         
         while(retour == -1 && elementParcoursListe->suivant != NULL)
         {
            //Si on a trouvé notre élément
            if(elementParcoursListe->suivant->numeroJob == numeroJob)
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
      struct elementListeJobs *ElementCherche = NULL;

      if(laListe->tete->numeroJob == numeroJob)
      {
         retour = 0;
      }
      else
      {
         struct elementListeJobs *elementParcoursListe = laListe->tete;
         
         while(retour == -1 && elementParcoursListe->suivant != NULL)
         {
            //Si on a trouvé notre élément
            if(elementParcoursListe->suivant->numeroJob == numeroJob)
            {
               ElementCherche = elementParcoursListe->suivant;
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
         pointeurCommandeLancement = ElementCherche->commandeLancementJob;
      }
   }

   return retour;
}