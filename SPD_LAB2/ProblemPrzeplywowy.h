#ifndef PROBLEMPRZEPLYWOWY_H
#define PROBLEMPRZEPLYWOWY_H

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <windows.h>

#include "Klasa_graf.hpp"

using namespace std;

class ProblemPrzeplywowy
{
    uint32_t** DaneZadan;
    uint32_t IloscMaszyn;
    uint32_t IloscZadan;

public:

    ProblemPrzeplywowy();
    ~ProblemPrzeplywowy();

    uint32_t MaxCzasWykonania(string Kolejnosc);
    void WczytajDane(string NazwaPliku);
    uint32_t PrzegladZupelny(string* Kolejnosc);
    uint32_t AlgorytmJohnsona(string* NajlepszaKolejnosc);
    uint32_t AlgorytmNEH(string* NajlepszaKolejnosc);
    uint32_t AkceleracjaNEH(string* NajlepszaKolejnosc);
    uint32_t ZmodyfikowanyNEH(string* NajlepszaKolejnosc);
    void GenerujLosoweDane(uint32_t LiczbaZadan, uint32_t LiczbaMaszyn);
    void TestAlgorytmow(uint32_t MaxLiczbaZadan, uint32_t MaxLiczbaMaszyn);

};

struct DaneNEH
{
    uint32_t NumerZadania;
    uint32_t Wartosc;

    DaneNEH(uint32_t NowyNumer, uint32_t NowaWartosc){NumerZadania = NowyNumer, Wartosc = NowaWartosc;}
};

#endif // PROBLEMPRZEPLYWOWY_H
