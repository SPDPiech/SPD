#include "Kopiec.h"

cKopiec::cKopiec(): TablicaKopca(NULL), RozmiarTablicy(0)
{
    ;
}

cKopiec::~cKopiec()
{
    if(TablicaKopca != NULL)
    {
        delete [] TablicaKopca;
    }
}

void cKopiec::TworzKopiecMaxQ(uint32_t Rozmiar, DaneSchrage* Dane)
{
    RozmiarTablicy = Rozmiar;
    MaksymalnyRozmiar = Rozmiar;

    TablicaKopca = new DaneSchrage[RozmiarTablicy+1];

    if(Dane != NULL)
    {
        for(uint32_t i = 1; i < RozmiarTablicy+1; i++)
            TablicaKopca[i] = Dane[i-1];

        for(int64_t i = RozmiarTablicy/2; i > 0; i--)
            PrzeniesWDolMaxQ(i);

    }else
    {
        RozmiarTablicy = 0;
    }

    /*
    for(uint32_t i = 0; i < RozmiarTablicy+1; i++)
        std::cout<<"TablicaKopca["<<i<<"]: "<<TablicaKopca[i].q<<std::endl;
    */
}

void cKopiec::PrzeniesWDolMaxQ(uint32_t Wezel)
{
    uint32_t Nastepny = 2*Wezel;
    DaneSchrage Pomocniczy;

    while(Nastepny <= RozmiarTablicy)
    {
        if((Nastepny < RozmiarTablicy) && (TablicaKopca[Nastepny].q < TablicaKopca[Nastepny+1].q))
            Nastepny++;

        if(TablicaKopca[Wezel].q < TablicaKopca[Nastepny].q)
        {
            Pomocniczy = TablicaKopca[Wezel];
            TablicaKopca[Wezel] = TablicaKopca[Nastepny];
            TablicaKopca[Nastepny] = Pomocniczy;

            Wezel = Nastepny;
            Nastepny *= 2;

        }else
        {
            Nastepny = RozmiarTablicy + 1;
        }
    }
}

void cKopiec::PrzeniesWGoreMaxQ(uint32_t Wezel)
{
    uint32_t Nastepny = Wezel/2;
    DaneSchrage Pomocniczy;

    while((Wezel > 1) && (TablicaKopca[Nastepny].q < TablicaKopca[Wezel].q))
    {
        Pomocniczy = TablicaKopca[Wezel];
        TablicaKopca[Wezel] = TablicaKopca[Nastepny];
        TablicaKopca[Nastepny] = Pomocniczy;

        Wezel = Nastepny;
        Nastepny /= 2;
    }


}

DaneSchrage cKopiec::SciagniZKopcaMaxQ()
{
    DaneSchrage Pomocniczy;
    Pomocniczy = TablicaKopca[1];
    TablicaKopca[1] = TablicaKopca[RozmiarTablicy];
    TablicaKopca[RozmiarTablicy] = Pomocniczy;

    if(RozmiarTablicy >= 1)
    {
       RozmiarTablicy--;
       PrzeniesWDolMaxQ(1);

    }else
    {
        std::cout<<"Kopiec jest pusty !"<<std::endl;
        return DaneSchrage(0,0,0,0);
    }


    /*
    std::cout<<std::endl;
    for(uint32_t i = 0; i < RozmiarTablicy+1; i++)
        std::cout<<"TablicaKopca["<<i<<"]: "<<TablicaKopca[i].q<<std::endl;
    */

    return Pomocniczy;

}


void cKopiec::DodajDoKopcaMaxQ(DaneSchrage Dane)
{
    if(RozmiarTablicy < MaksymalnyRozmiar)
    {
        RozmiarTablicy++;
        TablicaKopca[RozmiarTablicy] = Dane;
        PrzeniesWGoreMaxQ(RozmiarTablicy);

    }else
    {
        std::cout<<"Kopiec jest pelen!"<<std::endl;
    }


    /*
    std::cout<<std::endl;
    for(uint32_t i = 0; i < RozmiarTablicy+1; i++)
        std::cout<<"TablicaKopca["<<i<<"]: "<<TablicaKopca[i].q<<std::endl;
    */
}



void cKopiec::TworzKopiecMinR(uint32_t Rozmiar, DaneSchrage* Dane)
{
    RozmiarTablicy = Rozmiar;
    MaksymalnyRozmiar = Rozmiar;

    TablicaKopca = new DaneSchrage[RozmiarTablicy+1];

    if(Dane != NULL)
    {
        for(uint32_t i = 1; i < RozmiarTablicy+1; i++)
            TablicaKopca[i] = Dane[i-1];

        for(int64_t i = RozmiarTablicy/2; i > 0; i--)
            PrzeniesWDolMinR(i);

    }else
    {
        RozmiarTablicy = 0;
    }

    /*
    for(uint32_t i = 0; i < RozmiarTablicy+1; i++)
        std::cout<<"TablicaKopca["<<i<<"]: "<<TablicaKopca[i].r<<std::endl;
    */
}

void cKopiec::PrzeniesWDolMinR(uint32_t Wezel)
{
    uint32_t Nastepny = 2*Wezel;
    DaneSchrage Pomocniczy;

    while(Nastepny <= RozmiarTablicy)
    {
        if((Nastepny < RozmiarTablicy) && (TablicaKopca[Nastepny].r > TablicaKopca[Nastepny+1].r))
            Nastepny++;

        if(TablicaKopca[Wezel].r > TablicaKopca[Nastepny].r)
        {
            Pomocniczy = TablicaKopca[Wezel];
            TablicaKopca[Wezel] = TablicaKopca[Nastepny];
            TablicaKopca[Nastepny] = Pomocniczy;

            Wezel = Nastepny;
            Nastepny *= 2;

        }else
        {
            Nastepny = RozmiarTablicy + 1;
        }
    }
}

void cKopiec::PrzeniesWGoreMinR(uint32_t Wezel)
{
    uint32_t Nastepny = Wezel/2;
    DaneSchrage Pomocniczy;

    while((Wezel > 1) && (TablicaKopca[Nastepny].r > TablicaKopca[Wezel].r))
    {
        Pomocniczy = TablicaKopca[Wezel];
        TablicaKopca[Wezel] = TablicaKopca[Nastepny];
        TablicaKopca[Nastepny] = Pomocniczy;

        Wezel = Nastepny;
        Nastepny /= 2;
    }


}

DaneSchrage cKopiec::SciagniZKopcaMinR()
{
    DaneSchrage Pomocniczy;
    Pomocniczy = TablicaKopca[1];
    TablicaKopca[1] = TablicaKopca[RozmiarTablicy];
    TablicaKopca[RozmiarTablicy] = Pomocniczy;

    if(RozmiarTablicy >= 1)
    {
       RozmiarTablicy--;
       PrzeniesWDolMinR(1);

    }else
    {
        //std::cout<<"Kopiec jest pusty !"<<std::endl;
        return DaneSchrage(0,0,0,0);
    }



    //std::cout<<std::endl;
    for(uint32_t i = 0; i < RozmiarTablicy+1; i++)
        //std::cout<<"TablicaKopca["<<i<<"]: "<<TablicaKopca[i].NumerZadania<<std::endl;



    return Pomocniczy;

}


void cKopiec::DodajDoKopcaMinR(DaneSchrage Dane)
{
    if(RozmiarTablicy < MaksymalnyRozmiar)
    {
        RozmiarTablicy++;
        TablicaKopca[RozmiarTablicy] = Dane;
        PrzeniesWGoreMinR(RozmiarTablicy);

    }else
    {
        std::cout<<"Kopiec jest pelen!"<<std::endl;
    }

    /*
    std::cout<<std::endl;
    for(uint32_t i = 0; i < RozmiarTablicy+1; i++)
        std::cout<<"TablicaKopca["<<i<<"]: "<<TablicaKopca[i].r<<std::endl;
    */
}

uint8_t cKopiec::PustyKopiec()
{
    if(RozmiarTablicy == 0)
        return 1;
    else
        return 0;
}

