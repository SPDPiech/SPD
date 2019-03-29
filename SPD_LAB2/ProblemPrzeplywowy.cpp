#include "ProblemPrzeplywowy.h"

ProblemPrzeplywowy::ProblemPrzeplywowy()
    : DaneZadan(NULL), IloscMaszyn(0), IloscZadan(0) {;}

ProblemPrzeplywowy::~ProblemPrzeplywowy()
{
    //Zwalnianie zaalokowanej pamieci
    if(DaneZadan != NULL)
    {
        for(uint32_t i = 0; i < IloscZadan; i++)
            delete[] DaneZadan[i];

        delete[] DaneZadan;

    }
}

//Funkcja obliczajaca maxymalny czas jaki jest potrzebny do wykonanania zadanej kolejnosci zadan
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

    //Rob dopoki w stringu sa jeszcze liczby
    while((ObecnaPozycja = Kolejnosc.find(',', ObecnaPozycja+1)) != string::npos)
    {
        //Pobiera ze stringu akualne zadanie do uwzglednienia
        AktualneZadanie = "";
        AktualneZadanie.insert(0, Kolejnosc, PoprzedniaPozycja, ObecnaPozycja-PoprzedniaPozycja);
        ObecneZadanie = atoi(AktualneZadanie.c_str())-1;

        //Dodaj czas na pierwszej maszynie
        CmaxMaszyn[0] += DaneZadan[ObecneZadanie][0];

        //Jezeli czas zakonczenia zadania na poprzedniej maszynie jest dluzszy niz czas zakonczenia na obecnej maszynie
        //to czas zakonczenia zadania na obecnej maszynie jest rowny czasowi zakonczenia na poprzedniej + czas zadania
        //inaczej czas zakonczenia zadania na obecnej maszynie jest rowny czasowi zakonczenia na tej maszynie bez nowego
        //zadania oraz czas nowego zadania
        for(uint32_t i = 1; i < IloscMaszyn; i++)
        {
            if(CmaxMaszyn[i-1] > CmaxMaszyn[i])
                CmaxMaszyn[i] = CmaxMaszyn[i-1] + DaneZadan[ObecneZadanie][i];
            else if(CmaxMaszyn[i-1] <= CmaxMaszyn[i])
                CmaxMaszyn[i] = CmaxMaszyn[i] + DaneZadan[ObecneZadanie][i];
        }

        //Potrzebne do przesuniecia wyszukiwania w stringu
        PoprzedniaPozycja = ObecnaPozycja+1;
    }

    //Wybiera najwieksze Cmax z posrud wszystkich maszyn (to nie potrzebne Cmax = CmaxMaszyn[IloscMaszyn-1]
    for(uint32_t i = 0; i < IloscMaszyn; i++)
    {
        if(Cmax < CmaxMaszyn[i])
            Cmax = CmaxMaszyn[i];
    }

    delete[] CmaxMaszyn;

    return Cmax;
}

//Wczytywanie danych z pliku
void ProblemPrzeplywowy::WczytajDane(string NazwaPliku)
{

    fstream plik(NazwaPliku.c_str(), ios::in);
    if(plik.good())
    {
        if(DaneZadan == NULL)
        {
            plik>>IloscZadan>>IloscMaszyn;

            DaneZadan = new uint32_t* [IloscZadan];

            for(uint32_t i = 0; i < IloscZadan; i++)
                DaneZadan[i] = new uint32_t[IloscMaszyn];


            for(uint32_t i = 0; i < IloscZadan; i++)
                for(uint32_t j = 0; j < IloscMaszyn; j++)
                    plik>>DaneZadan[i][j];

        }else
        {

            for(uint32_t i = 0; i < IloscZadan; i++)
            delete[] DaneZadan[i];

            delete[] DaneZadan;

            //Ponowne wczytanie danych
            plik>>IloscZadan>>IloscMaszyn;

            DaneZadan = new uint32_t* [IloscZadan];

            for(uint32_t i = 0; i < IloscZadan; i++)
                DaneZadan[i] = new uint32_t[IloscMaszyn];


            for(uint32_t i = 0; i < IloscZadan; i++)
                for(uint32_t j = 0; j < IloscMaszyn; j++)
                    plik>>DaneZadan[i][j];
        }


    }
}

//Funkcja realizujaca sprawdzenie najlepszego Cmax dla podanej ilosci zadan
uint32_t ProblemPrzeplywowy::PrzegladZupelny(string* Kolejnosc)
{
    string BadanaKolejnosc = "";
    string NajlepszaKolejnosc = "";

    char Pomocniczy[32];
    uint32_t Cmax = 300000000;
    //uint32_t WartoscPomocnicza = 0;
    uint32_t CmaxBadanejKolejnosci = 300000000;
    //int i = 1;

    uint32_t* TablicaPomocnicza = new uint32_t[IloscZadan];
    for(uint32_t i = 1; i <= IloscZadan; i++)
        TablicaPomocnicza[i-1] = i;

    //int CzasRozpoczecia = GetTickCount();
    do
    {

        //Zamienia wartosci z int na string aby mozna bylo zbadac wygenerowana kolejnosc
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

    cout<<"Przeglad zupelny: "<<endl;
    cout<<"Najlepsza kolejnosc: "<<NajlepszaKolejnosc<<endl;
    cout<<"Czas max dla niej: "<<Cmax<<endl;

    *Kolejnosc = NajlepszaKolejnosc;
    return Cmax;
}

//Funkcja implementuje algorytm johnsona dla 2 i 3 maszyn
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
            //Przeszukiwanie listy w poszukiwaniu zadania z minimalna wartoscia czasu trwania operacji
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

            //Jezeli na pierwszej maszynie byla najkrotsza operacja to dodajemy na koniec pierwszej listy
            if(NumerMaszyny == 0)
            {
                Lista1.push_back(NumerZadania);
                ListaZadan.erase(IteratorZadania);
                MinimalnaWartosc = 300000000;

            }else if(NumerMaszyny == 1) //Jezeli na drugiej maszynie to dodajemy na poczatek drugiej listy
            {
                Lista2.insert(Lista2.begin(), NumerZadania);
                ListaZadan.erase(IteratorZadania);
                MinimalnaWartosc = 300000000;
            }

        }


        //Laczymy listy i sprawdzamy Cmax dla tej kolejnosci
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

uint32_t ProblemPrzeplywowy::AlgorytmNEH(string* NajlepszaKolejnosc)
{
    vector<DaneNEH> ZsumowaneWagi;

    for(uint32_t i = 0; i < IloscZadan; i++)
    {
        ZsumowaneWagi.push_back(DaneNEH(i, 0));
        for(uint32_t j = 0; j < IloscMaszyn; j++)
        {
            ZsumowaneWagi[i].Wartosc += DaneZadan[i][j];
        }
    }

    vector<uint32_t> PosortowanaTablica;
    uint32_t MaxWartosc = 0;
    uint32_t NumerZadania = 0;
    vector<DaneNEH>::iterator i = ZsumowaneWagi.begin();
    vector<DaneNEH>::iterator Iterator;



    while(!ZsumowaneWagi.empty())
    {
        i = ZsumowaneWagi.begin();
        for(; i != ZsumowaneWagi.end(); i++)
        {
            for(uint32_t j = 0; j < IloscMaszyn; j++)
            {
                if(i->Wartosc > MaxWartosc)
                {
                    MaxWartosc = i->Wartosc;
                    NumerZadania = i->NumerZadania;
                    Iterator = i;
                }
            }
        }

        PosortowanaTablica.insert(PosortowanaTablica.end(), NumerZadania);
        ZsumowaneWagi.erase(Iterator);
        NumerZadania = 0;
        MaxWartosc = 0;
    }

    string Lista1;
    string Lista2;
    string OstatecznaKombinacja;
    string Tymczasowy;
    uint32_t NajlepszeCmax = 300000000;
    char Pomocniczy[32];
    uint32_t Cmax;
    int32_t Pozycja;

    //int CzasRozpoczecia = GetTickCount();
    for(size_t i = 0; i < PosortowanaTablica.size(); i++)
    {
        Pozycja = 0;
        NajlepszeCmax = 300000000;
        for(size_t j = 0; j <= i; j++)
        {
            Lista1 = "";
            Lista1 = Lista2;

            if(j == 0)
                Pozycja = -1;
            else
                Pozycja = Lista1.find(',' , Pozycja+1);

            Tymczasowy = "";
            Tymczasowy = itoa(PosortowanaTablica[i] + 1, Pomocniczy, 10);
            Tymczasowy += ",";
            Lista1.insert(Pozycja+1, Tymczasowy);

            Cmax = MaxCzasWykonania(Lista1);

            if(NajlepszeCmax > Cmax)
            {
                NajlepszeCmax = Cmax;
                OstatecznaKombinacja = Lista1;
            }
        }
        Lista2 = OstatecznaKombinacja;
    }

    //cout<<"Czas potrzebny dla neh-a: "<<GetTickCount() - CzasRozpoczecia<<endl;

    cout<<"Najlepsza kombinacja "<<OstatecznaKombinacja<<endl;
    cout<<"Najlepsze Cmax "<<NajlepszeCmax<<endl;
    *NajlepszaKolejnosc = OstatecznaKombinacja;
    return NajlepszeCmax;
}

uint32_t ProblemPrzeplywowy::AkceleracjaNEH(string* NajlepszaKolejnosc)
{
    vector<DaneNEH> ZsumowaneWagi;

    for(uint32_t i = 0; i < IloscZadan; i++)
    {
        ZsumowaneWagi.push_back(DaneNEH(i, 0));
        for(uint32_t j = 0; j < IloscMaszyn; j++)
        {
            ZsumowaneWagi[i].Wartosc += DaneZadan[i][j];
        }
    }

    vector<uint32_t> PosortowanaTablica;
    uint32_t MaxWartosc = 0;
    uint32_t NumerZadania = 0;
    vector<DaneNEH>::iterator i = ZsumowaneWagi.begin();
    vector<DaneNEH>::iterator Iterator;

    while(!ZsumowaneWagi.empty())
    {
        i = ZsumowaneWagi.begin();
        for(; i != ZsumowaneWagi.end(); i++)
        {
            for(uint32_t j = 0; j < IloscMaszyn; j++)
            {
                if(i->Wartosc > MaxWartosc)
                {
                    MaxWartosc = i->Wartosc;
                    NumerZadania = i->NumerZadania;
                    Iterator = i;
                }
            }
        }

        PosortowanaTablica.insert(PosortowanaTablica.end(), NumerZadania);
        ZsumowaneWagi.erase(Iterator);
        NumerZadania = 0;
        MaxWartosc = 0;
    }

    uint32_t AktualneZadanie = 0;
    uint32_t AktualneZadanie1 = 0;
    uint32_t PoprzednieZadanie = 0;
    uint32_t PoprzednieZadanie1 = 0;
    uint32_t NastepneZadanie = 0;
    uint32_t NajlepszaPozycja = 0;
    uint32_t Cmax = 99999999;

    string Tymczasowy;
    string OstatecznaKombinacja = "";
    char Pomocnicza[32];
    int Pozycja = 0;
    int Pozycja1 = 0;

    uint32_t* SciezkaPom = new uint32_t [IloscMaszyn];
    for(uint32_t i = 0; i < IloscMaszyn; i++)
        SciezkaPom[i] = 0;

    uint32_t* WartoscMax = new uint32_t[IloscZadan+1];
    for(uint32_t i = 0; i <= IloscZadan; i++)
        WartoscMax[i] = 0;

    uint32_t** SciezkaD = new uint32_t*[IloscZadan];
    for(uint32_t i = 0; i < IloscZadan; i++)
        SciezkaD[i] = new uint32_t[IloscMaszyn];

    uint32_t** SciezkaW = new uint32_t*[IloscZadan];
    for(uint32_t i = 0; i < IloscZadan; i++)
        SciezkaW[i] = new uint32_t[IloscMaszyn];

    //int CzasRozpoczecia = GetTickCount();
    for(uint32_t i = 0; i < PosortowanaTablica.size(); i++)
    {
        AktualneZadanie = PosortowanaTablica[i];
        SciezkaD[AktualneZadanie][0] = DaneZadan[AktualneZadanie][0];
        SciezkaW[AktualneZadanie][IloscMaszyn-1] = DaneZadan[AktualneZadanie][IloscMaszyn-1];
        for(uint32_t k = 1; k < IloscMaszyn; k++)
        {
            SciezkaD[AktualneZadanie][k] = DaneZadan[AktualneZadanie][k] + SciezkaD[AktualneZadanie][k-1];
            SciezkaW[AktualneZadanie][IloscMaszyn-k-1] = DaneZadan[AktualneZadanie][IloscMaszyn-k-1] + SciezkaW[AktualneZadanie][IloscMaszyn-k] ;
        }

        if(i == 0)
        {
            OstatecznaKombinacja = itoa(AktualneZadanie+1, Pomocnicza, 10);
            OstatecznaKombinacja += ",";
        }else
        {
            for(uint32_t j = 0; j <= i; j++)
            {
                if(j == 0)
                {
                    Pozycja = OstatecznaKombinacja.find(",", 0);
                    NastepneZadanie = atoi(OstatecznaKombinacja.substr(0, Pozycja).c_str()) - 1;


                    for(uint32_t k = 0; k < IloscMaszyn; k++)
                    {

                        if(WartoscMax[0] < SciezkaD[AktualneZadanie][k]+SciezkaW[NastepneZadanie][k])
                        {
                            WartoscMax[0] = SciezkaD[AktualneZadanie][k]+SciezkaW[NastepneZadanie][k];
                        }

                    }

                }else if(j == i)
                {
                    Pozycja = OstatecznaKombinacja.length()-1;
                    Pozycja1 = OstatecznaKombinacja.length()-1;

                    Pozycja = OstatecznaKombinacja.rfind(",", Pozycja-1);

                    PoprzednieZadanie = atoi(OstatecznaKombinacja.substr(Pozycja+1, Pozycja1-Pozycja).c_str()) - 1;

                    for(uint32_t k = 0; k < IloscMaszyn; k++)
                    {
                        if(WartoscMax[i] < SciezkaD[PoprzednieZadanie][k]+SciezkaW[AktualneZadanie][k])
                        {
                            WartoscMax[i] = SciezkaD[PoprzednieZadanie][k]+SciezkaW[AktualneZadanie][k];
                        }
                    }

                }else
                {
                    Pozycja = 0;
                    Pozycja1 = 0;
                    for(uint32_t l = 0; l < j; l++)
                    {
                        Pozycja1 = Pozycja;
                        Pozycja = OstatecznaKombinacja.find(",", Pozycja+1);
                    }

                    if(j == 1) Pozycja1 = 0;
                    else Pozycja1 += 1;
                    PoprzednieZadanie = atoi(OstatecznaKombinacja.substr(Pozycja1, (Pozycja)-Pozycja1).c_str()) - 1;

                    Pozycja1 = Pozycja;
                    NastepneZadanie = atoi(OstatecznaKombinacja.substr(Pozycja1+1, (Pozycja-1)-Pozycja1).c_str()) - 1;

                    SciezkaPom[0] = SciezkaD[PoprzednieZadanie][0]+DaneZadan[AktualneZadanie][0];

                    if(WartoscMax[j] < SciezkaPom[0]+SciezkaW[NastepneZadanie][0])
                            WartoscMax[j] = SciezkaPom[0]+SciezkaW[NastepneZadanie][0];

                    for(uint32_t k = 1; k < IloscMaszyn; k++)
                    {

                        SciezkaPom[k] = DaneZadan[AktualneZadanie][k] + max(SciezkaD[PoprzednieZadanie][k], SciezkaPom[k-1]);

                        if(WartoscMax[j] < SciezkaPom[k] + SciezkaW[NastepneZadanie][k])
                        {
                            WartoscMax[j] = SciezkaPom[k] + SciezkaW[NastepneZadanie][k];
                        }
                    }
                }
            }



            Cmax = 999999999;
            for(uint32_t j = 0; j <= i; j++)
            {
                if(WartoscMax[j] < Cmax)
                {
                    Cmax = WartoscMax[j];
                    NajlepszaPozycja = j;
                }

                WartoscMax[j] = 0;
            }

            if(NajlepszaPozycja == 0)
            {
                Tymczasowy = "";
                Tymczasowy = itoa(AktualneZadanie+1, Pomocnicza, 10);
                Tymczasowy += ",";
                OstatecznaKombinacja.insert(0, Tymczasowy);

            }else
            {
                Pozycja = 0;
                for(uint32_t j = 0; j < NajlepszaPozycja; j++)
                {
                    Pozycja = OstatecznaKombinacja.find(",", Pozycja+1);
                }

                Tymczasowy = "";
                Tymczasowy = itoa(AktualneZadanie+1, Pomocnicza, 10);
                Tymczasowy += ",";

                OstatecznaKombinacja.insert(Pozycja+1, Tymczasowy);
            }

            Pozycja1 = 0;
            Pozycja = 0;

            for(uint32_t k = 0; k <=i; k++)
            {
                Pozycja = OstatecznaKombinacja.find(",", Pozycja+1);
                AktualneZadanie1 = atoi(OstatecznaKombinacja.substr(Pozycja1, Pozycja - Pozycja1).c_str()) - 1;

                if(k == 0)
                {
                    SciezkaD[AktualneZadanie1][0] = DaneZadan[AktualneZadanie1][0];
                    for(uint32_t j = 1; j < IloscMaszyn; j++)
                    {
                        SciezkaD[AktualneZadanie1][j] = DaneZadan[AktualneZadanie1][j] + SciezkaD[AktualneZadanie1][j-1];
                    }

                }else
                {
                    SciezkaD[AktualneZadanie1][0] = SciezkaD[PoprzednieZadanie1][0] + DaneZadan[AktualneZadanie1][0];
                    for(uint32_t j = 1; j < IloscMaszyn; j++)
                    {
                        SciezkaD[AktualneZadanie1][j] = DaneZadan[AktualneZadanie1][j] + max(SciezkaD[AktualneZadanie1][j-1], SciezkaD[PoprzednieZadanie1][j]);
                    }

                }

                PoprzednieZadanie1 = AktualneZadanie1;
                Pozycja1 = Pozycja+1;
            }

            Pozycja = OstatecznaKombinacja.length()-1;
            Pozycja1 = OstatecznaKombinacja.length()-1;


            for(uint32_t k = 0; k <= i; k++)
            {

                Pozycja = OstatecznaKombinacja.rfind(",", Pozycja-1);

                AktualneZadanie1 = atoi(OstatecznaKombinacja.substr(Pozycja+1, Pozycja1-Pozycja).c_str()) - 1;


                if(k == 0)
                {
                    SciezkaW[AktualneZadanie1][IloscMaszyn-1] = DaneZadan[AktualneZadanie1][IloscMaszyn-1];
                    for(int j = IloscMaszyn-2; j >= 0; j--)
                    {
                        SciezkaW[AktualneZadanie1][j] = DaneZadan[AktualneZadanie1][j] + SciezkaW[AktualneZadanie1][j+1];
                    }

                }else
                {
                    SciezkaW[AktualneZadanie1][IloscMaszyn-1] = SciezkaW[PoprzednieZadanie1][IloscMaszyn-1] + DaneZadan[AktualneZadanie1][IloscMaszyn-1];
                    for(int j = IloscMaszyn-2; j >= 0; j--)
                    {
                        SciezkaW[AktualneZadanie1][j] = DaneZadan[AktualneZadanie1][j] + max(SciezkaW[AktualneZadanie1][j+1], SciezkaW[PoprzednieZadanie1][j]);
                    }

                }


                PoprzednieZadanie1 = AktualneZadanie1;
                Pozycja1 = Pozycja;
            }
       /*     if(i>= 8 && i < 9)
            for(uint32_t k = 0; k < IloscZadan; k++)
                for(uint32_t j = 0; j < IloscMaszyn; j++)
                {
                    cout<<"Sciezka od "<<k<<" "<<j<<" = "<<SciezkaW[k][j]<<endl;
                }
*/

        }
        PoprzednieZadanie = AktualneZadanie;
    }

    //cout<<"Czas potrzebny dla akceleracji: "<<GetTickCount() - CzasRozpoczecia<<endl;

    for(uint32_t i = 0; i < IloscZadan; i++)
    {
        delete[] SciezkaD[i];
        delete[] SciezkaW[i];
    }

    delete[] SciezkaD;
    delete[] SciezkaW;

    delete[] WartoscMax;

    cout<<"Cmax: "<<Cmax<<endl;
    cout<<"Kombinacja: "<<OstatecznaKombinacja<<endl;

    *NajlepszaKolejnosc = OstatecznaKombinacja;
    return Cmax;
}

uint32_t ProblemPrzeplywowy::ZmodyfikowanyNEH(string* NajlepszaKolejnosc)
{
    vector<DaneNEH> ZsumowaneWagi;

    for(uint32_t i = 0; i < IloscZadan; i++)
    {
        ZsumowaneWagi.push_back(DaneNEH(i, 0));
        for(uint32_t j = 0; j < IloscMaszyn; j++)
        {
            ZsumowaneWagi[i].Wartosc += DaneZadan[i][j];
        }
    }

    vector<uint32_t> PosortowanaTablica;
    uint32_t MaxWartosc = 0;
    uint32_t NumerZadania = 0;
    vector<DaneNEH>::iterator i = ZsumowaneWagi.begin();
    vector<DaneNEH>::iterator Iterator;

    while(!ZsumowaneWagi.empty())
    {
        i = ZsumowaneWagi.begin();
        for(; i != ZsumowaneWagi.end(); i++)
        {
            for(uint32_t j = 0; j < IloscMaszyn; j++)
            {
                if(i->Wartosc > MaxWartosc)
                {
                    MaxWartosc = i->Wartosc;
                    NumerZadania = i->NumerZadania;
                    Iterator = i;
                }
            }
        }

        PosortowanaTablica.insert(PosortowanaTablica.end(), NumerZadania);
        ZsumowaneWagi.erase(Iterator);
        NumerZadania = 0;
        MaxWartosc = 0;
    }

    string Lista1;
    string Lista2 = "";
    string OstatecznaKombinacja;
    string Tymczasowy;
    uint32_t NajlepszeCmax = 300000000;
    char Pomocniczy[32];
    uint32_t Cmax;
    int32_t Pozycja;
    int32_t Pozycja1;
    int32_t UsuwanaWartosc = 0;
    int32_t NajgorszaWartosc = 0;
    size_t PozycjaNajgorszejWartosci = 0;
    int32_t IleUsunac = 0;

    for(size_t i = 0; i < PosortowanaTablica.size(); i++)
    {
        Pozycja = 0;
        NajlepszeCmax = 300000000;
        for(size_t j = 0; j <= i; j++)
        {
            Lista1 = "";
            Lista1 = Lista2;

            if(j == 0)
                Pozycja = -1;
            else
                Pozycja = Lista1.find(',' , Pozycja+1);

            Tymczasowy = "";
            Tymczasowy = itoa(PosortowanaTablica[i] + 1, Pomocniczy, 10);
            Tymczasowy += ",";
            Lista1.insert(Pozycja+1, Tymczasowy);

            Cmax = MaxCzasWykonania(Lista1);

            if(NajlepszeCmax > Cmax)
            {
                NajlepszeCmax = Cmax;
                OstatecznaKombinacja = Lista1;
            }
        }
        Lista2 = OstatecznaKombinacja;

        Pozycja = 0;
        Pozycja1 = 0;
        NajlepszeCmax = 300000000;

        if(i > 0)
        {
            for(size_t j = 0; j <= i; j++)
            {
                Pozycja = Lista2.find(",", Pozycja+1);

                if((PosortowanaTablica[i] + 1) != (UsuwanaWartosc = atoi(Lista2.substr(Pozycja1, Pozycja - Pozycja1).c_str())))
                {
                    Lista2.erase(Pozycja1, Pozycja+1 - Pozycja1);

                    Cmax = MaxCzasWykonania(Lista2);

                    if(NajlepszeCmax > Cmax)
                    {
                        NajlepszeCmax = Cmax;
                        NajgorszaWartosc = UsuwanaWartosc;
                        PozycjaNajgorszejWartosci = Pozycja1;
                        IleUsunac = Pozycja+1 - Pozycja1;
                    }
                    Tymczasowy = "";
                    Tymczasowy = itoa(UsuwanaWartosc, Pomocniczy, 10);
                    Tymczasowy += ",";
                    Lista2.insert(Pozycja1, Tymczasowy);
                }
                Pozycja1 = Pozycja+1;

            }

            Lista2.erase(PozycjaNajgorszejWartosci, IleUsunac);

            Pozycja = 0;
            NajlepszeCmax = 300000000;
            for(size_t j = 0; j <= i; j++)
            {
                Lista1 = "";
                Lista1 = Lista2;

                if(j == 0)
                    Pozycja = -1;
                else
                    Pozycja = Lista1.find(',' , Pozycja+1);

                Tymczasowy = "";
                Tymczasowy = itoa(NajgorszaWartosc, Pomocniczy, 10);
                Tymczasowy += ",";
                Lista1.insert(Pozycja+1, Tymczasowy);

                Cmax = MaxCzasWykonania(Lista1);

                if(NajlepszeCmax > Cmax)
                {
                    NajlepszeCmax = Cmax;
                    OstatecznaKombinacja = Lista1;
                }
            }
            Lista2 = OstatecznaKombinacja;
        }
    }

    cout<<"Najlepsza kombinacja "<<OstatecznaKombinacja<<endl;
    cout<<"Najlepsze Cmax "<<NajlepszeCmax<<endl;
    *NajlepszaKolejnosc = OstatecznaKombinacja;
    return NajlepszeCmax;
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

