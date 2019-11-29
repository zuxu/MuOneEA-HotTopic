#include <hotopic.h>

class ea{
public:
    int mu;
    bool ga;
    double c;
    hotopic * ht;
    vector<point> pop;

    ea(int populationSize, double mutationParameter, hotopic & function, bool corssOver = false){
    	mu = populationSize;
    	ga = corssOver;
    	c = mutationParameter;
    	ht = &function;
    	pop.assign(mu+1, point());
    };

    int run(int seed){
        char buf[256];
        sprintf(buf, "e%d-%d-%.2lf-%.2lf-%.2lf-%.2lf-%d-%d.csv", ht->n, ht->l, ht->a, ht->b, ht->e, c, mu, seed);
        if(ga == true)
            buf[0] = 'g';
        ofstream output(buf);
        
    	srand(seed);
    	default_random_engine gen;
    	binomial_distribution<int> dis(ht->n, c/ht->n);
    	for(int i=0; i<mu; ++i)
    		pop[i].sample(*ht);
    	int levelCount = 0;
    	vector<bool> level(ht->l+1, false);
        
    	long t = 0;
    	int deleteIndex = mu;
    	while(true){
    		// sample a point from the population
    		int m = (deleteIndex+1+(rand()%mu))%(mu+1);
        
        	if(ga && (rand()%2)){
            	int f = (deleteIndex+1+(rand()%mu))%(mu+1);
            	if(m == f)
                	pop[deleteIndex].duplicate(pop[m]);
            	else
                	pop[deleteIndex].crossover(pop[m], pop[f], *ht);            
        	}
        	else{
            	pop[deleteIndex].mutate(pop[m], *ht, gen, dis);
        	}
        
        	int highestLevel = pop[0].level;
    		int bestFitness = pop[0].fitness;
		    double worstValue = pop[0].value;
            deleteIndex = 0;
		    for(int i=1; i<=mu; ++i){
		        if(pop[i].level > highestLevel)
		            highestLevel = pop[i].level;
		        if(pop[i].fitness > bestFitness)
		            bestFitness = pop[i].fitness;
		        if(pop[i].value < worstValue){
		            worstValue = pop[i].value;
		            deleteIndex = i;
		        }
		    }
        
			++ t;
			if(level[highestLevel] == false){
				level[highestLevel] = true;
				++ levelCount;
				cout << seed << '\t' << t << '\t';
				cout << highestLevel << '/' << ht->l << '\t';
				cout << bestFitness << '/' << ht->n << endl;
			}
            
            if(t%1000 == 1){
                output << t << ',';
                output << 1.0*(ht->l-highestLevel)/ht->l << ',';
                output << 1.0*(ht->n-bestFitness)/ht->n << endl;
            }
        
        	if(bestFitness == ht->n)
            	break;
    	}
        output.close();
    	return t/1000*1000 + levelCount;
    };
};