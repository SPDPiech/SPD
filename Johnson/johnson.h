#ifndef JOHNSON_H
#define JOHNSON_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iomanip>


using namespace std;

class flowship
{
    int tasks;
    int machines;
    int **data;

public:

    flowship();
    ~flowship();
    void LoadData(string file);
    int Cmax(string order);
    int CompleteReview(string *bestord);
    int JohnsonsAlg(string *bestorder);
    int NEHAlg(string *bestorder);
    void PrintData();
    void RandomData(int _tasks, int _machines);
    void AlgTesting(int _tasks, int _machines);
    void Compare(string file);
    int AccNEH(string *bestorder);
    int ModNEH(string *bestorder);

};

struct NEHData
{
    int TaskNumber;
    int Value;

    NEHData(int newnumber, int newvalue) {TaskNumber=newnumber, Value=newvalue;}

};

#endif // JOHNSON_H
