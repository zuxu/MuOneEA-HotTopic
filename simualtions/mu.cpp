#include <ea.hpp>
#include <vector>
using namespace std;

int n = 10000;
int L = 100;
double alpha = 0.25;
double beta = 0.05;
double eps = 0.05;

int num = 11; // number of "mu"s
int mu[] = {1, 10, 20, 30, 40 ,50, 60, 70, 80, 90, 100};
double c = 1.0;
bool crossover = false;

int repeat = 10; // run 10 times for each c-mu pair

int main()
{
	ios_base::sync_with_stdio(false);
    srand(0);
    hotopic ht(n, L, alpha, beta, eps);

    ofstream level("mu-level.csv", ofstream::out | ofstream::app);
    ofstream rtime("mu-rtime.csv", ofstream::out | ofstream::app);
    for(int i=0; i<num; ++i){
        ea moea(mu[i], c, ht, crossover);
        for(int j=0; j<repeat; ++j){
            long result = moea.run(j);
            rtime << mu[i] << ',' << result/1000 << ',' << result/1000.0/mu[i] << endl;
            level << mu[i] << ',' << result%1000 << ',' << 1.0*(result%1000)/mu[i] << endl;     
        }
    }
    level.close();
    rtime.close();
}
