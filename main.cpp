#include "algorithms/algorithms.hpp"
#include "structs/structs.hpp"
#include "algorithms/vnd.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cerrno>
#include <vector>

using namespace std;

// Função para leitura do arquivo de entrada
int readFile(string, Data*);

// Mostra a solução
void printSolution(Solution*);

int main(int argc, char* argv[]) {
    Data data;
    Solution solution;

    if(argc < 2) {
        cerr << "Passe o caminho do arquivo" << endl;
        return 1;
    }
    readFile(argv[1], &data);
    solution.routes.resize(data.vehicle_number);

    // Resolve o problema com o algoritmo guloso
    Algorithms::greedySolver(&data, &solution);
    printSolution(&solution);

    // Melhora com o VND
    Vnd::vndAlgorithm(&solution, &data, 4);
    printSolution(&solution);

    return 0;
}

int readFile(string path, Data* data) {
    
    /*
    -- NOVA LEITURA DE ARQUIVO COM FSCANF (NÃO ESTÁ COMPLETO)
    
    FILE *file = fopen(path.c_str(), "r");

    if(!file) {
        cerr << "Erro ao abrir o arquivo" << endl;
        return 1;
    }
    if(fscanf(file, "%d", &data->facilities) != 1 or 
       fscanf(file, "%d", &data->vehicle_number) != 1 or
       fscanf(file, "%d", &data->vehicle_capacity) != 1) return 1;
       
    data->demands.resize(data->facilities);
    for(int i = 0; i < (int) data->demands.size(); i++) fscanf(file, "%d", &data->demands[i]);

    data->transit_cost.resize(data->facilities + 1);
    for(int j = 0; j < (int) data->facilities.size(); j++) data->transit_cost[j].resize(data->facilities + 1);
    
    for(int k = 0; k < (int) data->transit_cost; k++) {

    }
    

    for(int i = 0; i < (int) data->demands.size(); i++) {
        cout << data->demands[i] << endl;
    }
/*
    char line[200];
    fgets(line, sizeof(line), file);

    if(fgets(line, sizeof(line), file)) {
        char* token = strtok(line, " \n");
        while(token) {
            string value(token);
            data->demands.push_back(stoi(value));
            token = strtok(nullptr, " \n");
        }
    }
    fgets(line, sizeof(line), file);

    for(int i = 0; i < data->facilities + 1; i++) {
        if(fgets(line, sizeof(line), file)) {
            char* token = strtok(line, " \n");
            while(token) {
                string value(token);
                data->transit_cost[i].push_back(stoi(value));
                token = strtok(nullptr, " \n");
            } 
        }
    }
    fclose(file);
    */

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

void printSolution(Solution* solution) {
    for(int i = 0; i < (int) solution->routes.size(); i++) {
        cout << "Rota " << i + 1 << ": ";
        for(int j = 0; j < (int) solution->routes[i].size(); j++) cout << solution->routes[i][j] << " ";
        cout << endl;
    }
    cout << "Custo: " << solution->cost << endl;    
}