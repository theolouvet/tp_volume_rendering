# Gnuplot script
set terminal png
set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic 0.25                          # set xtics automatically
set ytic 5                          # set ytics automatically
set multiplot layout 3,1        # engage multiplot mode

set xlabel "Data value"


set ylabel "Attenuation r"
plot "output/transfert_function.dat" using 1:2 with lines ls 1 title ""

set ylabel "Attenuation g"
plot "output/transfert_function.dat" using 1:3 with lines ls 2 title ""

set ylabel "Attenuation b"
plot "output/transfert_function.dat" using 1:4 with lines ls 3 title ""

