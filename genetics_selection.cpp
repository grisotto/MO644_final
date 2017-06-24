#include <pthread.h>
#include <sys/time.h>
#include <bits/stdc++.h>
#define NUM_THREADS 8

using namespace std;

vector<pair<int, int> > points;
int K;

int fitness(vector<int> cromossome){
	int ans = 0;
	
	for(int i = 0; i < points.size(); i++) 
		for(int j = i + 1; j < points.size(); j++) 
			if(cromossome[i] == cromossome[j]) 
				ans += floor(0.5 + sqrt((points[i].first - points[j].first) * (points[i].first - points[j].first) + (points[i].second - points[j].second) * (points[i].second - points[j].second)));

	return ans;
}

vector<vector<int> > initialPopulation(int sizePopulation, int P){
	vector<vector<int> > pop(sizePopulation);
	
	for(int i = 0; i < sizePopulation; i++){
		for(int j = 0; j < P; j++){
			pop[i].push_back(rand() % K);
		}
	}

	return pop;
}

vector<vector<int> > parents;
vector<vector<int> > popAux;

typedef struct{
	int esq, dir;
} Aux;

pthread_mutex_t locked;
unsigned int seed;

void *sel(void *aux){
	Aux *la = (Aux *) aux;
	int esq = la->esq;
	int dir = la->dir;
	
	for(int i = esq; i < dir; i++){
		int l = rand_r(&seed) % popAux.size();
		int r = rand_r(&seed) % popAux.size();

		if(fitness(popAux[l]) < fitness(popAux[r])){
			pthread_mutex_lock(&locked);
			parents.push_back(popAux[l]);
			pthread_mutex_unlock(&locked);
		} else {	
			pthread_mutex_lock(&locked);
			parents.push_back(popAux[r]);
			pthread_mutex_unlock(&locked);
		}
	}
}

vector<vector<int> > selection(vector<vector<int> > pop){
	parents.clear();
	popAux = pop;
	pthread_t threads[NUM_THREADS];
	
	for(int i = 0; i < NUM_THREADS; i++){
		Aux *aux = (Aux *) malloc(sizeof(aux));
		aux->esq = i * pop.size() / NUM_THREADS;
		aux->dir = (i + 1) * pop.size() / NUM_THREADS;

		pthread_create(&threads[i], NULL, sel, (void *) aux);
	}

	for(int i = 0; i < NUM_THREADS; i++){
		pthread_join(threads[i], NULL);
	}

	return parents;
}

vector<int> cross(vector<int> cromossomeL, vector<int> cromossomeR, int l, int r){
	vector<int> out;

	for(int i = 0; i < cromossomeL.size(); i++){
		if(i < l || i > r){
			out.push_back(cromossomeL[i]);
		} else {
			out.push_back(cromossomeR[i]);
		}
	}

	return out;
}

vector<vector<int> > crossover(vector<vector<int> > &pop){
	vector<vector<int> > newPop;

	for(int i = 0; i < pop.size(); i += 2){
		int l = 1 + rand() % (pop[0].size() - 2);
		int r = l + 1 + rand() % (pop[0].size() - l - 1);		

		newPop.push_back(cross(pop[i], pop[i + 1], l, r));
		newPop.push_back(cross(pop[i + 1], pop[i], l, r));
	}

	return newPop;
}

void mutation(vector<vector<int> > &pop, double rate){
	for(int i = 0; i < pop.size(); i++){
		for(int j = 0; j < pop[i].size(); j++){
			double rnd = rand() / (RAND_MAX + 1.0);

			if(rnd <= rate){
				pop[i][j] = rand() % K;
			}
		}
	}
}

int getBest(vector<vector<int> > pop){
	int idx = 0;
	double bestSol = fitness(pop[0]);

	for(int i = 1; i < pop.size(); i++){
		double sol = fitness(pop[i]);

		if(sol < bestSol){
			idx = i;
			bestSol = sol; 
		}
	}

	return idx;
}

int getWorst(vector<vector<int> > pop){
	int idx = 0;
	double bestSol = fitness(pop[0]);

	for(int i = 1; i < pop.size(); i++){
		double sol = fitness(pop[i]);

		if(sol > bestSol){
			idx = i;
			bestSol = sol; 
		}
	}

	return idx;
}

vector<vector<int> > selectPopulation(vector<vector<int> > pop, vector<vector<int> > newPop){
	int idxB = getBest(pop);
	int idxW = getWorst(newPop);

	newPop[idxW] = pop[idxB];

	return newPop;
}

void diversifyPopulation(vector<vector<int> > &pop, double rate){
	int cont = 0;
	
	int idx = getBest(pop);

	for(int i = 0; i < pop.size(); i++){
		if(i == idx) continue;
		
		for(int j = i + 1; j < pop.size(); j++){
			int aux = (int) ceil(rate * points.size());

			bool flag = true;
			for(int k = 0; k < points.size(); k++){
				if(pop[i][k] != pop[j][k]) aux--;

				if(aux <= 0) break;
			}

			if(aux > 0){
				int gene = rand() % points.size();

				while(pop[i][gene] == pop[j][gene]) pop[i][gene] = rand() % K;

				break;
			}
		}			
	}	
}

int main(){
	int generations, P, x, y, sizePopulation, multSize, numPopulations, bestSol;
	double mutationRate, divRate;
	vector<int> solution;
	
	fstream output;
    output.open("output.dat",ios::out|ios::in|ios::app );
    
	scanf("%d %d", &P, &K);

	cout << endl << "in" << P << "------- ------" << endl;
    string in = "in" + to_string(P) + "-1";
    string instance;
    double temposerial;
    output.seekg(ios::beg);
    //reading a sequential time
    while(true){
        output >> instance >> instance;
        if(instance.compare(in)== 0 ) break;
        output.ignore(numeric_limits<streamsize>::max(),'\n');
        
    }
    output >> temposerial;
    //cout << temposerial <<  endl;
    output.seekg(0, output.end);

	for(int i = 0; i < P; i++){
		scanf("%d %d", &x, &y);

		points.push_back(make_pair(x, y));
	}

	scanf("%d %lf %lf %d", &generations, &mutationRate, &divRate, &multSize);
		
    generations = 10;
	sizePopulation = ceil(log2(points.size())) * multSize;

	if(sizePopulation % 2 == 1) sizePopulation++;

	mutationRate = mutationRate / (double) points.size();
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

	vector<vector<int> > pop = initialPopulation(sizePopulation, P);

	for(int i = 0; i < generations; i++){
		vector<vector<int> > parents = selection(pop);

		vector<vector<int> > cross = crossover(parents);

		mutation(cross, mutationRate);

		pop = selectPopulation(pop, cross);
		
		diversifyPopulation(pop, divRate);
		
		int idx = getBest(pop);

		if(bestSol > fitness(pop[idx])){
			bestSol = fitness(pop[idx]);
		
			// printf("Iter: %d, Sol: %d\n", i, bestSol);

			// for(int j = 0; j < pop[idx].size(); j++)
			// 	cout << pop[idx][j] << " ";
			// cout << endl;
		}
	}

	gettimeofday(&tp, NULL);
	cout << ((tp.tv_sec * 1000 + tp.tv_usec / 1000) - ms) << endl;
	double tempoparalelo = ((tp.tv_sec * 1000 + tp.tv_usec / 1000) - ms);
     output << "paralelo_selection " << "in" << P << " " << ((tp.tv_sec * 1000 + tp.tv_usec / 1000) - ms) << endl;
     cout << "Speedup: "<< temposerial / tempoparalelo << endl; 
     output.close();
	return 0;
}
