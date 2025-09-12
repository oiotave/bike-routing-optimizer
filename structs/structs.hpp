#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <vector>

using namespace std;

struct solution_t {
    int cost; // Custo total do problema
    int used_vehicles; // Número total de veículos usados
    vector<vector<int>> routes; // Caminho de cada rota

    solution_t() {
        this->cost = 0;
        this->used_vehicles = 0;
    }
};
typedef solution_t Solution;

struct data_t {
    int facilities; // Quantidade de estações (sem contar o depósito)
    int vehicle_number; // Quantidade total de veículos possíveis
    int vehicle_capacity; // Capacidade de cada veículo
    vector<int> demands; // Demanda de cada estação
    vector<vector<int>> transit_cost; // Matriz de custo de travessia entre as estações e o depósito

    data_t() {
        this->facilities = 0;
        this->vehicle_number = 0;
        this->vehicle_capacity = 0;
    }
};
typedef data_t Data;

struct heapNode_t {
    int value; // Custo do nó (estação)
    int destination; // Nó (estação) associada ao custo

    heapNode_t() { // Construtor padrão
        this->value = 0;
        this->destination = 0;
    }
    heapNode_t(int value, int destination) { // Construtor parâmetro
        this->value = value;
        this->destination = destination;
    } 
};
typedef struct heapNode_t HeapNode;

#endif // STRUCTS_HPP