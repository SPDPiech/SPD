#include "ProblemPrzeplywowy.h"

ProblemPrzeplywowy::ProblemPrzeplywowy()
    : DaneZadan(NULL), IloscMaszyn(0), IloscZadan(0) {;}

ProblemPrzeplywowy::~ProblemPrzeplywowy()
{
    if(DaneZadan != NULL)
    {
        for(uint32_t i = 0; i < IloscZadan; i++)
            delete[] DaneZadan[i];

        delete[] DaneZadan;

    }
}

uint32_t ProblemPrzeplywowy::MaxCzasWykonania(string Kolejnosc)
{
    uint32_t Cmax = 0;
    uint32_t ObecneZadanie = 0;
    uint32_t* CmaxMaszyn;
    string AktualneZadanie = "";
    size_t PoprzedniaPozycja = 0;
    size_t ObecnaPozycja = 0;

    CmaxMaszyn = new uint32_t [IloscMaszyn];
    for(uint32_t i = 0; i < IloscMaszyn; i++)
        CmaxMaszyn[i] = 0;

    while((ObecnaPozycja = Kolejnosc.find(',', ObecnaPozycja+1)) != string::npos)
    {
        AktualneZadanie = "";
        AktualneZadanie.insert(0, Kolejnosc, PoprzedniaPozycja, ObecnaPozycja-PoprzedniaPozycja);
        ObecneZadanie = atoi(AktualneZadanie.c_str())-1;

        CmaxMaszyn[0] += DaneZadan[ObecneZadanie][0];
        for(uint32_t i = 1; i < IloscMaszyn; i++)
        {
            if(CmaxMaszyn[i-1] > CmaxMaszyn[i])
                CmaxMaszyn[i] = CmaxMaszyn[i-1] + DaneZadan[ObecneZadanie][i];
            else if(CmaxMaszyn[i-1] <= CmaxMaszyn[i])
                CmaxMaszyn[i] = CmaxMaszyn[i] + DaneZadan[ObecneZadanie][i];
        }

        PoprzedniaPozycja = ObecnaPozycja+1;
    }

    for(uint32_t i = 0; i < IloscMaszyn; i++)
    {
        if(Cmax < CmaxMaszyn[i])
            Cmax = CmaxMaszyn[i];
    }

    delete[] CmaxMaszyn;

    return Cmax;
}

void ProblemPrzeplywowy::WczytajDane(string NazwaPliku)
{

    fstream plik(NazwaPliku.c_str(), ios::in);
    if(plik.good())
    {
        plik>>IloscZadan>>IloscMaszyn;

        DaneZadan = new uint32_t* [IloscZadan];

        for(uint32_t i = 0; i < IloscZadan; i++)
            DaneZadan[i] = new uint32_t[IloscMaszyn];


        for(uint32_t i = 0; i < IloscZadan; i++)
            for(uint32_t j = 0; j < IloscMaszyn; j++)
                plik>>DaneZadan[i][j];

    }
}

uint32_t ProblemPrzeplywowy::PrzegladZupelny(string* Kolejnosc)
{
    string BadanaKolejnosc = "";
    string NajlepszaKolejnosc = "";

    char Pomocniczy[32];
    uint32_t Cmax = 300000000;
    //uint32_t WartoscPomocnicza = 0;
    uint32_t CmaxBadanejKolejnosci = 300000000;
    int i = 1;

    uint32_t* TablicaPomocnicza = new uint32_t[IloscZadan];
    for(uint32_t i = 1; i <= IloscZadan; i++)
        TablicaPomocnicza[i-1] = i;

    int CzasRozpoczecia = GetTickCount();
    do
    {

        BadanaKolejnosc = "";
        for(uint32_t i = 0; i < IloscZadan; i++)
        {
            BadanaKolejnosc.append(itoa(TablicaPomocnicza[i], Pomocniczy, 10));
            BadanaKolejnosc.append(",");
        }

        //cout<<BadanaKolejnosc<<endl;
        if(Cmax > (CmaxBadanejKolejnosci = MaxCzasWykonania(BadanaKolejnosc)))
        {
            Cmax = CmaxBadanejKolejnosci;
            NajlepszaKolejnosc = BadanaKolejnosc;
        }

       // cout<<i++<<" Kolejnosc: "<<BadanaKolejnosc<<" Cmax: "<<CmaxBadanejKolejnosci<<endl;

    }while(next_permutation(TablicaPomocnicza, TablicaPomocnicza+IloscZadan));

    //cout<<"Czas potrzebny dla przegladu zupelnego"<<GetTickCount() - CzasRozpoczecia<<endl;
    delete[] TablicaPomocnicza;

/*  cout<<"Przeglad zupelny: "<<endl;
    cout<<"Najlepsza kolejnosc: "<<NajlepszaKolejnosc<<endl;
    cout<<"Czas max dla niej: "<<Cmax<<endl;
*/
    *Kolejnosc = NajlepszaKolejnosc;
    return Cmax;
}

uint32_t ProblemPrzeplywowy::AlgorytmJohnsona(string* NajlepszaKolejnosc)
{
    string Kolejnosc;
    char Pomocniczy[32];

    uint32_t MinimalnaWartosc = 300000000;
    uint32_t NumerZadania = 0;
    uint32_t NumerMaszyny = 0;

    vector<uint32_t> Lista1;
    vector<uint32_t> Lista2;
    vector<uint32_t> ListaZadan;
    vector<uint32_t>::iterator IteratorZadania;

    if(IloscMaszyn == 2)
    {
        for(uint32_t i = 1; i <= IloscZadan; i++)
            ListaZadan.push_back(i);

        Lista1.clear();
        Lista2.clear();

        while(!ListaZadan.empty())
        {
            vector<uint32_t>::iterator i = ListaZadan.begin();
            for(; i != ListaZadan.end(); i++)
            {
                for(uint32_t j = 0; j < IloscMaszyn; j++)
                {

                    if(MinimalnaWartosc > DaneZadan[*i-1][j])
                    {
                        MinimalnaWartosc = DaneZadan[*i-1][j];
                        NumerZadania = *i;
                        IteratorZadania = i;
                        NumerMaszyny = j;
                    }
                }
            }

            if(NumerMaszyny == 0)
            {
                Lista1.push_back(NumerZadania);
                ListaZadan.erase(IteratorZadania);
                MinimalnaWartosc = 300000000;

            }else if(NumerMaszyny == 1)
            {
                Lista2.insert(Lista2.begin(), NumerZadania);
                ListaZadan.erase(IteratorZadania);
                MinimalnaWartosc = 300000000;
            }

        }


        Lista1.insert(Lista1.end(), Lista2.begin(), Lista2.end());

        Kolejnosc = "";
        for(size_t k = 0; k < Lista1.size(); k++)
        {
            Kolejnosc.append(itoa(Lista1[k], Pomocniczy, 10));
            Kolejnosc.append(",");
        }

        *NajlepszaKolejnosc = Kolejnosc;
        return MaxCzasWykonania(Kolejnosc);

    }else if(IloscMaszyn == 3)
    {

        uint32_t** PoloczoneDane = new uint32_t*[IloscZadan];
        for(uint32_t i = 0; i < IloscZadan; i++)
            PoloczoneDane[i] = new uint32_t[2];

        for(uint32_t i = 0; i < IloscZadan; i++)
            for(uint32_t j = 0; j < 2; j++)
                PoloczoneDane[i][j] = DaneZadan[i][j] + DaneZadan[i][j+1];

        for(uint32_t i = 1; i <= IloscZadan; i++)
            ListaZadan.push_back(i);

        Lista1.clear();
        Lista2.clear();

        while(!ListaZadan.empty())
        {
            vector<uint32_t>::iterator i = ListaZadan.begin();
            for(; i != ListaZadan.end(); i++)
            {
                for(uint32_t j = 0; j < 2; j++)
                {
                    if(MinimalnaWartosc > PoloczoneDane[*i-1][j])
                    {
                        MinimalnaWartosc = PoloczoneDane[*i-1][j];
                        NumerZadania = *i;
                        IteratorZadania = i;
                        NumerMaszyny = j;
                    }
                }
            }

            if(NumerMaszyny == 0)
            {
                Lista1.push_back(NumerZadania);
                ListaZadan.erase(IteratorZadania);
                MinimalnaWartosc = 300000000;

            }else if(NumerMaszyny == 1)
            {
                Lista2.insert(Lista2.begin(), NumerZadania);
                ListaZadan.erase(IteratorZadania);
                MinimalnaWartosc = 300000000;
            }

        }


        Lista1.insert(Lista1.end(), Lista2.begin(), Lista2.end());

        Kolejnosc = "";
        for(size_t k = 0; k < Lista1.size(); k++)
        {
            Kolejnosc.append(itoa(Lista1[k], Pomocniczy, 10));
            Kolejnosc.append(",");
        }

        for(uint32_t k = 0; k < IloscZadan; k++)
            delete[] PoloczoneDane[k];

        delete[] PoloczoneDane;

        *NajlepszaKolejnosc = Kolejnosc;
        return MaxCzasWykonania(Kolejnosc);

    }else
    {
        cout<<"Algorytm dzila tylko dla 2 i 3 maszyn :("<<endl;
        return 0;
    }
}

void ProblemPrzeplywowy::GenerujLosoweDane(uint32_t LiczbaZadan, uint32_t LiczbaMaszyn)
{
    if(DaneZadan == NULL)
    {
        IloscZadan = LiczbaZadan;
        IloscMaszyn = LiczbaMaszyn;

        DaneZadan = new uint32_t* [IloscZadan];

        for(uint32_t i = 0; i < IloscZadan; i++)
            DaneZadan[i] = new uint32_t[IloscMaszyn];

        for(uint32_t i = 0; i < IloscZadan; i++)
            for(uint32_t j = 0; j < IloscMaszyn; j++)
                DaneZadan[i][j] = rand()%50;

    }else
    {
        for(uint32_t i = 0; i < IloscZadan; i++)
            delete[] DaneZadan[i];

        delete[] DaneZadan;


        IloscZadan = LiczbaZadan;
        IloscMaszyn = LiczbaMaszyn;

        DaneZadan = new uint32_t* [IloscZadan];

        for(uint32_t i = 0; i < IloscZadan; i++)
            DaneZadan[i] = new uint32_t[IloscMaszyn];

        for(uint32_t i = 0; i < IloscZadan; i++)
            for(uint32_t j = 0; j < IloscMaszyn; j++)
                DaneZadan[i][j] = rand()%50;

    }

}

void ProblemPrzeplywowy::TestAlgorytmow(uint32_t MaxLiczbaZadan, uint32_t MaxLiczbaMaszyn)
{
    string NajlepszaKolejnosc ="";
    uint32_t Cmax = 0;

    if(MaxLiczbaZadan >= 10)
        MaxLiczbaZadan = 10;

    if(MaxLiczbaMaszyn >= 3)
        MaxLiczbaMaszyn = 3;


    cout<<"____________________________________________________________________________"<<endl;
    cout.width(12);
    cout<<left<<"Ilosc zadan";
    cout.width(13);
    cout<<left<<"Ilosc maszyn";
    cout.width(27);
    cout<<left<<"Przeglad (Czas/Kolejnosc)";
    cout.width(27);
    cout<<left<<"Johnson (Czas/Kolejnosc)"<<endl;

    for(uint32_t i = 1; i <= MaxLiczbaZadan; i++)
    {
        for(uint32_t j = 2; j <= MaxLiczbaMaszyn; j++)
        {
            GenerujLosoweDane(i,j);
            Cmax = PrzegladZupelny(&NajlepszaKolejnosc);
            cout.width(12);
            cout<<left<<i;
            cout.width(13);
            cout<<left<<j;
            cout.width(5);
            cout<<left<<Cmax;
            cout.width(22);
            cout<<left<<NajlepszaKolejnosc;
            Cmax = AlgorytmJohnsona(&NajlepszaKolejnosc);
            cout.width(5);
            cout<<left<<Cmax;
            cout.width(22);
            cout<<left<<NajlepszaKolejnosc;
            cout<<endl;
        }

    }
}

