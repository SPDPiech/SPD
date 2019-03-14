#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include "ProblemPrzeplywowy.h"

using namespace std;

int main()
{
    srand(time(NULL));

    uint32_t Cmax = 0;
    string NajlepszaKolejnosc;
    ProblemPrzeplywowy Problem1;
    Problem1.TestAlgorytmow(10,2);

    return 0;
}
