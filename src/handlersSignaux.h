#ifndef __HANDLERS_SIGNAUX_H
#define __HANDLERS_SIGNAUX_H

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"
#include "traiter_cmd.h"
#include "listeInt.h"
#include "variablesExternes.h"

//Les fichiers handlersSignaux contiennent toutes les fonctions relatives aux signaux pouvant être reçus par le MiniShell (handlers et fonctions permettant de 
//mettre en place ces derniers).


//-1) Handlers de signaux
//Handler appelé lorsqu'un signal SIGCHLD (signal qu'un processus fils du processus actuel, ici le MiniShell, vient de prendre fin) est reçu.
//Ce handler a pour utilité d'éviter l'invasion des processus zombis mais pas que, il contient certaines instruction permettant d'avoir un affichage assez propre (même si pas parfait).
//Ce handler est mis en place via un appel à la fonction "gestion_SIGCHLD"
void handler_SIGCHLD(int sig);

//Handler appelé lorsqu'un signal SIGINT (ctrl + c, terminaison du processus au premier plan) est reçu.
//Si un/des processus sont actuellement en cours d'exécution au premier plan, ce signal leur est envoyé
//Si il n'y a aucun processus en cours d'exécution au premier plan, rien de spécial ne se produit.
//Un retour à la ligne est imprimmé en fin de fonction pour garder un affichage propre
void handler_SIGINT(int sig);

//Handler appelé lorsqu'un signal SIGTSTP (ctrl + z, pause du processus au premier plan) est reçu.
//Si un/des processus sont actuellement en cours d'exécution au premier plan, ce signal leur est envoyé
//Si il n'y a aucun processus en cours d'exécution au premier plan, rien de spécial ne se produit.
//Un retour à la ligne est imprimmé en fin de fonction pour garder un affichage propre
void handler_SIGTSTP(int sig);


//-2) Fonctions de mise en place des handlers
//Fonction de mise en place du handler handler_SIGCHLD.
void setup_handler_SIGCHLD();

//Fonction de mise en place du handler handler_SIGINT.
void setup_handler_SIGINT();

//Fonction de mise en place du handler handler_SIGTSTP.
void setup_handler_SIGTSTP();

#endif
