#include <ea.hpp>
#include <cmath>
#include <cstdio>
using namespace std;

int n = 10000;
int L = 100;
double alpha = 0.25;
double beta = 0.05;
double eps = 0.05;

int num = 11; // number of "c"s
int mu = 50;
double c[] = {0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5};
bool crossover = false;

int repeat = 10; // run 10 times for each c-mu pair

int main()
{
	ios_base::sync_with_stdio(false);
    srand(0);
    hotopic ht(n, L, alpha, beta, eps);
    
    char clevel[32], crtime[32];
    
    sprintf(clevel, "c-level-%d.csv", L);
    sprintf(crtime, "c-rtime-%d.csv", L);

    ofstream flevel(clevel, ofstream::out | ofstream::app);
    ofstream frtime(crtime, ofstream::out | ofstream::app);
    for(int i=0; i<num; ++i){
        ea moea(mu, c[i], ht, crossover);
        for(int j=0; j<repeat; ++j){
            long result = moea.run(j);
            long rtime = result/1000;   // running time (in thousand steps)
            int level = result%1000;    // number of visited levels
            frtime << c[i] << ',' << rtime << ',' << c[i]*rtime/exp(c[i]) << endl;
            flevel << c[i] << ',' << level << ',' << c[i]*level/exp(c[i]) << endl;     
        }
    }
    flevel.close();
    frtime.close();
}
