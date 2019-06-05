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
    ProblemPrzeplywowy problem1;
    //problem1.WczytajDane("in200.txt");
    uint32_t Cmax = 0;
    string order;
    //Cmax=problem1.Carlier(&order);
    //cout<<Cmax<<endl;
    problem1.Compare("Dane-1.txt");

    return 0;
}
