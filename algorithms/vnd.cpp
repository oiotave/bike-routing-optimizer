#include "algorithms.hpp"
#include "vnd.hpp"
#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <ctime>

using namespace std;

bool Vnd::checkSolution(vector<int> route, Data* data) {
    int initial_load = 0, current_load = 0;
    
    for(int i = 0; i < (int) route.size() - 2; i++) {
        if(current_load + data->demands[route[i + 1] - 1] > data->vehicle_capacity) return false;

        if(current_load + data->demands[route[i + 1] - 1] < 0) {
            if(initial_load + abs(current_load + data->demands[route[i + 1] - 1]) > data->vehicle_capacity) return false;

            else {
                initial_load = initial_load + abs(current_load + data->demands[route[i + 1] - 1]);
                current_load = current_load + abs(current_load + data->demands[route[i + 1] - 1]) + data->demands[route[i + 1] - 1];
                continue;
            }
        }
        else {
            current_load = current_load + data->demands[route[i + 1] - 1];
            continue;
        }
    }
    return true;
}

bool Vnd::checkSolution(vector<int> route1, vector<int> route2, Data* data) {
    int initial_load = 0, current_load = 0;

    for(int j = 0; j < 2; j++) {  
        vector<int> aux_route = (j == 0 ? route1 : route2);
        
        for(int i = 0; i < (int) (aux_route.size() - 2); i++) {
            if(current_load + data->demands[aux_route[i + 1] - 1] > data->vehicle_capacity) return false;

            if(current_load + data->demands[aux_route[i + 1] - 1] < 0) {
                if(initial_load + abs(current_load + data->demands[aux_route[i + 1] - 1]) > data->vehicle_capacity) return false;

                else {
                    initial_load = initial_load + abs(current_load + data->demands[aux_route[i + 1] - 1]);
                    current_load = current_load + abs(current_load + data->demands[aux_route[i + 1] - 1]) + data->demands[aux_route[i + 1] - 1];
                    continue;
                }
            }
            else {
                current_load = current_load + data->demands[aux_route[i + 1] - 1];
                continue;
            }
        }
        initial_load = 0, current_load = 0;
    }
    return true;
}

bool Vnd::intraSwap(Solution* solution, Data* data) {
    int ibest = 0, jbest = 0, best_solution = solution->cost;
    bool flag = false;
    
    for(int k = 0; k < (int) solution->routes.size(); k++) { // Para cada rota
        vector<int> &route = solution->routes[k]; // Pegamos a rota
        if(route.size() <= 3) continue; // Trata soluções com um elemento ou nenhum
        
        for(int i = 1; i < (int) route.size() - 2; i++) { // Para cada elemento da rota
            for(int j = i + 1; j < (int) route.size() - 1; j++) { // Testaremos trocar com os próximos elementos
                // Swap de estações adjacentes
                if(j == i + 1) {
                    int new_solution = solution->cost - (data->transit_cost[route[i - 1]][route[i]] + data->transit_cost[route[i]][route[j]] +
                        data->transit_cost[route[j]][route[j + 1]]) + data->transit_cost[route[i - 1]][route[j]] +
                        data->transit_cost[route[j]][route[i]] + data->transit_cost[route[i]][route[j + 1]];
   
                    if(best_solution > new_solution) {
                        vector<int> aux_route = route;
                        swap(aux_route[i], aux_route[j]);

                        if(checkSolution(aux_route, data)) {
                            ibest = i;
                            jbest = j;
                            best_solution = new_solution;
                            flag = true;
                        }
                    }
                    else continue;
                }
                // Swap de estações não-adjacentes
                else {
                    int new_solution = solution->cost - (data->transit_cost[route[i - 1]][route[i]] +
                        data->transit_cost[route[i]][route[i + 1]] + data->transit_cost[route[j - 1]][route[j]] +
                        data->transit_cost[route[j]][route[j + 1]]) + data->transit_cost[route[i - 1]][route[j]] +
                        data->transit_cost[route[j]][route[i + 1]] + data->transit_cost[route[j - 1]][route[i]] +
                        data->transit_cost[route[i]][route[j + 1]];
                    
                    if(best_solution > new_solution) {
                        vector<int> aux_route = route;
                        swap(aux_route[i], aux_route[j]);

                        if(checkSolution(aux_route, data)) {
                            ibest = i;
                            jbest = j;
                            best_solution = new_solution;
                            flag = true;
                        }
                    }
                    else continue;
                }
            }
        }
        if(solution->cost != best_solution) {
            swap(route[ibest], route[jbest]);
            solution->cost = best_solution;
        }
        ibest = 0;
        jbest = 0;
    }
    return flag;
}

bool Vnd::intraReinsertion(Solution* solution, Data* data) {
    int ibest = 0, jbest = 0, best_solution = solution->cost;
    bool flag = false;
    
    for(int k = 0; k < (int) solution->routes.size(); k++) { // Para cada rota
        vector<int> &route = solution->routes[k]; // Renomeia vetor
        
        if(route.size() <= 3) continue; // Trata soluções com uma estação ou nenhuma

        for(int i = 1; i < (int) route.size() - 1; i++) { // Para cada elemento i
            for(int j = 1; j < (int) route.size() - 1; j++) { // Para cada elemento j
               if(i == j or i == j - 1 or i == j + 1) continue;

               int new_solution = solution->cost 
                - (data->transit_cost[route[i - 1]][route[i]]
                + data->transit_cost[route[i]][route[i + 1]]
                + data->transit_cost[route[j]][route[j + 1]])
                + data->transit_cost[route[i - 1]][route[i + 1]]
                + data->transit_cost[route[j]][route[i]]
                + data->transit_cost[route[i]][route[j + 1]];

                if(best_solution > new_solution) {
                    vector<int> aux_route = route;
                    int aux = route[i];
                    aux_route.erase(aux_route.begin() + i);
                    aux_route.insert(aux_route.begin() + j, aux);

                    if(checkSolution(aux_route, data)) {
                        ibest = i;
                        jbest = j;
                        best_solution = new_solution;
                        flag = true;
                    }
                }
                else continue;
            }
        }
        if(solution->cost != best_solution) {
            int aux = route[ibest];
            route.erase(route.begin() + ibest);
            route.insert(route.begin() + jbest, aux);
            solution->cost = best_solution;
        }
    }
    return flag;
}

bool Vnd::interSwap(Solution* solution, Data* data) {
    int best_solution = solution->cost;
    int ibest = 0, jbest = 0, r1best = 0, r2best = 0;
    bool flag = false;
    
    // Percorre cada rota da solução
    for(int i = 0; i < (int) solution->routes.size() - 1; i++) {
        vector<int> &route1 = solution->routes[i];
        // Percorre as rotas restantes
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
                        
                        if(checkSolution(aux_route, bux_route, data)) {
                            ibest = k;
                            jbest = l;
                            best_solution = new_solution;
                            r1best = i;
                            r2best = j;
                            flag = true;
                        }
                    }
                    else continue;
                }
            }
        }
    }
    if(solution->cost != best_solution) {
        swap(solution->routes[r1best][ibest], solution->routes[r2best][jbest]);
        solution->cost = best_solution;
    }
        
    return flag;
}

bool Vnd::interReinsertion(Solution* solution, Data* data) {
    int best_solution = solution->cost;
    int ibest = 0, jbest = 0, r1best = 0, r2best = 0;
    bool flag = false;
    
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

                        if(checkSolution(aux_route, bux_route, data)) {
                            ibest = k;
                            jbest = l;
                            best_solution = new_solution;
                            r1best = i;
                            r2best = j;
                            flag = true;
                        }
                    }
                    else continue;
                }
            }
        }
    }
    if(solution->cost != best_solution) {
        int aux = solution->routes[r1best][ibest];
        solution->routes[r1best].erase(solution->routes[r1best].begin() + ibest);
        solution->routes[r2best].insert(solution->routes[r2best].begin() + jbest, aux);
        solution->cost = best_solution;
    }
    return flag;
}

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

        default: break;
    }
    return false;
}

void Vnd::vndAlgorithm(Solution* solution, Data* data, int neighbours) {
    srand(time(NULL));

    int k = rand() % 4;
    bool ret;

    while(k <= neighbours) {
        ret = switchSearch(solution, data, k);
        k = ret ? (rand() % 4) : k + 1;
    }
}