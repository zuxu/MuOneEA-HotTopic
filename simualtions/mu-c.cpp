#include <ea.hpp>
#include <vector>
using namespace std;

int n = 10000;
int L = 100;
double alpha = 0.25;
double beta = 0.05;
double eps = 0.05;

int num = 3; // number of "c"s
double c[3] = {3.0, 2.0, 1.0};
// note that for c < 1.0, the program might run for hours or days 
bool crossover = false;

int repeat = 10; // run 10 times for each c-mu pair
int threshold = 5; // if 5 of the runs visit all the levels

bool ok(int mu, double mpc, hotopic& ht){
	ea moea(mu, mpc, ht, crossover);
	vector<int> level(repeat, 0);
	int failure = 0;

	char buf[256];
    sprintf(buf, "e%d-%d-%.2lf-%.2lf-%.2lf-%.2lf.txt", n, L, alpha, beta, eps, mpc);
    if(crossover == true)
        buf[0] = 'g';
    ofstream result(buf, ofstream::out | ofstream::app);
    result << "mu = " << mu << endl;

    for(int i=0; i<repeat; ++i){
    	cout << "Checking for c=" << mpc;
    	cout << ", mu=" << mu;
    	cout << ", repeat=" << i << endl;
    	level[i] = moea.run(i)%1000;
    	result << level[i] << ", ";
    	if(level[i] != (L+1))
    		++ failure;
    	if(failure > (repeat - threshold)){
    		result << endl;
    		return false;
    	}
   	}
   	result << endl;
   	if((repeat-failure) >= threshold)
   		return true;
   	else
   		return false;
}

int search(double mpc, int lb, hotopic& ht){
    int left = lb;
    int right = lb;
    while(!ok(right, mpc, ht)){
        left = right;
        right *= 2;
    }

    while(right - left > 1){
        int mean = (left+right) / 2;
        if(ok(mean, mpc, ht)){
            right = mean;
        }
        else{
            left = mean;
        }
    }
    return right;
}

int main()
{
	ios_base::sync_with_stdio(false);
    srand(0);
    hotopic ht(n, L, alpha, beta, eps);

    ofstream result("mu-c.csv", ofstream::out | ofstream::app);
    vector<int> mu;
    for(int i=0; i<num; ++i){
        result << c[i] << ',';
        cout << "Search mu for c=" << c[i] << endl;
        int lowerbound = 1;
        mu.push_back(search(c[i], lowerbound, ht)); 
        result << mu[i] << endl;
    }
    result.close();
    /*
    ea mu1ea(mu, c, ht, crossover);
    
    vector<int> level(repeat, 0);
    int cnt = 0;
    int ind = 0;
    for(int i=0; i<repeat; ++i){
        level[i] = mu1ea.run(i);
        if(level[i] <= L)
            ++ cnt;
            
        if(cnt == 3){
            ind = i+1;
            break;
        }
        ++ ind;
    }
    char buf[256];
    sprintf(buf, "e%d-%d-%.2lf-%.2lf-%.2lf-%.2lf.csv", n, L, alpha, beta, eps, c);
    if(crossover == true)
        buf[0] = 'g';
    ofstream output(buf, ofstream::out | ofstream::app);
    output << "mu = " << mu << endl;
    
    for(int i=0; i<ind; ++i){
        output << level[i] << endl;
        cout << level[i] << endl;
    }
    output.close();
    */
}
