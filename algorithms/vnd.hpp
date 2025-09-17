#ifndef VND_HPP
#define VND_HPP

using namespace std;

#include "../structs/structs.hpp"
#include <vector>

class Vnd {
    private:
        static bool isValid(vector<int>&, Data*);
        static bool switchSearch(Solution*, Data*, int);

    public:
        Vnd();
        static bool intraSwap(Solution*, Data*);
        static bool interSwap(Solution*, Data*);
        static bool intraReinsertion(Solution*, Data*);
        static bool interReinsertion(Solution*, Data*);
        static bool inter2opt(Solution*, Data*);
        static void vndAlgorithm(Solution*, Data*, int);
        static void vndRandom(Solution*, Data*, int);
};
#endif // VND_HPP