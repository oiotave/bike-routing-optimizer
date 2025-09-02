#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <vector>

using namespace std;

struct solution_t {
    int cost;
    int used_vehicles;
    vector<vector<int>> routes;

    solution_t() {
        this->cost = 0;
        this->used_vehicles = 0;
    }
    solution_t(int cost, int used_vehicles) {
        this->cost = cost;
        this->used_vehicles = used_vehicles;
    }
};
typedef solution_t Solution;

struct data_t {
    int facilities;
    int vehicle_number;
    int vehicle_capacity;
    vector<int> demands;
    vector<vector<int>> transit_cost;

    data_t() {
        this->facilities = 0;
        this->vehicle_number = 0;
        this->vehicle_capacity = 0;
    }
    data_t(int facilities, int vehicle_number, int vehicle_capacity) {
        this->facilities = facilities;
        this->vehicle_number = vehicle_number;
        this->vehicle_capacity = vehicle_capacity;
    }
};
typedef data_t Data;

struct heapNode_t {
    int value;
    int destination;

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