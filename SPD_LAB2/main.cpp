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
    Problem1.WczytajDane("Dane.txt");
    //Problem1.PrzegladZupelny(&NajlepszaKolejnosc);
    Problem1.AlgorytmNEH(&NajlepszaKolejnosc);
    //Cmax = Problem1.AlgorytmJohnsona(&NajlepszaKolejnosc);
    //Problem1.ZmodyfikowanyNEH(&NajlepszaKolejnosc);
    Problem1.AkceleracjaNEH(&NajlepszaKolejnosc);
    //Problem1.Test();

    //Problem1.TestAlgorytmow(10,3);

    return 0;
}
