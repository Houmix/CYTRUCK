# CYTRUCK

Projet de Gestion des Données de Transport Routier

Ce projet vise à créer un script Shell pour analyser et visualiser les données de transport routier d'une société nationale. Le script utilise un programme C pour traiter les données, puis génère des graphiques résumant ces données à l'aide de GNUplot.

Structure du Projet

data: Dossier contenant le fichier de données d'entrée (fichier CSV).
progc: Dossier contenant le programme C, le makefile, et l'exécutable.
temp: Dossier pour les fichiers intermédiaires nécessaires au traitement.
images: Dossier pour stocker les graphiques générés.
demo: Dossier contenant les résultats d'exécutions précédentes.


Utilisation du Script Shell
./script.sh chemin/vers/fichier.csv [options]

ATTENTION : il est necessaire d'entrer la commande chmod +x script.sh

Options Disponibles:
-h: Affiche l'aide expliquant les options.
-d1: Conducteurs avec le plus de trajets.
-d2: Conducteurs avec la plus grande distance parcourue.
-l: Les 10 trajets les plus longs.
-t: Les 10 villes les plus traversées.
-s: Statistiques sur les étapes.
Exemples d'Utilisation:
./analyse_transport.sh data/transport_data.csv -d1
./analyse_transport.sh data/transport_data.csv -d2
./analyse_transport.sh data/transport_data.csv -t


Génération des graphiques qui sont stockés dans le dossier 'images' après l'exécution du script.


Génération de fichier texte contenant les resultats ecrits dans le dossier demo
