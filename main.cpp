#include "algorithms/algorithms.hpp"
#include "./structs/structs.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cerrno>
#include <vector>

using namespace std;

int readFile(string, Data*); // Leitura do arquivo de entrada

int main(int argc, char* argv[]) {
    Data data;
    Solution solution;

    if(argc < 2) {
        cerr << "Passe o caminho do arquivo" << endl;
        return 1;
    }
    readFile(argv[1], &data);
    solution.routes.resize(data.vehicle_number);

    Algorithms::greedySolver(&data, &solution);

    for(int i = 0; i < (int) solution.routes.size(); i++) {
        cout << "Rota " << i + 1 << ": ";
        for(int j = 0; j < (int) solution.routes[i].size(); j++)
            cout << solution.routes[i][j] << " ";
        cout << endl;
    }
    cout << "Custo: " << solution.cost << endl;
    return 0;
}

int readFile(string path, Data* data) {
    ifstream file(path);
    string linha, aux;

    if(!file.is_open()) {
        cerr << "Erro ao abrir o arquivo" << endl;
        return 1;
    }
    getline(file, linha);
    data->facilities = stoi(linha);
    data->transit_cost.resize(data->facilities + 1);

    getline(file, linha);
    data->vehicle_number = stoi(linha);
    
    getline(file, linha);
    data->vehicle_capacity = stoi(linha);
    
    getline(file, linha); // Consome a linha vazia
    getline(file, linha);

    size_t i = 0, j;
    while(i < linha.length()){
        if(linha[i] == ' ') {
            data->demands.push_back(stoi(aux));
            aux = "";
        }
        aux += linha[i++];
    }
    if(aux != "") data->demands.push_back(stoi(aux));
    getline(file, linha);
    
    i = 0;
    while(getline(file, linha)) {
        j = 0, aux = "";
        while(j < linha.length()){
            if(linha[j] == ' ') {
                data->transit_cost[i].push_back(stoi(aux));
                aux = "";
            }
            aux += linha[j++];
        }
        if(aux != "") data->transit_cost[i++].push_back(stoi(aux));
    }
    file.close();
    return 0;
}