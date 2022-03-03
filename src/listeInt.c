#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 

#include "listeInt.h"

listeInt* NouvelleListeInt(void)
{
   listeInt* nouvelleListe = malloc(sizeof(listeInt));

   nouvelleListe->tete = NULL;
   
   return nouvelleListe;
}

struct elementListeInt* NouvelElementListeInt(void)
{
   struct elementListeInt* nouvelElement = malloc(sizeof(struct elementListeInt));

   nouvelElement->valeur = 0;
   nouvelElement->suivant = NULL;
   
   return nouvelElement;
}

void DetruireListeInt(listeInt* ListeADetruire)
{
    if(ListeADetruire != NULL)
    {
         if(ListeADetruire->tete != NULL)
         {
            DetruireElementListeIntEtSaDescendance(ListeADetruire->tete);
         }

         free(ListeADetruire);
         ListeADetruire = NULL;
    }
}

void DetruireElementListeIntEtSaDescendance(struct elementListeInt* ElementListeIntADetruire)
{
   if(ElementListeIntADetruire != NULL)
   {
      if(ElementListeIntADetruire->suivant != NULL)
      {
         DetruireElementListeIntEtSaDescendance(ElementListeIntADetruire->suivant);
      }

      DetruireElementListeInt(ElementListeIntADetruire);
      ElementListeIntADetruire = NULL;
   }
}

void DetruireElementListeInt(struct elementListeInt *ElementADetruire)
{
   free(ElementADetruire);
   ElementADetruire = NULL;
}

void AjouterElementListeInt(listeInt *laListe, int valeur)
{
   struct elementListeInt* elementInsere = NouvelElementListeInt();
   elementInsere->valeur = valeur;

   if(laListe->tete != NULL)
   {
      elementInsere->suivant = laListe->tete;
   }

   laListe->tete = elementInsere;
}

int SupprimerElementListeInt(listeInt *laListe, int valeur)
{
   int retour = -1;

   if(!EstListeIntVide(laListe))
   {
      struct elementListeInt *ElementADetruire = NULL;

      if(laListe->tete->valeur == valeur)
      {
         ElementADetruire = laListe->tete;
         laListe->tete = laListe->tete->suivant;
         DetruireElementListeInt(ElementADetruire);
         retour = 0;
      }
      else
      {
         struct elementListeInt *elementParcoursListe = laListe->tete;
         
         while(retour == -1 && elementParcoursListe->suivant != NULL)
         {
            //Si on a trouvé notre variable
            if(laListe->tete->suivant->valeur == valeur)
            {
               ElementADetruire = elementParcoursListe->suivant;
               elementParcoursListe->suivant = elementParcoursListe->suivant->suivant;
               DetruireElementListeInt(ElementADetruire);
               retour = 0;
            }
            else
            {
               elementParcoursListe = elementParcoursListe->suivant;
            }
         }
      }
   }

   return retour;
}

int EstListeIntVide(listeInt *laListe)
{
   return (laListe->tete == NULL);
}