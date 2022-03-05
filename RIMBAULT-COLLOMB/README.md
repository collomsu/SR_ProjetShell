# Pour executer le script pearl (se placer à la racine du dossier)
./sdriver.pl -t tests/nomDuTest.txt -s ./src/shell

# Voici une brève description des fichiers du programme
 * shell: Fichier principal du programme, contient la fonction main.
 * variablesExternes: Contient les déclarations des variables partagées entre les différents fichiers du programme.
 * traiter_cmd: Contient les différentes fonctions permettant de traiter une (série de) commande(s).
 * listeInt: Contient une structure et les fonctions qui lui sont associées permettant de gérer une liste d'entiers (utilisée ici pour gérer des listes de PIDs).
 * listeJobs: Contient une structure et les fonctions qui lui sont associées permettant de gérer une liste de jobs (pour chacun de ces derniers: sont numéro, la commande l'ayant lancé, les PIDs qui lui sont associés et son état actuel (RUNNING, STOPPED)).


Pour compiler ce MiniShell, veuillez effectuer une commande "make" depuis le répertoire "src".
