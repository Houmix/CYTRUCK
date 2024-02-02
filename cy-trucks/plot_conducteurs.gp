# Utilisation de Gnuplot pour tracer un histogramme vertical du top 10 des conducteurs
set term pngcairo
set output 'images/top10_conducteurs.png'
set title 'Option -d1 : Nb routes = f(Driver)'
set title font ',8'
set xlabel 'NB ROUTES'
set ylabel 'DRIVER NAMES'
set style fill solid
unset key
set boxwidth 0.2
set datafile separator ';'
set xtics font ',8'
set ytics font ',8'
# Chargez les données du fichier 'temp/top10_conducteur.txt'
plot 'temp/top10_conducteurInverse.txt' using (0.5*$2):0:(0.5*$2):(0.35):(2):ytic(1) with boxxy lc var
