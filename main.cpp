#include "algorithms/algorithms.hpp"
#include "structs/structs.hpp"
#include "algorithms/vnd.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cerrno>
#include <vector>

using namespace std;

int readFile(string path, Data* data) { 
    FILE *file = fopen(path.c_str(), "r");

    if(!file) {
        cerr << "Erro ao abrir o arquivo" << endl;
        exit(1);
    }
    if(fscanf(file, "%d", &data->facilities) != 1 or 
       fscanf(file, "%d", &data->vehicle_number) != 1 or
       fscanf(file, "%d", &data->vehicle_capacity) != 1) return 1;
       
    data->demands.resize(data->facilities);
    for(int i = 0; i < (int) data->demands.size(); i++) 
        if(fscanf(file, "%d", &data->demands[i]) != 1) return 1;

    data->transit_cost.resize(data->facilities + 1);
    for(int j = 0; j < (int) data->facilities + 1; j++) data->transit_cost[j].resize(data->facilities + 1);
    
    for(int k = 0; k < (int) data->transit_cost.size(); k++) {
        for(int l = 0; l < (int) data->transit_cost[k].size(); l++) {
            if(fscanf(file, "%d", &data->transit_cost[k][l]) != 1) return 1;
        }
    }    
    fclose(file);
    return 0;
}

void printSolution(Solution solution) {
    for(int i = 0; i < (int) solution.routes.size(); i++) {
        cout << "Rota " << i + 1 << ": ";
        for(int j = 0; j < (int) solution.routes[i].size(); j++) cout << solution.routes[i][j] << " ";
        cout << endl;
    }
    cout << endl;
    cout << "Custo: " << solution.cost << endl;    
}

void printCost(Solution solution, Data data) {
    int trueCost = 0;
    for(int i = 0; i < (int) solution.routes.size(); i++) {
        vector<int> curr_r = solution.routes[i];
        for(int j = 0; j < (int) curr_r.size() - 1; j++) {
            trueCost += data.transit_cost[curr_r[j]][curr_r[j+1]];
        }
    }
    cout << "Custo real: " << trueCost << "\tDiferenca: " << solution.cost-trueCost << endl;
}

void runner(Solution* solution, Data* data, int algorithm, int neighbor) {
    switch(algorithm) {
        case 1:
        Algorithms::greedySolver(data, solution);
        break;
        
        case 2:
        Algorithms::bestInsertion(data, solution);
        break;
        
        default: return;
    }
    Vnd::vndAlgorithm(solution, data, neighbor);

    printSolution(*solution);
    printCost(*solution, *data);
}

int main(int argc, char* argv[]) {
    Data data;
    Solution solution;

    if(argc < 2) {
        cerr << "Passe o caminho do arquivo" << endl;
        return 1;
    }
    readFile(argv[1], &data);

    runner(&solution, &data, 1, 4);
    return 0;
}
