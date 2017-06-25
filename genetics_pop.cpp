#include <pthread.h>
#include <sys/time.h>
#include <bits/stdc++.h>
#define NUM_THREADS 4

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

vector<vector<int> > selection(vector<vector<int> > pop){
	vector<vector<int> > parents;

	while(parents.size() < pop.size()){
		int l = rand() % pop.size();
		int r = rand() % pop.size();

		if(fitness(pop[l]) < fitness(pop[r])){
			parents.push_back(pop[l]);
		} else {
			parents.push_back(pop[r]);
		}
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

vector<vector<int> > crossover(vector<vector<int> > pop){
	vector<vector<int> > newPop;

	for(int i = 0; i < pop.size(); i += 2){
		
		
		int l = 1 + rand() % (pop.size() - 2);
		int r = l + 1 + rand() % (pop.size() - l - 1);		

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

typedef struct{
	int P, generations, sizePopulation;
	double mutationRate, divRate;
} AuxExe;

void *exe(void *aux){
	AuxExe *la = (AuxExe *) aux;
	int generations = la->generations;
	int sizePopulation = la->sizePopulation;
	int P = la->P;
	double mutationRate = la->mutationRate;
	int divRate = la->divRate;
	int bestSol = INT_MAX;
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
		}
	}

	return (void *) bestSol;	
}

int main(){
	int generations, P, x, y, sizePopulation, multSize, numPopulations;
	double mutationRate, bestSol = DBL_MAX, divRate;
	vector<int> solution;
 	
 	fstream output;
    output.open("output.dat",ios::out|ios::in|ios::app );
    
	scanf("%d %d %d", &P, &K, &numPopulations);

	cout << endl << "in" << P << "------- ------" << endl;
    string in = "in" + to_string(P) + "-1";
    string instance;
    double temposerial;
    output.seekg(ios::beg);
    //reading a sequential time
     while(true){
        output >> instance >> instance;
        if(instance.compare(in)== 0 ){ 
        	for(int i=0; i < 4; i++){

        		output >> temposerial;
        		//cout << temposerial << endl;
        		temposerial += temposerial;
        output.ignore(numeric_limits<streamsize>::max(),'\n');
        	output >> instance >> instance;	
        	}
        	break;
        	
        
        }
        output.ignore(numeric_limits<streamsize>::max(),'\n');
        
    }
// cout << temposerial << endl;
    //cout << temposerial <<  endl;
    output.seekg(0, output.end);
	for(int i = 0; i < P; i++){
		scanf("%d %d", &x, &y);

		points.push_back(make_pair(x, y));
	}

	struct timeval tp;
	gettimeofday(&tp, NULL);
	long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	pthread_t threads[numPopulations];

	for(int i = 0; i < numPopulations; i++){
		scanf("%d %lf %lf %d", &generations, &mutationRate, &divRate, &multSize);
        //generations = 10;
		
		sizePopulation = ceil(log2(points.size())) * multSize;

		if(sizePopulation % 2 == 1) sizePopulation++;

		mutationRate = mutationRate / (double) points.size();

		AuxExe *aux = (AuxExe *) malloc(sizeof(AuxExe));
		aux->P = P;
		aux->generations = generations;
		aux->mutationRate = mutationRate;
		aux->divRate = divRate;
		aux->sizePopulation = sizePopulation;

		pthread_create(&threads[i], NULL, exe, (void *) aux);
	}
	
	int ans = INT_MAX, st;

	for(int i = 0; i < numPopulations; i++){
		pthread_join(threads[i], (void **) &st);

		// cout << (int) st << endl;
	
	}
	gettimeofday(&tp, NULL);
	// cout << ((tp.tv_sec * 1000 + tp.tv_usec / 1000) - ms) << endl;
	double tempoparalelo = ((tp.tv_sec * 1000 + tp.tv_usec / 1000) - ms);
    output << "paralelo_pop " << "in" << P << " " << ((tp.tv_sec * 1000 + tp.tv_usec / 1000) - ms) << endl;
    cout << "Speedup: "<< temposerial / tempoparalelo << endl; 
    output.close();


	return 0;
}
