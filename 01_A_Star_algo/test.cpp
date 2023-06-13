#include <iostream>
#include "a_star.h"

using namespace std;

int main()
{
    a_star *algo = new a_star();
    algo->input();
    algo->solve(hamming);
    delete algo;
    return 0;
}