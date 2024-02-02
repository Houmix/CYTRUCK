# Utilisation de Gnuplot pour tracer un histogramme vertical du top 10 des conducteurs
set term pngcairo
set output 'images/top10_distances.png'
set title "Option -l  Distance = f(Route)"
set title font ",8"
set ylabel "Distance (km)"
set xlabel "Route ID"

set xtics font ',8'
set ytics font ',8'

unset key
set style fill solid
set boxwidth 0.7
set ytics nomirror
set datafile separator ';'
set yrange [0:3000]  # Définir l'intervalle de l'axe y
# Chargez les données du fichier 'temp/top10_conducteur.txt'
plot 'temp/trajets_longsInverse.txt' using 1:xticlabels(2) with boxes lc rgb "#5EF09C" title "Distance totale par conducteur"