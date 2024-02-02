set terminal pngcairo enhanced font "arial,10" fontscale 1.0 size 800, 600
set output 'images/longs_ville.png'

set title "Option -t : Nb routes = f(Towns)"
set xlabel "TOWN NAMES"
set ylabel "NB ROUTES"
set style data histograms
set style histogram cluster gap 1
set style fill solid border -1
set xtics rotate by -60 font ',8'
set ytics font ',8'
# Utilisez un espace comme séparateur de colonnes
set datafile separator ";"

# Ajustez le chemin vers votre fichier de données
plot 'temp/thirdtemp.csv' using 2:xticlabels(1) title 'Total routes', '' using 3 title 'First town'
