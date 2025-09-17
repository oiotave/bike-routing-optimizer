#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <vector>

using namespace std;

struct solution_t {
    // Custo total da solução
    int cost;

    // Número total de veículos usados
    int used_vehicles;

    // Caminho de cada rota feita
    vector<vector<int>> routes;

    // Tempo de execução do algoritmo
    double execution_time;

    solution_t() { this->cost = 0; this->used_vehicles = 0; this->execution_time = 0.0; }
};
typedef solution_t Solution;

struct data_t {
    // Quantas de estações (desconsiderando o depósito)
    int facilities;

    // Quantidade total de veículos disponíveis
    int vehicle_number;

    // Capacidade de cada veículo
    int vehicle_capacity;

    // Demanda de bicicletas de cada estação
    vector<int> demands;

    // Matriz de custo de travessia entre as estações e o depósito
    vector<vector<int>> transit_cost; 

    data_t() { this->facilities = 0; this->vehicle_number = 0; this->vehicle_capacity = 0; }
};
typedef data_t Data;

struct heapNode_t {
    // Custo de trânsito associado a uma estação
    int value;

    // Estação associada ao custo
    int destination; 

    heapNode_t() { this->value = 0; this->destination = 0; }
    
    heapNode_t(int value, int destination) { this->value = value; this->destination = destination; } 
};
typedef struct heapNode_t HeapNode;

#endif // STRUCTS_HPP