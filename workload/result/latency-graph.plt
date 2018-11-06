set term png
set output "latency-graph.png"
set autoscale
set datafile separator ","
set xlab "Time (sec)"
set ylab "Latency (sec)"
set linestyle 1 lt 1 lw 1 lc 0
set linestyle 2 lt 1 lw 1 lc 2
set linestyle 3 lt 1 lw 1 lc 3
set linestyle 4 lt 1 lw 2 lc 1
set linestyle 10 lt 1 lw 1 lc 4
set linestyle 11 lt 1 lw 1 lc 5
set linestyle 12 lt 1 lw 1 lc 7
set arrow from 36, graph 0 to 36, graph 1 nohead ls 10
set arrow from 103, graph 0 to 103, graph 1 nohead ls 10
set arrow from 140, graph 0 to 140, graph 1 nohead ls 11
set arrow from 191, graph 0 to 191, graph 1 nohead ls 11
set arrow from 253, graph 0 to 253, graph 1 nohead ls 12
set arrow from 377, graph 0 to 377, graph 1 nohead ls 12
plot "capdynamic-mixed-1.log" using 1:($6/1000) with lines ls 1 title "Cap-dynamic", \
     "energyaware-mixed-2.log" using 1:($6/1000) with lines ls 2 title "Energy-aware", \
     "latencyaware-mixed-1.log" using 1:($6/1000) with lines ls 3 title "Latency-aware", \
     "appaware-mixed-3.log" using 1:($6/1000) with lines ls 4 title "App-aware"
