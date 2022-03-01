#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 

#include "listeInt.h"

ListeVariables* NouvelleListeVariables(void)
{
   ListeVariables* nouvelleListe = malloc(sizeof(ListeVariables));

   nouvelleListe->tete = NULL;
   
   return nouvelleListe;
}

ElementListeVariables* NouvelElementListeVariables(void)
{
   ElementListeVariables* nouvelElement = malloc(sizeof(ElementListeVariables));

   nouvelElement->nom = NULL;
   nouvelElement->typeVariable = TYPE_VARIABLE_NON_DEFINI;
   nouvelElement->variableInt = NULL;
   nouvelElement->variableDouble = NULL;
   nouvelElement->variableString = NULL;
   nouvelElement->suivant = NULL;
   
   return nouvelElement;
}


ElementListeVariablesInt* NouvelElementListeVariablesInt(void)
{
   ElementListeVariablesInt* nouvelElement = malloc(sizeof(ElementListeVariablesInt));

   nouvelElement->pasDeValeur = 1;
   
   return nouvelElement;
}
ElementListeVariablesDouble* NouvelElementListeVariablesDouble(void)
{
   ElementListeVariablesDouble* nouvelElement = malloc(sizeof(ElementListeVariablesDouble));

   nouvelElement->pasDeValeur = 1;
   
   return nouvelElement;
}
ElementListeVariablesString* NouvelElementListeVariablesString(void)
{
   ElementListeVariablesString* nouvelElement = malloc(sizeof(ElementListeVariablesString));

   nouvelElement->pasDeValeur = 1;
   nouvelElement->adresseChaine = NULL;
   
   return nouvelElement;
}


//Fait pointer le de pointeur pointeur passé en paramètre vers un pointeur pointant une variable d'une liste dont le nom est passé en paramètre
//Une erreur est retournée si la liste est nulle ou si la variable n'est pas trouvée
ERREUR_LISTE_VARIABLES GetPointeurVariableByNom(ListeVariables* laListe, char* nomVariable, ElementListeVariables** pointeurVariable)
{
   ERREUR_LISTE_VARIABLES retour = VARIABLE_PAS_TROUVEE;

   if(laListe != NULL)
   {
      ElementListeVariables* pointeurParcoursListe = laListe->tete;
      
      while(retour == VARIABLE_PAS_TROUVEE && pointeurParcoursListe != NULL)
      {
         //Si on a trouvé notre variable
         if(strcmp(nomVariable, pointeurParcoursListe->nom) == 0)
         {
            *pointeurVariable = pointeurParcoursListe;
            retour = PAS_D_ERREUR_LISTE_VARIABLES;
         }
         else
         {
            pointeurParcoursListe = pointeurParcoursListe->suivant;
         }
      }
   }
   else
   {
      retour = LISTE_VARIABLES_NULLE;
   }

   return retour;
}

//Fonction retournant le type et la valeur d'une variable dont le nom est passé en paramètre
//Une erreur est retournée si la variable n'est pas trouvée
//valeurNulle == 1 si la variable est nulle (seulement pour les string)
//Si l'on a pas encore affecté de valeur à la variable (si elle est juste déclarée), pasDeValeurAffectee == 1
ERREUR_LISTE_VARIABLES GetTypeEtValeurVariableListeVariables(
  ListeVariables* laListe, char* nomVariable, TypesVariables* typeVariable, int* pasDeValeurAffectee, int* valeurNulle,
  int* valeurInt, double* valeurDouble, char** pointeurChaine
)
{
   //On contrôle si la variable existe, si elle n'existe pas on retourne une erreur
   ElementListeVariables *pointeurVariableRecherchee;
   
   ERREUR_LISTE_VARIABLES retour = GetPointeurVariableByNom(laListe, nomVariable, &pointeurVariableRecherchee);
   
   //Si la variable existe, c'est ok
   if(retour == PAS_D_ERREUR_LISTE_VARIABLES)
   {
      *typeVariable = pointeurVariableRecherchee->typeVariable;
      *pasDeValeurAffectee = 0;
      *valeurNulle = 0;

      if(*typeVariable == INT)
      {
         if(pointeurVariableRecherchee->variableInt != NULL)
         {
            *pasDeValeurAffectee = pointeurVariableRecherchee->variableInt->pasDeValeur;
            *valeurInt = pointeurVariableRecherchee->variableInt->valeur;
         }
         else
         {
            retour = POINTEUR_VALEUR_VARIABLE_NULL;
            printf("Erreur, le pointeur vers la valeur de la variable (ElementListeVariablesInt* variableInt) est nul.\n");
         }
      }
      else
      {
         if(*typeVariable == DOUBLE)
         {
            if(pointeurVariableRecherchee->variableDouble != NULL)
            {
               *pasDeValeurAffectee = pointeurVariableRecherchee->variableDouble->pasDeValeur;
               *valeurDouble = pointeurVariableRecherchee->variableDouble->valeur;
            }
            else
            {
               retour = POINTEUR_VALEUR_VARIABLE_NULL;
               printf("Erreur, le pointeur vers la valeur de la variable (ElementListeVariablesDouble* variableDouble) est nul.\n");
            }
         }
         else
         {
            if(*typeVariable == STRING)
            {
               if(pointeurVariableRecherchee->variableString != NULL)
               {
                  if(*pointeurChaine != NULL && strlen(*pointeurChaine) > 0)
                  {
                     free(*pointeurChaine);
                     *pointeurChaine = NULL;
                  }

                  *pasDeValeurAffectee = pointeurVariableRecherchee->variableString->pasDeValeur;
                  
                  if(pointeurVariableRecherchee->variableString->pasDeValeur == 0)
                  {
                     if(pointeurVariableRecherchee->variableString->adresseChaine == NULL)
                     {
                        *valeurNulle = 1;
                     }
                     else
                     {
                        *valeurNulle = 0;
                        *pointeurChaine = malloc((strlen(pointeurVariableRecherchee->variableString->adresseChaine) + 1) * sizeof(char));
                        strcpy(*pointeurChaine, pointeurVariableRecherchee->variableString->adresseChaine);
                     }
                  }
               }
               else
               {
                  retour = POINTEUR_VALEUR_VARIABLE_NULL;
                  printf("Erreur, le pointeur vers la valeur de la variable (ElementListeVariablesString* variableString) est nul.\n");
               }
            }
            else
            {
               if(*typeVariable == TYPE_VARIABLE_NON_DEFINI)
               {
                  retour = TYPE_VARIABLE_NON_DEFINI;
                  printf("Erreur, le type de la variable dont on veut les informations n'est pas défini dans la liste");
               }
               else
               {
                  retour = TYPE_VARIABLE_INCONNU;
                  printf("Erreur, le type de la variable est inconnu.\n");
               }
            }
         }
      }
   }

   return retour;
}

//Fonction retournant le type d'une variable dont le nom est passé en paramètre
//Une erreur est retournée si la variable n'est pas trouvée
ERREUR_LISTE_VARIABLES GetTypeVariableListeVariables(
  ListeVariables* laListe, char* nomVariable, TypesVariables* typeVariable
)
{
   int valeurNulle, pasDevaleur, valeurInt;
   double valeurDouble;
   char* chaine = NULL;
   
   ERREUR_LISTE_VARIABLES retour = GetTypeEtValeurVariableListeVariables(
      laListe, nomVariable, typeVariable,
      &pasDevaleur, &valeurNulle, &valeurInt, &valeurDouble, &chaine
   );
   

   if(chaine != NULL && strlen(chaine) > 0)
   {
      free(chaine);
      chaine = NULL;
   }
   

   return retour;
}


//Fonction d'ajout d'une variable à la liste/de changement de valeur d'une variable de la liste
//La valeur de la variable ne peut être nulle que si cette dernière est de type string
ERREUR_LISTE_VARIABLES DeclarerOuChangerValeurVariableListeVariables(
   ListeVariables* laListe,
   char* nomVariable, TypesVariables type, int declaration, int valeurNulle, int pasDeValeur,
   int valeurInt, double valeurDouble, char* chaine
)
{
   ERREUR_LISTE_VARIABLES retour = PAS_D_ERREUR_LISTE_VARIABLES;

   //Il faut que le type soit valide
   if(type != INT && type != DOUBLE && type != STRING)
   {
      printf("Problème avec le type de variable indiqué.\n");
      retour = PROBLEME_PARAMETRES;
   }
   else
   {
      //Si la fonction est apellée pour une déclaration de variable (avec assignation de valeur ou non), on
      //part du postulat qu'un controle a déjà été fait (qu'aucune variable du même non n'a été déclarée) et
      //on ajoute un élément en tête de liste
      if(declaration == 1)
      {
         ElementListeVariables* nouvelleVariable = NouvelElementListeVariables();


         //On copie la chaine du nom de la variable depuis le pointeur passé en paramètre (pour ne pas avoir de
         //problème si la chaine à l'adresse du paramètre nom est modifiée/supprimée)
         nouvelleVariable->nom = malloc((strlen(nomVariable) + 1) * sizeof(char));
         strcpy (nouvelleVariable->nom, nomVariable);

         //On donne un type à notre variable
         nouvelleVariable->typeVariable = type;

         //On associe à l'élément créé le pointeur vers la valeur
         if(type == INT)
         {
            nouvelleVariable->variableInt = NouvelElementListeVariablesInt();
         }
         else
         {
            if(type == DOUBLE)
            {
               nouvelleVariable->variableDouble = NouvelElementListeVariablesDouble();
            }
            else
            {
               if(type == STRING)
               {
                  nouvelleVariable->variableString = NouvelElementListeVariablesString();
               }
            }
         }

         nouvelleVariable->suivant = laListe->tete;
         laListe->tete = nouvelleVariable;
      }



      //Le code ci-dessous indique comment on associe une valeur à notre variable
      //On commence par rechercher notre variable dans la liste
      ElementListeVariables *variableAModifier;

      retour = GetPointeurVariableByNom(laListe, nomVariable, &variableAModifier);

      //Si il n'y a pas d'erreur, on a trouvé la variable. On modifie alors sa valeur
      if(retour == PAS_D_ERREUR_LISTE_VARIABLES)
      {
         //Si une valeur est affectée
         if(pasDeValeur == 0)
         {
            //Si la valeur affectée est nulle est que ne type n'est pas string, une erreur est renvoyée
            if(valeurNulle == 1)
            {
               if(type != STRING)
               {
                  printf("Problème, la valeur nulle est affectée à une variable dont le type n'est pas string.\n");
                  retour = PROBLEME_PARAMETRES;
               }
               else
               {
                  variableAModifier->variableString->pasDeValeur = 0;

                  if(variableAModifier->variableString->adresseChaine != NULL && strlen(variableAModifier->variableString->adresseChaine) > 0)
                  {
                     free(variableAModifier->variableString->adresseChaine);
                  }
                  
                  variableAModifier->variableString->adresseChaine = NULL;
               }
            }
            //Si la valeur affectée n'est pas nulle
            else
            {
               if(type == INT)
               {
                  variableAModifier->variableInt->pasDeValeur = 0;
                  variableAModifier->variableInt->valeur = valeurInt;
               }
               else
               {
                  if(type == DOUBLE)
                  {
                     variableAModifier->variableDouble->pasDeValeur = 0;
                     variableAModifier->variableDouble->valeur = valeurDouble;
                  }
                  else
                  {
                     if(type == STRING)
                     {
                        if(chaine != NULL)
                        {
                           variableAModifier->variableString->pasDeValeur = 0;

                           if(variableAModifier->variableString->adresseChaine != NULL && strlen(variableAModifier->variableString->adresseChaine) > 0)
                           {
                              free(variableAModifier->variableString->adresseChaine);
                              variableAModifier->variableString->adresseChaine = NULL;
                           }
                           variableAModifier->variableString->adresseChaine = malloc ((strlen(chaine) + 1) * sizeof(char));
                           strcpy (variableAModifier->variableString->adresseChaine, chaine);
                        }
                        else
                        {
                           printf("L'adresse entrée dans le paramètre \"chaine\" est nulle alors que les paramètres de la fonction indiquent qu'il faut faire une assignation de valeur à une variable chaine de caractères non-nulle.\n");
                           retour = PROBLEME_PARAMETRES;
                        }
                     }
                     else
                     {
                        printf("Problème avec le type de variable indiqué.\n");
                        retour = PROBLEME_PARAMETRES;
                     }
                  }
               }
            }
         }
      }
   }

   return retour;
}


//Fonction d'ajout d'une variable à la liste (déclaration) avec affectation de valeur
//Retourne une erreur si la variable n'est pas trouvée dans la liste
//Retourne une erreur si une variable du même nom est déjà dans la liste
//La valeur de la variable ne peut être nulle que si cette dernière est de type string
ERREUR_LISTE_VARIABLES DeclarerVariableAvecValeurListeVariables(
   ListeVariables* laListe, char* nomVariable, TypesVariables type,
   int valeurInt, double valeurDouble, int valeurNulle, char* chaine
)
{
   //On contrôle si la variable existe, si elle existe on retourne une erreur
   ElementListeVariables *pointeurTestExistenceVariable;
   ERREUR_LISTE_VARIABLES retour = GetPointeurVariableByNom(laListe, nomVariable, &pointeurTestExistenceVariable);
   
   //Si la variable n'existe pas, c'est ok
   if(retour == VARIABLE_PAS_TROUVEE)
   {
      retour = DeclarerOuChangerValeurVariableListeVariables(laListe, nomVariable, type, 1, valeurNulle, 0, valeurInt, valeurDouble, chaine);
   }

   return retour;
}
//Fonction d'ajout d'une variable à la liste (déclaration) sans affectation de valeur
//Retourne une erreur si la variable n'est pas trouvée dans la liste
//Retourne une erreur si une variable du même nom est déjà dans la liste
ERREUR_LISTE_VARIABLES DeclarerVariableSansValeurListeVariables(
   ListeVariables* laListe, char* nomVariable, TypesVariables type
)
{
   //On contrôle si la variable existe, si elle existe on retourne une erreur
   ElementListeVariables *pointeurTestExistenceVariable;
   ERREUR_LISTE_VARIABLES retour = GetPointeurVariableByNom(laListe, nomVariable, &pointeurTestExistenceVariable);
   
   //Si la variable n'existe pas, c'est ok
   if(retour == VARIABLE_PAS_TROUVEE)
   {
      retour = DeclarerOuChangerValeurVariableListeVariables(laListe, nomVariable, type, 1, 0, 1, 0, 0, NULL);
   }

   return retour;
}

//Fonction de modification de valeur d'une variable existante dans la liste
//Retourne une erreur si la variable n'est pas trouvée dans la liste
//Retourne une erreur si le type de la variable ne coincide pas avec celui de la valeur devant être affectée
//Si valeurNulle == 1, on rend la valeur de notre variable NULLE (il faut que ce soit une chaine de caractères, sinon une erreur est renvoyée)
//Si estValeurAffecteeNombre == 1, la valeur affectée à la variable sera valeurNombre, sinon ce sera chaine
ERREUR_LISTE_VARIABLES ChangerValeurVariableListeVariables(
   ListeVariables* laListe, char* nomVariable, int estValeurAffecteeNombre, int valeurNulle,
   double valeurNombre, char* chaine
)
{
   //On contrôle si la variable existe, si elle n'existe pas on retourne une erreur
   ElementListeVariables *pointeurVariableAModifier;
   ERREUR_LISTE_VARIABLES retour = GetPointeurVariableByNom(laListe, nomVariable, &pointeurVariableAModifier);
   
   //Si la variable existe, c'est ok
   if(retour == PAS_D_ERREUR_LISTE_VARIABLES)
   {
      //On contrôle maintenant si les informations passées dans les paramètres sont correctes
      //->Il faut que le type de la valeur à affecter concorde avec le type de la variable (nombre pour int/double, chaine pour string)
      
      //Cas dans lesquels c'est correct
      if(((pointeurVariableAModifier->typeVariable == INT || pointeurVariableAModifier->typeVariable == DOUBLE) && estValeurAffecteeNombre == 1)
         || (pointeurVariableAModifier->typeVariable == STRING && estValeurAffecteeNombre == 0))
      {
         retour = DeclarerOuChangerValeurVariableListeVariables(laListe, nomVariable, pointeurVariableAModifier->typeVariable, 0, valeurNulle, 0, valeurNombre, valeurNombre, chaine);
      }
      //Sinon c'est incorrect
      else
      {
         printf("Problème la valeur à affecter n'est pas cohérent avec le type de la variable cible.\n");
         retour = PROBLEME_PARAMETRES;
      }
   }

   return retour;
}



void DetruireListeVariables(ListeVariables* ListeVariablesADetruire)
{
    if(ListeVariablesADetruire != NULL)
    {
         if(ListeVariablesADetruire->tete != NULL)
         {
            DetruireElementListeIntEtSaDescendance(ListeVariablesADetruire->tete);
         }

         free(ListeVariablesADetruire);
         ListeVariablesADetruire = NULL;
    }
}

void DetruireElementListeIntEtSaDescendance(ElementListeVariables* ElementListeVariablesADetruire)
{
   if(ElementListeVariablesADetruire != NULL)
   {
      if(ElementListeVariablesADetruire->suivant != NULL)
      {
         DetruireElementListeIntEtSaDescendance(ElementListeVariablesADetruire->suivant);
      }

      DetruireElementListeInt(ElementListeVariablesADetruire);
      ElementListeVariablesADetruire = NULL;
   }
}

void DetruireElementListeInt(ElementListeVariables* ElementListeVariablesADetruire)
{
   if(ElementListeVariablesADetruire != NULL)
   {
      //On libère l'espace des valeurs prises par la variable
      if(ElementListeVariablesADetruire->variableInt != NULL)
      {
         free(ElementListeVariablesADetruire->variableInt);
      }
      if(ElementListeVariablesADetruire->variableDouble != NULL)
      {
         free(ElementListeVariablesADetruire->variableDouble);
      }
      if(ElementListeVariablesADetruire->variableString != NULL)
      {
         if(ElementListeVariablesADetruire->variableString->adresseChaine != NULL && strlen(ElementListeVariablesADetruire->variableString->adresseChaine) > 0)
         {
            free(ElementListeVariablesADetruire->variableString->adresseChaine);
         }

         free(ElementListeVariablesADetruire->variableString);
      }

      free(ElementListeVariablesADetruire);
      ElementListeVariablesADetruire = NULL;
   }
}


int supprimerElementListeInt(listeInt *laListe, int valeur)
{
   int retour = -1;

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

   return retour;
}