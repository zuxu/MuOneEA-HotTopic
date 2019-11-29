#ifndef HOTOPIC_H
#define HOTOPIC_H

#include <cstring>
#include <iostream>
#include <fstream>
#include <random>
#include <map>

using namespace std;

#define bit(x) ((unsigned char)(1<<(x)))

void subset(int k, int n, int *a){
    int cur = 0;
    int left = n;
    while(k --){
        int r = rand()%left + cur;
        int tmp = a[cur];
        a[cur] = a[r];
        a[r] = tmp;
        ++ cur;
        -- left;
    }
}

class hotopic
{
public:
    int n, l;
    double a, b, e;
    int sizeA, sizeB;
    unsigned char **A;
    unsigned char **B;
    
    int * bits; // array for subset sampling
    int count[256] = {
        0, 1, 1, 2, 1, 2, 2, 3, 
        1, 2, 2, 3, 2, 3, 3, 4
    }; // array of precomputed bits count
    
    hotopic(int numBits, int numLevel, double alpha, double beta, double eps){
        n = numBits;
        l = numLevel;
        a = alpha;
        b = beta;
        e = eps;
        sizeA = n * alpha;
        sizeB = n * beta;
        
        A = new unsigned char * [numLevel];
        B = new unsigned char * [numLevel];
        for(int i=0; i<numLevel; ++i){
            A[i] = new unsigned char[numBits>>3];
            memset(A[i], 0, (n>>3));
            B[i] = new unsigned char[numBits>>3];
            memset(B[i], 0, (n>>3));
        }
        
        bits = new int[numBits];
        
        for(int i=0; i<n; ++i){
            bits[i] = i;
        }
        for(int i=0; i<l; ++i){
            // sample Ai
            subset(sizeA, n, bits);
            for(int j=0; j<sizeA; ++j){
                int x = (bits[j] >> 3);
                unsigned char y = bit(bits[j] & 0x7);
                A[i][x] |= y;
            }
            // sample Bi
            subset(sizeB, sizeA, bits);
            for(int j=0; j<sizeB; ++j){
                int x = (bits[j] >> 3);
                unsigned char y = bit(bits[j] & 0x7);
                B[i][x] |= y;
            }
        }
        
        for(int i=16; i<256; ++i){
            count[i] = count[i&0xf] + count[i>>4];
        }
    };
    
    ~hotopic(){
        for(int i=0; i<l; ++i){
            delete A[i];
            delete B[i];
        }
        delete A;
        delete B;
        delete bits;
    }
    
    void print(string s){
        ofstream output(s);
        for(int i=0; i<l; ++i){
            for(int j=0; j<(n>>3); ++j){
                for(int k=0; k<8; ++k){
                    if(A[i][j] & bit(k))
                        output << 1;
                    else
                        output << 0;
                }
            }
            output << endl;
        }
        for(int i=0; i<l; ++i){
            for(int j=0; j<(n>>3); ++j){
                for(int k=0; k<8; ++k){
                    if(B[i][j] & bit(k))
                        output << 1;
                    else
                        output << 0;
                }
            }
            output << endl;
        }
    }
};

class point{
public:
    int n;
    int level;
    int fitness;
    int numAlpo; // number of bit in A[l+1]
    double value;
    unsigned char * bits;
    
    point(){
        n = 0;
        bits = NULL;
    }
    
    ~point(){
        if(bits != NULL)
            delete bits;
    }
    
    void sample(hotopic& ht){ // sample a random search point
        if(n != ht.n){
            if(bits != NULL)
                delete bits;
            n = ht.n;
            bits = new unsigned char[n>>3];
        }
        
        for(int i=0; i<(n>>3); ++i){
            bits[i] = (unsigned char)(rand() % 256);
        }
        evaluate(ht);
    };
    
    void duplicate(point& p){
        if(n != p.n){
            if(bits != NULL)
                delete bits;
            n = p.n;
            bits = new unsigned char[n>>3];
        }
        level = p.level;
        value = p.value;
        fitness = p.fitness;
        numAlpo = p.numAlpo;
        copy(p.bits, p.bits+(n>>3), bits);
    }
    
    void mutate(point& p, hotopic& ht,
        default_random_engine& gen, binomial_distribution<int>& dis){ // create a search point by mutation
        int num = dis(gen);
        if(num == 0){ // duplication
            duplicate(p);
            return;
        }
        
        if(n != p.n){
            if(bits != NULL)
                delete bits;
            n = p.n;
            bits = new unsigned char[n>>3];
        }
        
        copy(p.bits, p.bits+(n>>3), bits);
        map<int, bool> mutated;
        while(num){
            int r = rand() % n;
            if(mutated[r])
                continue;
            int x = (r>>3);
            unsigned char y = bit(r & 0x7);
            
            bits[x] ^= y;
            mutated[r] = true;
            -- num;
        }
        evaluate(ht);
    };
    
    void crossover(point& f, point& m, hotopic& ht){
        if(n != f.n){
            if(bits != NULL)
                delete bits;
            n = f.n;
            bits = new unsigned char[n>>3];
        }
        for(int i=0; i<(n>>3); ++i){
            unsigned char maskf = rand() % 256;
            unsigned char maskm = maskf ^ 0xff;
            bits[i] = (maskf&f.bits[i]) | (maskm&m.bits[i]);
        }
        evaluate(ht);
    }
    
    void evaluate(hotopic& ht){
        // calculate the fitness
        fitness = 0;
        for(int i=0; i<(n>>3); ++i){
            fitness += ht.count[bits[i]];
        }
        
        // calculate the level
        level = -1;
        for(int i=ht.l-1; i>=0; --i){
            int cnt = 0;
            for(int j=0; j<(n>>3); ++j){
                cnt += ht.count[ht.B[i][j] & bits[j]];
            }
            if(cnt >= (1.0-ht.e)*ht.sizeB){
                level = i;
                break;
            }
        }
        level += 1;
        
        // calculate numAlpo
        numAlpo = 0;
        if(level != ht.l){
            for(int i=0; i<(n>>3); ++i)
                numAlpo += ht.count[ht.A[level][i] & bits[i]];
        }
        
        value = 1.0*n*n*level + n*numAlpo + (fitness-numAlpo);
    };
    
    void print(ofstream& output){
        output << fitness << endl;
        output << level << endl;
        output << numAlpo << endl;
        for(int i=0; i<(n>>3); ++i){
            for(int j=0; j<8; ++j){
                if(bits[i] & bit(j))
                    output << 1;
                else
                    output << 0;
            }
        }
        output << endl;
    }
    /*bool operator<(const point& p){
        if(level > p.level)
            return true;
        else if(level == p.level){
            if(numAlpo > p.numAlpo)
                return true;
            else if(numAlpo == p.numAlpo){
                if(fitness-numAlpo > p.fitness-p.numAlpo)
                    return true;
            }
        }
        return false;
    };*/
};

#endif // HOTOPIC_H
