#!/bin/bash
#rapport, repartition des taches (+ ou -)

# Vérification des arguments
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <chemin_fichier_csv> <option>"
    exit 1
fi

# Récupération de l'argument
fichier_csv="$1"
option="$2"

# Vérification de l'existence du fichier CSV
if [ ! -f "$fichier_csv" ]; then
    echo "Erreur : Le fichier $fichier_csv n'existe pas."
    exit 2
fi
#chmod +x Makefile
#make

# Vérification de l'existence des dossiers temp et images
mkdir -p temp
mkdir -p images
mkdir -p demo

# Nettoyage du dossier temp
rm -rf temp/*
rm -rf images/*
rm -rf demo/*

# Exécution du traitement en fonction de l'option
if [ "$option" == "-h" ]; then    
    echo "Options disponnibles : "
    echo "-d1 : Conducteurs avec le plus de trajets "
    echo "-d2 : Conducteurs avec le plus long trajets "
    echo "-l : Les plus long trajets "
    echo "-t : Les 10 villes les plus traversées "
    echo "-s : Les statistiques sur les étapes "
    
elif [ "$option" == "-d1" ]; then
    # Traitement D1 : Conducteurs avec le plus de trajets
    echo "Traitement D1 : Conducteurs avec le plus de trajets"

    # Calcul du temps d'exécution
    start_time=$(date +%s)
    awk -F';' '$2 == 1 {conducteurs[$6]++} END {for (conducteur in conducteurs) print conducteurs[conducteur], conducteur}' "$fichier_csv" | sort -k1,1nr | head -n 10 | awk -F' ' '{print $2 " " $3 ";" $1}' > temp/top10_conducteur.txt 
    awk '{a[i++]=$0} END {for (j=i-1; j>=0;) print a[j--] }' temp/top10_conducteur.txt > temp/top10_conducteurInverse.txt 
    cat temp/top10_conducteurInverse.txt > demo/data_d1.txt
    gnuplot -c plot_conducteurs.gp

    # Affichage du temps d'exécution
    end_time=$(date +%s)
    elapsed_time=$((end_time - start_time))
    echo "Les resultats sont dans demo et images"
    echo "Temps d'exécution du traitement D1 : $elapsed_time secondes"


elif [ "$option" == "-d2" ]; then
    # Traitement D2 : Conducteurs avec le plus long trajets
    echo "Traitement D2 : Conducteurs avec le plus long trajets"

    # le dossier "progc" existe
    if [ ! -d "progc" ]; then
        echo "Le dossier 'progc' n'existe pas. Veuillez créer le dossier 'progc' et y placer le fichier main.c."
        exit 3
    fi

    # Exécution du programme C
    start_time=$(date +%s)

    # Compilation du code C
    gcc progc/d2.c -o progc/d2
    progc/d2 "$fichier_csv" > temp/top10_distances.txt

    awk '{a[i++]=$0} END {for (j=i-1; j>=0;) print a[j--] }' temp/top10_distances.txt > temp/top10_distancesInverse.txt 

    cat temp/top10_distancesInverse.txt > demo/data_d2.txt

    gnuplot -c plot_distances.gp

    end_time=$(date +%s)
    elapsed_time=$((end_time - start_time))
    echo "Les resultats sont dans demo et images"
    echo "Temps d'exécution du traitement D2 : $elapsed_time secondes"
        
elif [ "$option" == "-l" ]; then
    # Traitement L : Conducteurs avec le plus long trajets
    echo "Traitement L : Les plus long trajets"

    

    # Calcul du temps d'exécution
    start_time=$(date +%s)
    awk -F';' '{somme_etapes[$1]+=$5} END {for (trajet in somme_etapes) print trajet, somme_etapes[trajet]}' "$fichier_csv" | sort -k2,2nr | head -n 10 | sort -n -k1 | awk '{print $2 ";" $1}'> temp/trajets_longs.txt 
    awk '{a[i++]=$0} END {for (j=i-1; j>=0;) print a[j--] }' temp/trajets_longs.txt > temp/trajets_longsInverse.txt 

    cat temp/trajets_longsInverse.txt > demo/data_l.txt

    gnuplot -c plot_trajet.gp
    

    # Affichage du temps d'exécution
    end_time=$(date +%s)
    elapsed_time=$((end_time - start_time))
    echo "Les resultats sont dans demo et images"
    echo "Temps d'exécution du traitement L : $elapsed_time secondes"


elif [ "$option" == "-t" ]; then
    
		
	# Traitement T : Les 10 villes les plus traversées
    echo "Traitement T : "

    

    # Calcul du temps d'exécution
    start_time=$(date +%s)
    

        
		
    # Will count the number of times each city is crossed and the number of times they are departure city 
    awk -F';' 'BEGIN { OFS=";"; }
    { 
        route_id = $1;
        townA = $3;
        townB = $4;
    if (!(route_id SUBSEP townA in visited_cities) || !(route_id SUBSEP townB in visited_cities)) {
        if (!(route_id SUBSEP townA in visited_cities)) {
            visited_cities[route_id SUBSEP townA] = 1;
            count[townA] += 1;
        }
        if (!(route_id SUBSEP townB in visited_cities)) {
            visited_cities[route_id SUBSEP townB] = 1;
            count[townB] += 1;
        }
    }
    if ($2 == 1) { 
        departure_city[$3] += 1;
        }
    }
    END { 
        for (city in count) {
            print city, count[city] ";" (city in departure_city ? departure_city[city] : 0);
        }
    }' "$fichier_csv" > temp/firsttemp.csv

    gcc progc/programme_t.c -o progc/programme_t
    gcc progc/programme_t2.c -o progc/programme_t2
    ./progc/programme_t  temp/firsttemp.csv

    head -n 11 temp/firsttemp.csv > temp/secondtemp.csv
    
    ./progc/programme_t2 temp/secondtemp.csv

    cat temp/secondtemp.csv > demo/data_t.txt
    
    gnuplot -c plot_villes.gp


    # Affichage du temps d'exécution
    end_time=$(date +%s)
    elapsed_time=$((end_time - start_time))
    echo "Les resultats sont dans demo et images"
    echo "Temps d'exécution du traitement T : $elapsed_time secondes"

elif [ "$option" == "-s" ]; then
    # Traitement S : Conducteurs avec le plus long trajets
    echo "Traitement S : "

    

    # Calcul du temps d'exécution
    start_time=$(date +%s)


    
    # Enregistrez le temps de début
    gcc progc/filtre_s.c -o progc/filtre_s
    gcc progc/trier_fichier_s.c -o progc/trier_fichier_s


    awk -F";" ' FNR > 1 {print $1 ";" $5}' "$fichier_csv" > temp/s_intermediaire_calcul.csv
    touch temp/s_filtre.csv
    ./progc/filtre_s
    touch temp/fichier_traite_opt_s.csv
    ./progc/trier_fichier_s
    
    cat temp/fichier_traite_opt_s.csv > demo/data_s.txt
    gnuplot -c plot_graphique_trajets.gp

    # Affichage du temps d'exécution
    end_time=$(date +%s)
    elapsed_time=$((end_time - start_time))
    echo "Les resultats sont dans demo et images"
    echo "Temps d'exécution du traitement S : $elapsed_time secondes"
#!/bin/bash
else
    echo "Option non reconnue : $option"
    exit 4
fi

exit 0
