#include <bits/stdc++.h>
#include <sys/time.h>

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

int main(){
	int generations, P, x, y, sizePopulation,  numPopulations, bestSol = INT_MAX;
	
	vector<int> solution;
    generations = 1000;
    double mutationRate[4] = {0.5 , 1.0, 1.5, 2.0};
    double divRate[4] = {0.01, 0.02, 0.03, 0.04};
    int multSize[4] = {2, 3, 5, 7};


    ofstream  output, output2;
    output.open("times.dat", ios_base::app);
    output2.open("output.dat", ios_base::app);
	scanf("%d %d", &P, &K);


	for(int i = 0; i < P; i++){
		scanf("%d %d", &x, &y);

		points.push_back(make_pair(x, y));
	}
	//scanf("%d %lf %lf %d", &generations, &mutationRate, &divRate, &multSize);
        //generations = 100;
    
    for (int i = 0; i < 4; i++){
    cout << endl << "in" << P <<"-" << i+1 << "------- ------" << endl;
    output2<< endl << "in" << P <<"-" << i+1 << "------- ------" << endl;
	sizePopulation = ceil(log2(points.size())) * multSize[i];

	if(sizePopulation % 2 == 1) sizePopulation++;

	mutationRate[i] = mutationRate[i] / (double) points.size();

	struct timeval tp;
	gettimeofday(&tp, NULL);
	long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

	vector<vector<int> > pop = initialPopulation(sizePopulation, P);

	for(int ii = 0; ii < generations; ii++){
		vector<vector<int> > parents = selection(pop);

		vector<vector<int> > cross = crossover(parents);

		mutation(cross, mutationRate[i]);

		pop = selectPopulation(pop, cross);
		
		diversifyPopulation(pop, divRate[i]);
		
		int idx = getBest(pop);
	
		if(bestSol > fitness(pop[idx])){
			bestSol = fitness(pop[idx]);

			/*printf("Iter: %d, Sol: %d\n", i, bestSol);

			for(int j = 0; j < pop[idx].size(); j++)
				cout << pop[idx][j] << " ";
			cout << endl;*/
		}
	}

	gettimeofday(&tp, NULL);
	//cout << ((tp.tv_sec * 1000 + tp.tv_usec / 1000) - ms) << endl;
    output2 << bestSol << endl;
        cout<< bestSol << endl;

    output << "serial " << "in" << P << "-" << i+1 << " "  << ((tp.tv_sec * 1000 + tp.tv_usec / 1000) - ms) << endl;
    }
    output.close();
    output2.close();

	//cout << bestSol << endl;

	return 0;
}
