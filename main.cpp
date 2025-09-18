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
#include <ctime>

using namespace std;

/**
 * Lê o arquivo de entrada com as informações da instância
 * 
 * @param path Caminho relativo do arquivo em texto
 * @param data Dados do problema
 */
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

/**
 * Calcula a diferença percentual entre a solução ótima e o ótimo obtido pelo algoritmo
 * 
 * @param optimal Solução ótima da instância
 * @param solution Solução inicial
 * @return Diferença percentual entre os valores das soluções
 */
double gap(int optimal, Solution* solution) {
    int solution_optimal = solution->cost;
    double gap = ((float) solution_optimal - (float) optimal) / float(optimal);
    return gap * 100;
}

/**
 * Mostra a solução do problema, rota a rota
 * 
 * @param solution Solução do problema obtida
 * @param optimal Solução ótima da instância
 */
void printSolution(Solution solution, int optimal) {
    double gap_value = gap(optimal, &solution);
    
    for(int i = 0; i < (int) solution.routes.size(); i++) {
        cout << "Rota " << i + 1 << ": ";
        for(int j = 0; j < (int) solution.routes[i].size(); j++) cout << solution.routes[i][j] << " ";
        cout << endl;
    }
    cout << endl;
    cout << "Custo: " << solution.cost << " (GAP: " << gap_value << "%)" << endl;
}

/**
 * Executa as duas versões do VND e mostra resultados no console
 * 
 * @param solution Solução do problema obtido
 * @param data Dados da instância
 * @param optimal Valor ótimo da instância
 */
void vndRunner(Solution* solution, Data* data, int optimal) {
    Solution aux_solution = *solution;
    clock_t str, end;

    cout << ">>> VND DETERMINISTICO: " << endl << endl;
    str = clock();
    Vnd::vndAlgorithm(&aux_solution, data, 4);
    end = clock();
    
    printSolution(aux_solution, optimal);
    cout << "Tempo de execucao em milissegundos: " << 1000 * (double(end - str) / CLOCKS_PER_SEC) << endl;

    cout << "\n>>> VND RANDOMICO: " << endl << endl;
    
    Solution aux, best_solution = *solution;
    double avg_cost = 0.0;
    
    str = clock();
    for(int i = 0; i < 1000; i++) {
        aux = *solution;
        Vnd::vndRandom(&aux, data, 4);
        avg_cost += (double) aux.cost;

        if(aux.cost < best_solution.cost) best_solution = aux;
    }
    end = clock();
    avg_cost = avg_cost / 1000;

    printSolution(best_solution, optimal);
    cout << "Tempo de execucao em milissegundos: " << 1000 * (double(end - str) / CLOCKS_PER_SEC) << endl;
    cout << "Media das solucoes: " << avg_cost << endl;

    *solution = best_solution.cost < aux_solution.cost ? best_solution : aux_solution;
}

int main(int argc, char* argv[]) {
    clock_t str, end;
    Solution solution1, solution2;
    Data data;

    if(argc < 3) {
        cerr << "Passe o caminho do arquivo e sua solucao otima" << endl;
        return 1;
    }
    readFile(argv[1], &data);

    str = clock();
    Algorithms::greedySolver(&data, &solution1); // Resolve primeiro com o greedySolver
    end = clock();

    solution1.execution_time = double(end - str) / CLOCKS_PER_SEC;
    
    cout << "\n== SOLUCAO DO GREEDY SOLVER SEM O VND ===\n" << endl;
    printSolution(solution1, stoi(argv[2]));
    cout << "Tempo de execucao em milissegundos: " << 1000 * solution1.execution_time << endl;

    cout << "\n=== SOLUCAO DO GREEDY SOLVER COM O VND ===\n" << endl;
    vndRunner(&solution1, &data, stoi(argv[2]));

    str = clock();
    Algorithms::bestInsertion(&data, &solution2); // Resolve o problema com o bestInsertion
    end = clock();

    solution2.execution_time = double(end - str) / CLOCKS_PER_SEC;
    
    cout << "\n== SOLUCAO DO BEST INSERTION SEM O VND ===\n" << endl;
    printSolution(solution2, stoi(argv[2]));
    cout << "Tempo de execucao em segundos: " << solution2.execution_time << endl;

    cout << "\n=== SOLUCAO DO BEST INSERTION COM O VND ===\n" << endl;
    vndRunner(&solution2, &data, stoi(argv[2]));

    return 0;
}
