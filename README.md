# (μ+1)-Evolutionary-Algorithm for Hot-Topic functions
## How to Use
When compiling .cpp files that have included hotopic.h or ea.hpp, append following to the compiler:
-std=c++11;
-I [path], where [path] is the directory containing hotopic.h or ea.hpp;
-O2 or -O3, which makes the program significantly faster.

## hotopic.h and ea.hpp
Only support hot-topic functions, of which the number of bits is a multiple of 8.
Crossover can be enabled, but this has not been tested thoroughly.
Member function ea::run() outputs a .csv file, which records information of fitness and levels every 1000 steps. It returns an integer [return], such that [return]%1000 is the number of visited levels and [return]/1000 is the running time (in 1000 steps). See /simulations/c.cpp for sample usage.

## /simulations
Codes that are used to generate plots in the paper.



