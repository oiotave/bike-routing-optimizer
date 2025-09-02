#include "../structs/structs.hpp"
#include "algorithms.hpp"
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

void Algorithms::swap(vector<HeapNode> &heap, int addr1, int addr2) {
    HeapNode aux = heap[addr1];
    heap[addr1] = heap[addr2];
    heap[addr2] = aux;
}

void Algorithms::heapify(vector<HeapNode> &heap, int index, int tam) {
    int min = index, left = 2 * index + 1, right = 2 * index + 2;

    if(left < tam and heap[left].value < heap[min].value) min = left;
    if(right < tam and heap[right].value < heap[min].value) min = right;
    
    if(min != index) {
        swap(heap, min, index);
        heapify(heap, min, tam);
    }
}

void Algorithms::makeHeap(int tam, vector<HeapNode> &heap) {
    int initial_node = (tam / 2) - 1;
    for(int i = initial_node; i >= 0; i--) heapify(heap, i, tam);
}

bool Algorithms::isValid(Data *data, HeapNode station, int *current_load, int *initial_load) {
    int dest = station.destination;
    
    // Verifica se a demanda da próxima estação fura a capacidade do veículo
    if(*current_load + data->demands[dest - 1] > data->vehicle_capacity) return false;
    
    if(*current_load + data->demands[dest - 1] < 0) {
        if(*initial_load + abs(*current_load + data->demands[dest - 1]) > data->vehicle_capacity) return false;

        else {
            *initial_load = *initial_load + abs(*current_load + data->demands[dest - 1]);
            *current_load = *current_load + abs(*current_load + data->demands[dest - 1]) + data->demands[dest - 1];
            return true;
        }
    }
    else {
        *current_load = *current_load + data->demands[dest - 1];
        return true;
    }
}

void Algorithms::greedySolver(Data* data, Solution* solution) {
    int not_visited[data->facilities];
    for(int i = 0; i < data->facilities; i++) not_visited[i] = i + 1;

    int current_load, initial_load, current_route = 0, amount_visited = 0;
    
    while(current_route + 1 <= data->vehicle_number) {
        if(amount_visited >= data->facilities) break;

        current_load = 0, initial_load = 0;
        solution->routes[current_route].push_back(0);
        int current_position = 0; // Sempre começa no depósito

        while(true) {
            vector<HeapNode> adjacents(data->facilities + 1);
            
            // Copia a linha de adjacências para fazer heap
            for(int i = 0; i <= data->facilities; i++) {
                if(current_position == i) {
                    adjacents[i].value = INT_MAX;
                    adjacents[i].destination = current_position;
                    continue;
                }
                adjacents[i].value = data->transit_cost[current_position][i];
                adjacents[i].destination = i;
            }
            makeHeap(adjacents.size(), adjacents);

            while(adjacents[0].destination != 0) {
                if(not_visited[adjacents[0].destination - 1] == -1 or isValid(data, adjacents[0], &current_load, &initial_load) == false) {
                    adjacents.erase(adjacents.begin() + 0);
                    makeHeap(adjacents.size(), adjacents);
                }
                else break;
            }
            if(adjacents[0].destination == 0) {
                solution->cost += data->transit_cost[current_position][0];
                break;
            }
            amount_visited++;
            not_visited[adjacents[0].destination - 1] = -1;
            solution->cost += adjacents[0].value;
            solution->routes[current_route].push_back(adjacents[0].destination);
            current_position = adjacents[0].destination;
        }
        solution->used_vehicles++;
        solution->routes[current_route].push_back(0);
        current_route++;
    }
}

void Algorithms::vnd() {}