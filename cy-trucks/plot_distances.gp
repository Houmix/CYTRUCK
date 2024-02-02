# Utilisation de Gnuplot pour tracer un histogramme vertical du top 10 des conducteurs
set term pngcairo
set output 'images/top10_distances.png'
set title "Option -d2 : Distance = f(Driver)"

set margins 22, 5, 5, 5
set style data lines
set title font ",8"
set style fill transparent solid 0.25

unset key

set ylabel "DRIVER NAMES"
set xlabel "DISTANCE (Km)"
set style fill solid
set ytics nomirror
set datafile separator ';'
set boxwidth 0.2 relative
set xtics font ',8'
set ytics font ',8'
# Chargez les données du fichier 'temp/top10_conducteur.txt'
plot 'temp/top10_distancesInverse.txt' using (0.5*$1):0.1:(0.5*$1):(0.3):(2):ytic(2) with boxxy lc var
