#ifndef KOPIEC_H
#define KOPIEC_H

#include <iostream>
#include <cstdlib>

struct DaneSchrage
{
    uint32_t NumerZadania;
    uint32_t r;
    uint32_t p;
    uint32_t q;

    DaneSchrage(uint32_t NowyNumer, uint32_t NoweR, uint32_t NoweP, uint32_t NoweQ)
        {NumerZadania = NowyNumer; r = NoweR; p = NoweP; q = NoweQ;}

    DaneSchrage():NumerZadania(0), r(0), p(0), q(0) {;}
};


class cKopiec
{
    DaneSchrage* TablicaKopca;
    uint32_t RozmiarTablicy;
    uint32_t MaksymalnyRozmiar;

public:

    cKopiec();
    ~cKopiec();
    DaneSchrage SciagniZKopcaMaxQ();
    void DodajDoKopcaMaxQ(DaneSchrage Dane);
    void TworzKopiecMaxQ(uint32_t Rozmiar, DaneSchrage* Dane);
    void PrzeniesWGoreMaxQ(uint32_t Wezel);
    void PrzeniesWDolMaxQ(uint32_t Wezel);

    DaneSchrage SciagniZKopcaMinR();
    void DodajDoKopcaMinR(DaneSchrage Dane);
    void TworzKopiecMinR(uint32_t Rozmiar, DaneSchrage* Dane);
    void PrzeniesWGoreMinR(uint32_t Wezel);
    void PrzeniesWDolMinR(uint32_t Wezel);

    uint8_t PustyKopiec();

};



#endif
