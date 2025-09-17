#include "../structs/structs.hpp"
#include "algorithms.hpp"
#include <iostream>
#include <climits>
#include <vector>

using namespace std;

Algorithms::Algorithms() {
}

/**
 * Essa função permuta dois nós do heap mínimo de custos
 * 
 * @param heap Fila de prioridade de custos
 * @param adddr1 Endereço do primeiro nó a ser permutado
 * @param addr2 Endereço do segundo nó a ser permutado
 */
void Algorithms::swap(vector<HeapNode> &heap, int addr1, int addr2) {
    HeapNode aux = heap[addr1];
    heap[addr1] = heap[addr2];
    heap[addr2] = aux;
}

/**
 * Executa as trocas de nós com base na prioridade
 * 
 * @param heap Fila de prioridade de custos
 * @param index Índice do nó a ser analisado
 * @param size Tamanho da fila de prioridade
 */
void Algorithms::heapify(vector<HeapNode> &heap, int index, int size) {
    int min = index;
    int lft = 2 * index + 1, rgt = 2 * index + 2;

    if(lft < size and heap[lft].value < heap[min].value) min = lft;
    if(rgt < size and heap[rgt].value < heap[min].value) min = rgt;
    
    if(min != index) {
        swap(heap, min, index);
        heapify(heap, min, size);
    }
}

/**
 * Transforma o vetor em uma fila de prioridade
 * 
 * @param size Tamanho da fila de prioridade
 * @param heap Fila de prioridade de custos a ser feita
 */
void Algorithms::makeHeap(int size, vector<HeapNode> &heap) {
    int initial_node = (size / 2) - 1;
    for(int i = initial_node; i >= 0; i--) heapify(heap, i, size);
}

/**
 * Checa a validade de uma rota encontrada
 * 
 * @param data Dados do problema
 * @param route Rota a ser analisada
 * @return bool Validade da rota 
 */
bool Algorithms::isValid(Data* data, vector<int> &route) {
    int load = 0, initial_load = 0;

    // Acumula as cargas sucessivas ao longo da rota
    for(int i = 1; i < (int) route.size(); i++) {
        load += data->demands[route[i] - 1];
        
        // Se a carga atual for insuficiente, corrige a demanda na carga inicial 
        if(load < initial_load) initial_load = load;
    }
    if(abs(initial_load) > data->vehicle_capacity) return false;

    // Valor mínimo de bicicletas para sair do depósito para poder cumprir a rota
    int required_initial = abs(initial_load);

    // Verifica se não ultrapassa capacidade em nenhum ponto
    load = required_initial;
    for(int j = 1; j < (int) route.size(); j++) {
        load += data->demands[route[j] - 1];

        if(load < 0 || load > data->vehicle_capacity) return false;
    }
    return required_initial <= data->vehicle_capacity;
}

/**
 * Executa o algoritmo guloso com a lógica de vizinho mais próximo
 * 
 * @param data Dados do problema
 * @param solution Campo para armazenar informações da solução obtida
 */
void Algorithms::greedySolver(Data* data, Solution* solution) {
    // Armazena o número de estações ainda não visitadas
    int not_visited[data->facilities];
    for(int i = 0; i < data->facilities; i++) not_visited[i] = i + 1;

    int current_route = 0, amount_visited = 0;
    
    while(current_route + 1 <= data->vehicle_number) {
        // Encerra execução quando não há mais estações para visitar
        if(amount_visited >= data->facilities) break;
        
        else solution->routes.resize((int) solution->routes.size() + 1);

        // Toda rota sempre começa no depósito
        solution->routes[current_route].push_back(0);
        int current_position = 0;
        
        while(true) {
            vector<HeapNode> adjacents(data->facilities + 1);
            
            // Copia os custos de adjacência para aquela estação em um heap mínimo
            for(int i = 0; i <= data->facilities; i++) {
                if(current_position == i) {
                    adjacents[i].value = INT_MAX;
                    adjacents[i].destination = current_position;
                    continue;
                }
                adjacents[i].value = data->transit_cost[current_position][i];
                adjacents[i].destination = i;
            }
            // Arranja o heap de acordo com o custo de trânsito
            makeHeap(adjacents.size(), adjacents);

            while(adjacents[0].destination != 0) {
                // Repete o processo caso a estação já tenha sido visitada ou não sejá viável na rota
                if(not_visited[adjacents[0].destination - 1] != -1) {
                    solution->routes[current_route].push_back(adjacents[0].destination);
                    
                    if(isValid(data, solution->routes[current_route])) {
                        solution->routes[current_route].pop_back();
                        break;
                    }
                    else solution->routes[current_route].pop_back();
                }
                adjacents.erase(adjacents.begin() + 0);
                makeHeap(adjacents.size(), adjacents);
            }
            // Encerra caso o custo menor seja retornar ao depósito
            if(adjacents[0].destination == 0) {
                solution->cost += data->transit_cost[current_position][0];
                break;
            }
            // Em outro caso, adiciona a estação a rota e atualiza a solução
            not_visited[adjacents[0].destination - 1] = -1;
            solution->cost += adjacents[0].value;
            solution->routes[current_route].push_back(adjacents[0].destination);
            current_position = adjacents[0].destination;
            amount_visited++;
        }
        // Após concluir a rota, atualiza informações da solução
        solution->routes[current_route].push_back(0);
        solution->used_vehicles++;
        current_route++;
    }
}

/**
 * Executa o algoritmo guloso com a lógica de melhor inserção
 * 
 * @param data Dados do problema
 * @param solution Campo para armazenar as informações da solução obtida
 */
void Algorithms::bestInsertion(Data* data, Solution* solution) {
    int best_route = 0, best_pos = 1, first_empty_route = 1, best_insertion;

    solution->routes.resize(data->vehicle_number);
    
    // Define-se a primeira rota para visitar a primeira estação (presução inicial)
    solution->cost = data->transit_cost[0][1] + data->transit_cost[1][0];
    solution->routes[0].push_back(0);
    solution->routes[0].push_back(1);
    solution->routes[0].push_back(0);

    // Todas as outras rotas começam a priori com a rota nula
    for(int i = 1; i < (int) solution->routes.size(); i++) {
        solution->routes[i].push_back(0);
        solution->routes[i].push_back(0);
    }
    vector<int> aux_route;

    // Percorre cada nó fora da solução para encontrar a posição de melhor inserção
    for(int i = 2; i <= data->facilities; i++) {
        // Presume-se inicialmente que a melhor é na primeira rota vazia
        best_insertion = data->transit_cost[0][i] + data->transit_cost[i][0];
        best_route = first_empty_route;
        best_pos = 1;

        // Percorre cada rota usada para checar inserção
        for(int j = first_empty_route; j >= 0; j--) {
            aux_route = solution->routes[j];

            // Percorre cada posição em uma dada rota
            for(int k = 1; k < (int) aux_route.size(); k++) { // Para cada posicao dentro de uma rota
                int insertion_cost = data->transit_cost[aux_route[k - 1]][i] 
                                   + data->transit_cost[i][aux_route[k]] 
                                   - data->transit_cost[aux_route[k - 1]][aux_route[k]];
                
                if(insertion_cost < best_insertion) {
                    vector<int> bux_route = aux_route;
                    bux_route.insert(bux_route.begin() + k, i);
                    bux_route.erase(bux_route.begin() + (int) bux_route.size() - 1);

                    // Checa se a menor solução é possível e substitui os valores
                    if(isValid(data, bux_route)) {
                        best_route = j;
                        best_pos = k;
                        best_insertion = insertion_cost;
                    }
                }
            }
        }
        if(best_route == first_empty_route and first_empty_route < (int) solution->routes.size() - 1) first_empty_route++;
        
        // Adiciona a estação na solução e incrementa a solução
        solution->routes[best_route].insert(solution->routes[best_route].begin() + best_pos, i);
        solution->cost += best_insertion;
    }
    // Incrementa o número de veículos usados
    for(int i = 0; i < (int) solution->routes.size(); i ++) {
        if(solution->routes[i].size() <= 2) {
            solution->used_vehicles++;
            break;
        }
        else solution->used_vehicles++;
    }
}