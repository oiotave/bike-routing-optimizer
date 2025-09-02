#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "../structs/structs.hpp"

using namespace std;

class Algorithms {
    private:
        static void swap(vector<HeapNode>&, int, int);
        static void heapify(vector<HeapNode>&, int, int);
        static void makeHeap(int, vector<HeapNode>&);
        static bool isValid(Data*, HeapNode, int*, int*);

    public:
        Algorithms();
        static void greedySolver(Data*, Solution*);
        static void vnd();
};
#endif // ALGORITHMS_HPP