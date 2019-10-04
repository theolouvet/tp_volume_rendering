#!/bin/bash

echo "load 'gnuplot_attenuation.p'" | gnuplot -persist > pic_attenuation.png
echo "load 'gnuplot_emission.p'" | gnuplot -persist > pic_emmision.png
