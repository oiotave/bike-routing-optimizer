#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "../structs/structs.hpp"

using namespace std;

class Algorithms {
    private:
        static void swap(vector<HeapNode>&, int, int);
        static void heapify(vector<HeapNode>&, int, int);
        static void makeHeap(int, vector<HeapNode>&);
        static bool isValid(Data*, vector<int>&);
    
    public:
        Algorithms();
        static void greedySolver(Data*, Solution*);
        static void limitedGreedy(Data*, Solution*);
        static void bestInsertion(Data*, Solution*);
};
#endif // ALGORITHMS_HPP