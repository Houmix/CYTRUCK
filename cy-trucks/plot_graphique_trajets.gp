set term pngcairo enhanced size 1600,1200
set output 'images/graph_moyenne_route.png'
set encoding utf8
set margins 7, 5, 5, 5
set style data lines
set title font ",8"
set style fill transparent solid 0.25

set title "Option -s : Distance = f(Route)" 

set xrange [ 1 : * ]
set xtics rotate by 45 center font ",12" offset -1,-1 
set xlabel 'ROUTE ID'

set ylabel 'DISTANCE (Km)'
set yrange [ -1 : 1000 ]
set ytics 100

set xtics font ',8'
set ytics font ',8'


set datafile separator ";"
plot 'temp/fichier_traite_opt_s.csv' using 1:3:5 with filledcurves lc rgb "#3DDC66" t "Distances Max/Min (Km)", \
	'' using 1:4 lc rgb "#33A04F" t "Distances average (Km)" ,\
	'' using 1:2:xticlabels(2) notitle 

