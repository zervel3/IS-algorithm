#!/bin/bash
gcc -fopenmp -o step1 main_step1.c
gcc -fopenmp -o step2 main_step2.c
gcc -fopenmp -o step3 main_step3.c
./step1 arenas-email 0.001 100000
./step2 arenas-email 0.001 100
./step3 arenas-email 0.001 100 100000


