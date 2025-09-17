#include "algorithms.hpp"
#include "vnd.hpp"
#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <ctime>

using namespace std;

Vnd::Vnd() {
}

/**
 * Checa a validade de uma rota encontrada
 * 
 * @param data Dados do problema
 * @param route Rota a ser analisada
 * @return bool Validade da rota 
 */
bool Vnd::isValid(vector<int> &route, Data* data) {
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
 * Executa a vizinhança do swap para uma mesma rota
 * 
 * @param solution Solução inicial encontrada
 * @param data Dados do problema
 * @return bool Indicador de ocorrência de melhora
 */
bool Vnd::intraSwap(Solution* solution, Data* data) {
    int ibest = 0, jbest = 0, best_solution = solution->cost;
    
    // Percorre-se cada rota da solução
    for(int k = 0; k < (int) solution->routes.size(); k++) {
        int new_solution;
        vector<int> &route = solution->routes[k];
        
        // Desconsidera rotas que visitam uma estação apenas
        if(route.size() <= 3) continue;
        
        // Percorre-se cada conjunto de duas estações distintas da rota
        for(int i = 1; i < (int) route.size() - 2; i++) { 
            for(int j = i + 1; j < (int) route.size() - 1; j++) {
                // Lógica de cálculo da nova solução para estações adjacentes
                if(j == i + 1) {
                    new_solution =  solution->cost 
                    - data->transit_cost[route[i - 1]][route[i]] - data->transit_cost[route[i]][route[j]]
                    - data->transit_cost[route[j]][route[j + 1]] + data->transit_cost[route[i - 1]][route[j]] 
                    + data->transit_cost[route[j]][route[i]] + data->transit_cost[route[i]][route[j + 1]];
                }
                // Lógica de cálculo da nova solução para estações não-adjacentes
                else {
                    new_solution = solution->cost 
                    - data->transit_cost[route[i - 1]][route[i]] - data->transit_cost[route[i]][route[i + 1]] 
                    - data->transit_cost[route[j - 1]][route[j]] - data->transit_cost[route[j]][route[j + 1]]
                    + data->transit_cost[route[i - 1]][route[j]] + data->transit_cost[route[j]][route[i + 1]] 
                    + data->transit_cost[route[j - 1]][route[i]] + data->transit_cost[route[i]][route[j + 1]];
                }
                if(best_solution > new_solution) {
                    swap(route[i], route[j]);

                    // Verifica a validade da nova solução
                    if(isValid(route, data)) {
                        swap(route[i], route[j]);
                        best_solution = new_solution;
                        ibest = i;
                        jbest = j;
                    }
                    // Se não for válida, só desfaz o swap anterior
                    else swap(route[i], route[j]);
                } 
                else continue;
            }
        }
        // Atualiza para a melhor solução válida encontrada
        if(solution->cost != best_solution) {
            swap(route[ibest], route[jbest]);
            solution->cost = best_solution;
            return true;
        }
    }
    return false;
}

/**
 * Executa a vizinhança da reinserção para uma mesma rota
 * 
 * @param solution Solução inicial encontrada
 * @param data Dados do problema
 * @return bool Indicador de ocorrência de melhora
 */
bool Vnd::intraReinsertion(Solution* solution, Data* data) {
    int best_element = 0, best_position = 0, best_solution = solution->cost;
    
    // Percorre cada rota da solução
    for(int k = 0; k < (int) solution->routes.size(); k++) {
        vector<int> &route = solution->routes[k];
        
        // Não há reinserções em rotas com apenas uma estação
        if(route.size() <= 3) continue;

        // Percorre cada elemento e testa a solução para cada posição possível na rota
        for(int i = 1; i < (int) route.size() - 1; i++) {
            for(int j = 1; j < (int) route.size() - 1; j++) {
                // ignora casos análogos a swap ou com origem e destino iguais
                if(i == j or i == j - 1 or i == j + 1) continue;

                int new_solution = solution->cost 
                - data->transit_cost[route[i - 1]][route[i]]
                - data->transit_cost[route[i]][route[i + 1]]
                - data->transit_cost[route[j - 1]][route[j]]
                + data->transit_cost[route[i - 1]][route[i + 1]]
                + data->transit_cost[route[j - 1]][route[i]]
                + data->transit_cost[route[i]][route[j]];

                if(best_solution > new_solution) {
                    vector<int> aux_route = route;
                    int aux = route[i];
                    aux_route.erase(aux_route.begin() + i);
                    
                    if(j < i) aux_route.insert(aux_route.begin() + j, aux);
                    
                    else aux_route.insert(aux_route.begin() + j - 1, aux);

                    // Verifica a validade da solução
                    if(isValid(aux_route, data)) {
                        best_solution = new_solution;
                        best_element = i;
                        best_position = j;
                    }
                }
                else continue;
            }
        }
        // Substitui a solução com a melhor reinserção e custo
        if(solution->cost != best_solution) {
            int aux = route[best_element];
            route.erase(route.begin() + best_element);
            
            if(best_position < best_element) route.insert(route.begin() + best_position, aux);
            
            else route.insert(route.begin() + best_position - 1, aux);
            solution->cost = best_solution;
            return true;
        }
    }
    return false;
}

/**
 * Executa a vizinhança do swap para rotas diferentes
 * 
 * @param solution Solução inicial encontrada
 * @param data Dados do problema
 * @return bool Indicador de ocorrência de melhora
 */
bool Vnd::interSwap(Solution* solution, Data* data) {
    int best_solution = solution->cost;
    int ibest = 0, jbest = 0, best_route1 = 0, best_route2 = 0;
    
    // Percorre cada rota da solução
    for(int i = 0; i < (int) solution->routes.size() - 1; i++) {
        vector<int> &route1 = solution->routes[i];
        
        // Percorre as outras rotas restantes
        for(int j = i + 1; j < (int) solution->routes.size(); j++) {
            vector<int> &route2 = solution->routes[j];
            
            // Pecorre cada elemento da primeira rota para verificar swap
            for(int k = 1; k < (int) solution->routes[i].size() - 1; k++) {
                
                // Percorre cada elemento da segunda rota para verificar swap
                for(int l = 1; l < (int) solution->routes[j].size() - 1; l++) {

                    int new_solution = solution->cost
                    - data->transit_cost[route1[k - 1]][route1[k]] - data->transit_cost[route1[k]][route1[k + 1]]
                    - data->transit_cost[route2[l - 1]][route2[l]] - data->transit_cost[route2[l]][route2[l + 1]]
                    + data->transit_cost[route1[k - 1]][route2[l]] + data->transit_cost[route2[l]][route1[k + 1]]
                    + data->transit_cost[route2[l - 1]][route1[k]] + data->transit_cost[route1[k]][route2[l + 1]];
                    
                    if(best_solution > new_solution) {
                        vector<int> aux_route = route1, bux_route = route2;
                        swap(aux_route[k], bux_route[l]);
                        
                        // Verifica a validade de ambas as rotas alteradas
                        if(isValid(aux_route, data) and isValid(bux_route, data)) {
                            best_solution = new_solution;
                            ibest = k;
                            jbest = l;
                            best_route1 = i;
                            best_route2 = j;
                        }
                    }
                    else continue;
                }
            }
        }
    }
    if(solution->cost != best_solution) {
        swap(solution->routes[best_route1][ibest], solution->routes[best_route2][jbest]);
        solution->cost = best_solution;
        return true;
    }
    return false;
}

/**
 * Executa a vizinhança da reinserção para rotas diferentes
 * 
 * @param solution Solução inicial encontrada
 * @param data Dados do problema
 * @return bool Indicador de ocorrência de melhora
 */
bool Vnd::interReinsertion(Solution* solution, Data* data) {
    int best_solution = solution->cost;
    int ibest = 0, jbest = 0, best_route1 = 0, best_route2 = 0;
    
    // Percorre cada rota da solução
    for(int i = 0; i < (int) solution->routes.size(); i++) {
        vector<int> &route1 = solution->routes[i];
        
        // Percorre cada outra rota da solução
        for(int j = 0; j < (int) solution->routes.size(); j++) {
            if(i == j) continue;
            vector<int> &route2 = solution->routes[j];
            
            // Pecorre cada elemento da primeira rota para verificar inserção
            for(int k = 1; k < (int) solution->routes[i].size() - 1; k++) {
                // Percorre todas as posições dos outros vetores
                for(int l = 1; l < (int) solution->routes[j].size(); l++) {

                    int new_solution = solution->cost
                    - data->transit_cost[route1[k - 1]][route1[k]] - data->transit_cost[route1[k]][route1[k + 1]]
                    - data->transit_cost[route2[l - 1]][route2[l]] + data->transit_cost[route1[k - 1]][route1[k + 1]] 
                    + data->transit_cost[route2[l - 1]][route1[k]] + data->transit_cost[route1[k]][route2[l]];
                    
                    if(best_solution > new_solution) {
                        vector<int> aux_route = route1, bux_route = route2;
                        int aux = aux_route[k];
                        
                        aux_route.erase(aux_route.begin() + k);
                        bux_route.insert(bux_route.begin() + l, aux);

                        // Checa a validade de ambas as rotas alteradas
                        if(isValid(aux_route, data) and isValid(bux_route, data)) {
                            best_solution = new_solution;
                            ibest = k;
                            jbest = l;
                            best_route1 = i;
                            best_route2 = j;
                        }
                    }
                    else continue;
                }
            }
        }
    }
    // Atualiza a solução com as novas rotas
    if(solution->cost != best_solution) {
        int aux = solution->routes[best_route1][ibest];
        solution->routes[best_route1].erase(solution->routes[best_route1].begin() + ibest);
        solution->routes[best_route2].insert(solution->routes[best_route2].begin() + jbest, aux);
        solution->cost = best_solution;
        return true;
    }
    return false;
}

/**
 * Executa a vizinhança do two-opt para uma mesma rota
 * 
 * @param solution Solução inicial encontrada
 * @param data Dados do problema
 * @return bool Indicador de ocorrência de melhora
 */
bool Vnd::inter2opt(Solution* solution, Data* data) {
    int best_solution = solution->cost;
    int ibest = 0, jbest = 0, best_route1 = 0, best_route2 = 0;

    // Percorre cada rota da solução
    for(int i = 0; i < (int) solution->routes.size() - 1; i++) {
        if(solution->routes[i].size() <= 3) continue;
        vector<int> &route1 = solution->routes[i];

        // Percorre cada outra rota da solução
        for(int j = i + 1; j < (int) solution->routes.size(); j++) {
            if(solution->routes[i].size() <= 3 or i == j) continue;
            vector<int> &route2 = solution->routes[j];

            // Pecorre cada elemento da primeira rota
            for(int k = 1; k < (int) route1.size() - 2; k++) {
                // Percorre todos os elementos da outra rota
                for(int l = 1; l < (int) route2.size() - 2; l++) {
                    
                    int new_solution = solution->cost
                    - (data->transit_cost[route1[k]][route1[k+1]] + data->transit_cost[route2[l]][route2[l+1]])
                    + (data->transit_cost[route1[k]][route2[l+1]] + data->transit_cost[route2[l]][route1[k+1]]);

                    if(best_solution > new_solution) {
                        vector<int> aux_route = route1, bux_route = route2;
                        vector<int> tempA, tempB;
                        
                        for(int aux = (int) route1.size() - 1; aux > k; aux--) {
                            tempA.push_back(aux_route.back());;
                            aux_route.pop_back();
                        }
                        for(int aux = (int) route2.size() - 1; aux > l; aux--) {
                            tempB.push_back(bux_route.back());
                            bux_route.pop_back();                        
                        }
                        int A_siz = (int) tempA.size(), B_siz = (int) tempB.size();
                        
                        for(int aux = 1; aux <= B_siz; aux++)
                            aux_route.push_back(tempB[B_siz - aux]);

                        for(int aux = 1; aux <= A_siz ; aux++)
                            bux_route.push_back(tempA[A_siz - aux]);
                        
                        // Checa a validade da solução com os auxiliares
                        if(isValid(aux_route, data) and isValid(bux_route, data)) {
                            best_solution = new_solution;
                            best_route1 = i;
                            ibest = k;
                            best_route2 = j;
                            jbest = l;
                        }
                    }
                }
            }
        }
    }
    // Substitui a solução pelo melhor valor encontrado
    if(solution->cost != best_solution) {
        vector<int> &routeA = solution->routes[best_route1];
        vector<int> &routeB = solution->routes[best_route2];
        vector<int> tempA, tempB;
        
        for(int aux = (int) routeA.size() - 1; aux > ibest; aux--) {
            tempA.push_back(routeA.back());
            routeA.pop_back();
        }
        for(int aux = (int) routeB.size() - 1; aux > jbest; aux--) {
            tempB.push_back(routeB.back());
            routeB.pop_back();
        }
        int A_siz = (int) tempA.size(), B_siz = (int) tempB.size();
        
        for(int aux = 1; aux <= B_siz; aux++) routeA.push_back(tempB[B_siz - aux]);
        for(int aux = 1; aux <= A_siz; aux++) routeB.push_back(tempA[A_siz - aux]);

        solution->cost = best_solution;
        return true;
    }
    return false;
}

/**
 * Escolhe entre um dos moveimtnos de vizinhança
 * 
 * @param solution Solução inicial encontrada
 * @param data Dados do problema
 * @param option Índice do movimento escolhido
 * @return bool Indicador de ocorrência de melhora
 */
bool Vnd::switchSearch(Solution* solution, Data* data, int option) {
    switch(option) {
        case 1:
        return intraSwap(solution, data);
        
        case 2:
        return intraReinsertion(solution, data);
        
        case 3:
        return interSwap(solution, data);

        case 4:
        return interReinsertion(solution, data);

        // Essa vizinhança foi desativada, pois mesmo que melhorasse no início, ficava presa em um mínimo local
        case 5: break;
        return inter2opt(solution, data);

        default: break;
    }
    return false;
}

/**
 * Essa função implementa a lógica do algoritmo VND
 * 
 * @param solution Solução inicial do problema
 * @param data Dados do problema
 * @param neighbours Número de vizinhanças usadasd
 */
void Vnd::vndAlgorithm(Solution* solution, Data* data, int neighbours) {
    int k = neighbours;
    while(k <= neighbours) k = switchSearch(solution, data, k) ? 1 : k + 1;
}

/**
 * Essa função implementa a lógica do algoritmo VND randômico
 * 
 * @param solution Solução inicial do problema
 * @param data Dados do problema
 * @param neighbours Número de vizinhanças usadasd
 */
void Vnd::vndRandom(Solution* solution, Data* data, int neighbours) {
    srand(time(NULL));
    
    int k = (rand() % 5) + 1;
    while(k <= neighbours) k = switchSearch(solution, data, k) ? (rand() % 5) + 1 : k + 1;
}