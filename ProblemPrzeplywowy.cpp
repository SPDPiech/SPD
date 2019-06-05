#include "ProblemPrzeplywowy.h"

/* A utility function to reverse a string  */
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap(*(str+start), *(str+end));
        start++;
        end--;
    }
}

char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}


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

uint32_t ProblemPrzeplywowy::MaxCzasRPQ(vector<uint32_t> Kolejnosc)
{
    vector<uint32_t>S;
    uint32_t j = 0;
    vector<uint32_t> Cmax;

    for(; j < Kolejnosc.size(); j++)
    {
        if(j == 0)
        {
            S.push_back(DaneZadan[Kolejnosc[j]][0]);


        }else
        {
            S.push_back(max(DaneZadan[Kolejnosc[j]][0], S[j-1]+DaneZadan[Kolejnosc[j-1]][1]));
        }

        Cmax.push_back(S[j] + DaneZadan[Kolejnosc[j]][1]);
    }

    uint32_t MaxWartosc = 0;

    for(size_t i = 0; i < Cmax.size(); i++)
    {
        if((Cmax[i] + DaneZadan[Kolejnosc[i]][2]) > MaxWartosc)
            MaxWartosc = Cmax[i] + DaneZadan[Kolejnosc[i]][2];
    }

    return MaxWartosc;

}

//Wczytywanie danych z pliku
void ProblemPrzeplywowy::WczytajDane(string NazwaPliku)
{

    IloscMaszyn=3;
    fstream plik(NazwaPliku.c_str(), ios::in);
    if(plik.good())
    {
        if(DaneZadan == NULL)
        {
            plik>>IloscZadan;

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

    //cout<<"Cmax: "<<Cmax<<endl;
    //cout<<"Kombinacja: "<<OstatecznaKombinacja<<endl;

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

uint32_t ProblemPrzeplywowy::AlgorytmWyzarzaniaNEH(string* NajlepszaKolejnosc, float PoczatkowaTemperatura, float ParametrChlodzenia, float TemperaturaGraniczna, bool UzyjSWAP)
{
    if(TemperaturaGraniczna > PoczatkowaTemperatura)
    {
        cout<<"Niewlasciwa temperatura graniczna"<<endl;
        return 0;
    }

    string BiezacaKolejnosc;
    string TestowanaKolejnosc;
    string UsuwanaWartosc1;
    string UsuwanaWartosc2;

    float Temperatura;
    double Prawdopodobienstwo;
    double RandPraw;
    uint32_t CmaxPi, CmaxPiPrim;
    uint32_t NumerZadania1;
    uint32_t NumerZadania2;

    uint32_t Pozycja = 0;
    uint32_t Pozycja1  = 0;


    Temperatura = PoczatkowaTemperatura;
    CmaxPi = AkceleracjaNEH(&BiezacaKolejnosc);
    TestowanaKolejnosc = BiezacaKolejnosc;

    do
    {

        //Odkomentowac petle do while dla podpunktu 7 badan !!!
        //do{
        if(UzyjSWAP == true)
        {

            NumerZadania1 = (rand()%IloscZadan);

            do
            {
                NumerZadania2 = (rand()%IloscZadan);

            }while(NumerZadania1 == NumerZadania2);

            Pozycja1 = 0;
            Pozycja = 0;

            for(uint32_t i = 0; i <= NumerZadania1; i++)
            {
                Pozycja = TestowanaKolejnosc.find(",", Pozycja+1);
                UsuwanaWartosc1 = TestowanaKolejnosc.substr(Pozycja1, Pozycja - Pozycja1).c_str();

                if(i < NumerZadania1)
                    Pozycja1 = Pozycja+1;
            }

            TestowanaKolejnosc.erase(Pozycja1, Pozycja+1 - Pozycja1);

            if(NumerZadania1 < NumerZadania2) NumerZadania2 -= 1;

            Pozycja1 = 0;
            Pozycja = 0;

            for(uint32_t i = 0; i <= NumerZadania2; i++)
            {
                Pozycja = TestowanaKolejnosc.find(",", Pozycja+1);
                UsuwanaWartosc2 = TestowanaKolejnosc.substr(Pozycja1, Pozycja - Pozycja1).c_str();

                if(i < NumerZadania2)
                    Pozycja1 = Pozycja+1;
            }

            TestowanaKolejnosc.erase(Pozycja1, Pozycja+1 - Pozycja1);

            Pozycja1 = 0;
            Pozycja = 0;

            for(uint32_t i = 0; i < NumerZadania2; i++)
            {
                Pozycja = TestowanaKolejnosc.find(",", Pozycja+1);
            }

            UsuwanaWartosc1 += ",";
            if(NumerZadania2 == 0)
                TestowanaKolejnosc.insert(0, UsuwanaWartosc1);
            else
                TestowanaKolejnosc.insert(Pozycja+1, UsuwanaWartosc1);

            Pozycja1 = 0;
            Pozycja = 0;

            for(uint32_t i = 0; i < NumerZadania1; i++)
            {
                Pozycja = TestowanaKolejnosc.find(",", Pozycja+1);
            }

            UsuwanaWartosc2 += ",";
            if(NumerZadania1 == 0)
                TestowanaKolejnosc.insert(0, UsuwanaWartosc2);
            else
                TestowanaKolejnosc.insert(Pozycja+1, UsuwanaWartosc2);

        }else
        {
            NumerZadania1 = (rand()%IloscZadan);

            do
            {
                NumerZadania2 = (rand()%(IloscZadan+1));

            }while(NumerZadania1 == NumerZadania2);

            Pozycja1 = 0;
            Pozycja = 0;

            for(uint32_t i = 0; i <= NumerZadania1; i++)
            {
                Pozycja = TestowanaKolejnosc.find(",", Pozycja+1);
                UsuwanaWartosc1 = TestowanaKolejnosc.substr(Pozycja1, Pozycja - Pozycja1).c_str();

                if(i < NumerZadania1)
                    Pozycja1 = Pozycja+1;
            }

            TestowanaKolejnosc.erase(Pozycja1, Pozycja+1 - Pozycja1);

            if(NumerZadania1 < NumerZadania2) NumerZadania2 -= 1;

            Pozycja1 = 0;
            Pozycja = 0;

            for(uint32_t i = 0; i < NumerZadania2; i++)
            {
                Pozycja = TestowanaKolejnosc.find(",", Pozycja+1);
            }

            UsuwanaWartosc1 += ",";
            if(NumerZadania2 == 0)
                TestowanaKolejnosc.insert(0, UsuwanaWartosc1);
            else
                TestowanaKolejnosc.insert(Pozycja+1, UsuwanaWartosc1);

        }


        CmaxPi = MaxCzasWykonania(BiezacaKolejnosc);
        CmaxPiPrim = MaxCzasWykonania(TestowanaKolejnosc);

        //Petla do odkomentowania dla podpunktu 7
        //}while(CmaxPi == CmaxPiPrim)


        //W 6 podpunkcie uzyc zamiast if tylko :
        //Prawdopodobienstwo = exp(((int)CmaxPi - (int)CmaxPiPrim)/Temperatura);

        if(CmaxPiPrim >= CmaxPi)
            Prawdopodobienstwo = exp(((int)CmaxPi - (int)CmaxPiPrim)/Temperatura);
        else
            Prawdopodobienstwo = 1;

        RandPraw = ((rand()%101)/100.0f);

        if(Prawdopodobienstwo >= RandPraw)
        {
            BiezacaKolejnosc = TestowanaKolejnosc;

        }else
        {
            Temperatura = ParametrChlodzenia*Temperatura;
        }

    //cout<<"Tem :"<<Temperatura<<endl;
    }while(Temperatura > TemperaturaGraniczna);

    *NajlepszaKolejnosc = BiezacaKolejnosc;
    return MaxCzasWykonania(BiezacaKolejnosc);

}

uint32_t ProblemPrzeplywowy::AlgorytmWyzarzaniaLosowy(string* NajlepszaKolejnosc, float PoczatkowaTemperatura, float ParametrChlodzenia, float TemperaturaGraniczna, bool UzyjSWAP)
{
    if(TemperaturaGraniczna > PoczatkowaTemperatura)
    {
        cout<<"Niewlasciwa temperatura graniczna"<<endl;
        return 0;
    }

    string BiezacaKolejnosc;
    string TestowanaKolejnosc;
    string UsuwanaWartosc1;
    string UsuwanaWartosc2;
    string Pomocniczy = "";

    char Pom[32];
    int Losowy;

    float Temperatura;
    double Prawdopodobienstwo;
    double RandPraw;
    uint32_t CmaxPi, CmaxPiPrim;
    uint32_t NumerZadania1;
    uint32_t NumerZadania2;

    uint32_t Pozycja = 0;
    uint32_t Pozycja1  = 0;
    uint32_t k;

    for(uint32_t i = 0; i < IloscZadan; i++)
    {
        Pomocniczy += itoa(i+1, Pom, 10);
        Pomocniczy += ",";
    }

    k = 0;
    while(!Pomocniczy.empty())
    {
        k += 1;
        Losowy = (rand()%(IloscZadan-k+1));

        Pozycja1 = 0;
        Pozycja = 0;

        for(uint32_t i = 0; i <= Losowy; i++)
        {
            Pozycja = Pomocniczy.find(",", Pozycja+1);
            UsuwanaWartosc1 = Pomocniczy.substr(Pozycja1, Pozycja - Pozycja1).c_str();

            if(i < Losowy)
                Pozycja1 = Pozycja+1;
        }

        Pomocniczy.erase(Pozycja1, Pozycja+1 - Pozycja1);
        BiezacaKolejnosc += UsuwanaWartosc1;
        BiezacaKolejnosc += ",";
    }


    Temperatura = PoczatkowaTemperatura;
    CmaxPi = MaxCzasWykonania(BiezacaKolejnosc);
    TestowanaKolejnosc = BiezacaKolejnosc;


    do
    {

        //Odkomentowac petle do while dla podpunktu 7 badan !!!
        //do{
        if(UzyjSWAP == true)
        {
            NumerZadania1 = (rand()%IloscZadan);

            do
            {
                NumerZadania2 = (rand()%IloscZadan);

            }while(NumerZadania1 == NumerZadania2);

            Pozycja1 = 0;
            Pozycja = 0;

            for(uint32_t i = 0; i <= NumerZadania1; i++)
            {
                Pozycja = TestowanaKolejnosc.find(",", Pozycja+1);
                UsuwanaWartosc1 = TestowanaKolejnosc.substr(Pozycja1, Pozycja - Pozycja1).c_str();

                if(i < NumerZadania1)
                    Pozycja1 = Pozycja+1;
            }

            TestowanaKolejnosc.erase(Pozycja1, Pozycja+1 - Pozycja1);

            if(NumerZadania1 < NumerZadania2) NumerZadania2 -= 1;

            Pozycja1 = 0;
            Pozycja = 0;

            for(uint32_t i = 0; i <= NumerZadania2; i++)
            {
                Pozycja = TestowanaKolejnosc.find(",", Pozycja+1);
                UsuwanaWartosc2 = TestowanaKolejnosc.substr(Pozycja1, Pozycja - Pozycja1).c_str();

                if(i < NumerZadania2)
                    Pozycja1 = Pozycja+1;
            }

            TestowanaKolejnosc.erase(Pozycja1, Pozycja+1 - Pozycja1);

            Pozycja1 = 0;
            Pozycja = 0;

            for(uint32_t i = 0; i < NumerZadania2; i++)
            {
                Pozycja = TestowanaKolejnosc.find(",", Pozycja+1);
            }

            UsuwanaWartosc1 += ",";
            if(NumerZadania2 == 0)
                TestowanaKolejnosc.insert(0, UsuwanaWartosc1);
            else
                TestowanaKolejnosc.insert(Pozycja+1, UsuwanaWartosc1);

            Pozycja1 = 0;
            Pozycja = 0;

            for(uint32_t i = 0; i < NumerZadania1; i++)
            {
                Pozycja = TestowanaKolejnosc.find(",", Pozycja+1);
            }

            UsuwanaWartosc2 += ",";
            if(NumerZadania1 == 0)
                TestowanaKolejnosc.insert(0, UsuwanaWartosc2);
            else
                TestowanaKolejnosc.insert(Pozycja+1, UsuwanaWartosc2);

        }else
        {
            NumerZadania1 = (rand()%IloscZadan);

            do
            {
                NumerZadania2 = (rand()%(IloscZadan+1));

            }while(NumerZadania1 == NumerZadania2);

            Pozycja1 = 0;
            Pozycja = 0;

            for(uint32_t i = 0; i <= NumerZadania1; i++)
            {
                Pozycja = TestowanaKolejnosc.find(",", Pozycja+1);
                UsuwanaWartosc1 = TestowanaKolejnosc.substr(Pozycja1, Pozycja - Pozycja1).c_str();

                if(i < NumerZadania1)
                    Pozycja1 = Pozycja+1;
            }

            TestowanaKolejnosc.erase(Pozycja1, Pozycja+1 - Pozycja1);

            if(NumerZadania1 < NumerZadania2) NumerZadania2 -= 1;

            Pozycja1 = 0;
            Pozycja = 0;

            for(uint32_t i = 0; i < NumerZadania2; i++)
            {
                Pozycja = TestowanaKolejnosc.find(",", Pozycja+1);
            }

            UsuwanaWartosc1 += ",";
            if(NumerZadania2 == 0)
                TestowanaKolejnosc.insert(0, UsuwanaWartosc1);
            else
                TestowanaKolejnosc.insert(Pozycja+1, UsuwanaWartosc1);

        }


        CmaxPi = MaxCzasWykonania(BiezacaKolejnosc);
        CmaxPiPrim = MaxCzasWykonania(TestowanaKolejnosc);

        //Petla do odkomentowania dla podpunktu 7
        //}while(CmaxPi == CmaxPiPrim)

        //W 6 podpunkcie uzyc zamiast if tylko :
        //Prawdopodobienstwo = exp(((int)CmaxPi - (int)CmaxPiPrim)/Temperatura);

        if(CmaxPiPrim >= CmaxPi)
            Prawdopodobienstwo = exp(((int)CmaxPi - (int)CmaxPiPrim)/Temperatura);
        else
            Prawdopodobienstwo = 1;


        RandPraw = ((rand()%101)/100.0f);

        if(Prawdopodobienstwo >= RandPraw)
        {
            BiezacaKolejnosc = TestowanaKolejnosc;

        }else
        {
            Temperatura = ParametrChlodzenia*Temperatura;
        }

    //cout<<"Tem :"<<Temperatura<<endl;
    }while(Temperatura > TemperaturaGraniczna);

    *NajlepszaKolejnosc = BiezacaKolejnosc;
    return MaxCzasWykonania(BiezacaKolejnosc);
}

uint32_t ProblemPrzeplywowy::Schrage(string* NajlepszaKolejnosc)
{
    vector<DaneSchrage> ListaZadan;

    for(uint32_t i = 0; i < IloscZadan; i++)
    {
        ListaZadan.push_back(DaneSchrage(i, DaneZadan[i][0], DaneZadan[i][1], DaneZadan[i][2]));
    }

    vector<uint32_t> PosortowanaTablica;
    uint32_t MinWartosc = 2222222222;
    uint32_t NumerZadania = 0;
    vector<DaneSchrage>::iterator ite = ListaZadan.begin();
    vector<DaneSchrage>::iterator Iterator;

    while(!ListaZadan.empty())
    {
        ite = ListaZadan.begin();
        for(; ite != ListaZadan.end(); ite++)
        {
            if(ite->r < MinWartosc)
            {
                MinWartosc = ite->r;
                NumerZadania = ite->NumerZadania;
                Iterator = ite;

            }
        }

        PosortowanaTablica.insert(PosortowanaTablica.end(), NumerZadania);
        ListaZadan.erase(Iterator);
        NumerZadania = 0;
        MinWartosc = 2222222222;
    }

/*
    for(int k = 0; k < PosortowanaTablica.size(); k++)
    {
        cout<<PosortowanaTablica[k]<<endl;
    }
*/

    uint32_t i = 1;
    uint32_t j = 0;
    vector<uint32_t> Kolejnosc;
    vector<uint32_t> GotoweZadania;
    uint32_t t = DaneZadan[PosortowanaTablica[0]][0];

    int64_t MaxWartosc = -1;
    NumerZadania = 0;
    vector<uint32_t>::iterator Iterator1;
    vector<uint32_t>::iterator Iterator2;

    while(!GotoweZadania.empty() || !PosortowanaTablica.empty())
    {
        while(!PosortowanaTablica.empty() && DaneZadan[PosortowanaTablica[0]][0] <= t)
        {
            //cout<<"DaneZadan[PosortowanaTablica[0]][0]: "<<DaneZadan[PosortowanaTablica[0]][0]<<endl;
            GotoweZadania.push_back(PosortowanaTablica[0]);
            PosortowanaTablica.erase(PosortowanaTablica.begin());

        }

        if(GotoweZadania.empty())
        {
            t = DaneZadan[PosortowanaTablica[0]][0];

        }else
        {
            Iterator1 = GotoweZadania.begin();
            MaxWartosc = -1;
            for(; Iterator1 != GotoweZadania.end(); Iterator1++)
            {
                if(DaneZadan[*Iterator1][2] > MaxWartosc)
                {
                    //cout<<"ite1 :"<<*Iterator1<<endl;
                    MaxWartosc = DaneZadan[*Iterator1][2];
                    NumerZadania = *Iterator1;
                    Iterator2 = Iterator1;
                }
            }

            Kolejnosc.push_back(*Iterator2);
            i += 1;

            t += DaneZadan[*Iterator2][1];

            GotoweZadania.erase(Iterator2);
        }

        //Sleep(10);

    }

    *NajlepszaKolejnosc = "";
    char Pomocniczy[32];

    for(int k = 0; k < Kolejnosc.size(); k++)
    {
        NajlepszaKolejnosc->append(itoa(Kolejnosc[k], Pomocniczy, 10));
        NajlepszaKolejnosc->append(",");
        //cout<<Kolejnosc[k]+1<<" ";
    }
    //cout<<endl;

    return MaxCzasRPQ(Kolejnosc);
}

uint32_t ProblemPrzeplywowy::SchrageZPodzialem()
{
    vector<DaneSchrage> ListaZadan;

    for(uint32_t i = 0; i < IloscZadan; i++)
    {
        ListaZadan.push_back(DaneSchrage(i, DaneZadan[i][0], DaneZadan[i][1], DaneZadan[i][2]));
    }

    vector<uint32_t> PosortowanaTablica;
    uint32_t MinWartosc = 2222222222;
    uint32_t NumerZadania = 0;
    vector<DaneSchrage>::iterator ite = ListaZadan.begin();
    vector<DaneSchrage>::iterator Iterator;

    while(!ListaZadan.empty())
    {
        ite = ListaZadan.begin();
        for(; ite != ListaZadan.end(); ite++)
        {
            if(ite->r < MinWartosc)
            {
                MinWartosc = ite->r;
                NumerZadania = ite->NumerZadania;
                Iterator = ite;

            }
        }

        PosortowanaTablica.insert(PosortowanaTablica.end(), NumerZadania);
        ListaZadan.erase(Iterator);
        NumerZadania = 0;
        MinWartosc = 2222222222;
    }

/*
    for(int k = 0; k < PosortowanaTablica.size(); k++)
    {
        cout<<PosortowanaTablica[k]<<endl;
    }
*/

    uint32_t j = 0;
    vector<uint32_t> Kolejnosc;
    vector<uint32_t> GotoweZadania;
    uint32_t t = 0;
    uint32_t l = 0;
    uint32_t Cmax = 0;

    for(uint32_t i = 0; i < IloscZadan; i++)
    {
        ListaZadan.push_back(DaneSchrage(i, DaneZadan[i][0], DaneZadan[i][1], DaneZadan[i][2]));
    }

    ListaZadan[0].q = 411111111;
    int64_t MaxWartosc = -1;
    NumerZadania = 0;
    vector<uint32_t>::iterator Iterator1;
    vector<uint32_t>::iterator Iterator2;

    while(!GotoweZadania.empty() || !PosortowanaTablica.empty())
    {
        while(!PosortowanaTablica.empty() && DaneZadan[PosortowanaTablica[0]][0] <= t)
        {
            GotoweZadania.push_back(PosortowanaTablica[0]);
            PosortowanaTablica.erase(PosortowanaTablica.begin());

            if(ListaZadan[*(GotoweZadania.end()-1)].q > ListaZadan[l].q)
            {
                ListaZadan[l].p = t - DaneZadan[*(GotoweZadania.end()-1)][0];
                t = DaneZadan[*(GotoweZadania.end()-1)][0];

                if(ListaZadan[l].p > 0)
                    GotoweZadania.push_back(l);
            }

        }

        if(GotoweZadania.empty())
        {
            t = DaneZadan[PosortowanaTablica[0]][0];

        }else
        {
            Iterator1 = GotoweZadania.begin();
            MaxWartosc = -1;
            for(; Iterator1 != GotoweZadania.end(); Iterator1++)
            {
                if(DaneZadan[*Iterator1][2] > MaxWartosc)
                {
                    //cout<<"ite1 :"<<*Iterator1<<endl;
                    MaxWartosc = DaneZadan[*Iterator1][2];
                    NumerZadania = *Iterator1;
                    Iterator2 = Iterator1;
                }
            }

            l = *Iterator2;
            t += ListaZadan[*Iterator2].p;
            Cmax =  max(Cmax, t + DaneZadan[*Iterator2][2]);
            GotoweZadania.erase(Iterator2);
        }
        //Sleep(10);
    }

    return Cmax;

}

uint32_t ProblemPrzeplywowy::ShrageNaKopcu(string* NajlepszaKolejnosc)
{
    vector<DaneSchrage> ListaZadan;

    for(uint32_t i = 0; i < IloscZadan; i++)
    {
        ListaZadan.push_back(DaneSchrage(i+1, DaneZadan[i][0], DaneZadan[i][1], DaneZadan[i][2]));
    }

    KopiecMinR.TworzKopiecMinR(IloscZadan, &ListaZadan[0]);
    KopiecMaxQ.TworzKopiecMaxQ(IloscZadan, NULL);


    uint32_t i = 1;
    uint32_t j = 0;
    vector<uint32_t> Kolejnosc;
    DaneSchrage Tymczasowe = KopiecMinR.SciagniZKopcaMinR();
    uint32_t t = Tymczasowe.r;

    DaneSchrage Tymczasowe1;

    while(!KopiecMaxQ.PustyKopiec() || !KopiecMinR.PustyKopiec())
    {

        while(Tymczasowe.NumerZadania != 0 && Tymczasowe.r <= t)
        {
            KopiecMaxQ.DodajDoKopcaMaxQ(Tymczasowe);
            Tymczasowe = KopiecMinR.SciagniZKopcaMinR();

        }

        if(KopiecMaxQ.PustyKopiec())
        {
            t = Tymczasowe.r;
            KopiecMaxQ.DodajDoKopcaMaxQ(Tymczasowe);
            Tymczasowe = KopiecMinR.SciagniZKopcaMinR();

        }else
        {
            Tymczasowe1 = KopiecMaxQ.SciagniZKopcaMaxQ();
            Kolejnosc.push_back(Tymczasowe1.NumerZadania-1);

            i += 1;

            t += Tymczasowe1.p;
        }
        //Sleep(10);
    }

    *NajlepszaKolejnosc = "";
    char Pomocniczy[32];

    for(int k = 0; k < Kolejnosc.size(); k++)
    {
        NajlepszaKolejnosc->append(itoa(Kolejnosc[k], Pomocniczy, 10));
        NajlepszaKolejnosc->append(",");
       // cout<<Kolejnosc[k]+1<<" ";
    }
    //cout<<endl;

    return MaxCzasRPQ(Kolejnosc);

}


uint32_t ProblemPrzeplywowy::ShrageZPodzialemNaKopcu()
{
    vector<DaneSchrage> ListaZadan;

    for(uint32_t i = 0; i < IloscZadan; i++)
    {
        ListaZadan.push_back(DaneSchrage(i+1, DaneZadan[i][0], DaneZadan[i][1], DaneZadan[i][2]));
    }

    KopiecMinR.TworzKopiecMinR(IloscZadan, &ListaZadan[0]);
    KopiecMaxQ.TworzKopiecMaxQ(IloscZadan, NULL);


    DaneSchrage Tymczasowe = KopiecMinR.SciagniZKopcaMinR();

    uint32_t t = 0;
    uint32_t l = 0;
    uint32_t Cmax = 0;
    DaneSchrage Tymczasowe1;

    ListaZadan[0].q = 411111111;

    while(!KopiecMaxQ.PustyKopiec() || !KopiecMinR.PustyKopiec())
    {
        while(Tymczasowe.NumerZadania != 0 && Tymczasowe.r <= t)
        {
            KopiecMaxQ.DodajDoKopcaMaxQ(Tymczasowe);


            if(ListaZadan[Tymczasowe.NumerZadania-1].q > ListaZadan[l].q)
            {
                ListaZadan[l].p = t - Tymczasowe.r;
                t = Tymczasowe.r;

                if(ListaZadan[l].p > 0)
                    KopiecMaxQ.DodajDoKopcaMaxQ(ListaZadan[l]);
            }

            Tymczasowe = KopiecMinR.SciagniZKopcaMinR();

        }

        if(KopiecMaxQ.PustyKopiec())
        {
            t = Tymczasowe.r;
            KopiecMaxQ.DodajDoKopcaMaxQ(Tymczasowe);
            Tymczasowe = KopiecMinR.SciagniZKopcaMinR();

        }else
        {
            Tymczasowe1 = KopiecMaxQ.SciagniZKopcaMaxQ();

            l = Tymczasowe1.NumerZadania-1;
            t += ListaZadan[Tymczasowe1.NumerZadania-1].p;
            Cmax =  max(Cmax, t + Tymczasowe1.q);

        }
        //Sleep(10);

    }



    return Cmax;

}

void ProblemPrzeplywowy::Compare(string file)
{

    string linia;
    fstream plik;
    ofstream zapis,czas;
    size_t position;
    int _tasks, _machines=3;
    int shrage_kopiec, shrageptmn,shrage, shrageptmn_kopiec;
    time_t start1,start2, stop1, stop2;
    double t_shrage, t_shrageptmn, t_shrage2, t_shrageptmn2;
    string order;

    cout<<"_______________________________________________________________________"<<endl;
    cout.width(27);
    cout<<left<<"Ilosc zadan";
    cout.width(27);
    cout<<left<<"Shrage O(n^2) (bez podz./z podz.)";
    cout<<left<<"Shrage kopiec (bez podz./z podz.)"<<endl;


    plik.open(file.c_str(), ios::in);
    zapis.open("zapis.txt");
    czas.open("czas.txt");

        while(!plik.eof())
        {


            getline(plik, linia);
            position=linia.find("data");
            if(position!=string::npos)
            {


                //cout << linia << endl; //wyœwietlenie linii
                plik>>_tasks;
                if (DaneZadan==NULL)
                    {
                        IloscZadan=_tasks;
                        IloscMaszyn=_machines;

                        DaneZadan  = new uint32_t*[IloscZadan];
                        for(int i=0; i<IloscZadan; i++)
                            DaneZadan[i]= new uint32_t[IloscMaszyn];

                        for(int i=0; i<IloscZadan; i++)
                        for(int j=0; j<IloscMaszyn; j++)
                            plik>>DaneZadan[i][j];

                        //cout<<tasks<<" "<<machines<<endl;

                    }
                else
                {


                    for(int i=0; i<IloscZadan; i++)
                    delete[] DaneZadan[i];

                    delete[] DaneZadan;

                    IloscZadan=_tasks;
                    IloscMaszyn=_machines;

                    DaneZadan = new uint32_t*[IloscZadan];
                    for(int i=0; i<IloscZadan; i++)
                        DaneZadan[i]= new uint32_t[IloscMaszyn];

                    for(int i=0; i<IloscZadan; i++)
                        for(int j=0; j<IloscMaszyn; j++)
                            plik>>DaneZadan[i][j];
                    }
                    //PrintData();
                    auto start4 = std::chrono::system_clock::now();
                    shrage=Schrage(&order);
                    auto stop4 = std::chrono::system_clock::now();
                    std::chrono::duration<double>t_shrage =stop4-start4;



                    auto start1 = std::chrono::system_clock::now();
                    shrageptmn=SchrageZPodzialem();
                    auto stop1= std::chrono::system_clock::now();
                    std::chrono::duration<double>t_shrageptmn =stop1-start1;

                    auto start=std::chrono::system_clock::now();
                    shrage_kopiec=ShrageNaKopcu(&order);
                    auto stop=std::chrono::system_clock::now();
                    std::chrono::duration<double>t_shrage2 =stop-start;

                    auto start3=std::chrono::system_clock::now();
                    shrageptmn_kopiec=ShrageZPodzialemNaKopcu();
                    auto stop3=std::chrono::system_clock::now();
                    std::chrono::duration<double>t_shrageptmn2 =stop-start;

                    zapis<<IloscZadan<<"\t"<<shrage<<"\t"<<shrageptmn<<"\t"<<shrage_kopiec<<"\t"<<shrageptmn_kopiec<<endl;
                    czas<<IloscZadan<<"\t"<<t_shrage.count()<<"\t"<<t_shrageptmn.count()<<"\t"<<t_shrage2.count()<<"\t"<<t_shrageptmn2.count()<<endl;

                    cout.width(27);
                    cout<<left<<IloscZadan;
                    cout.width(12);
                    cout<<left<<shrage;
                    cout.width(12);
                    cout<<left<<setprecision(8)<<shrageptmn;
                    cout.width(12);
                    cout<<left<<shrage_kopiec;
                    cout.width(12);
                    cout<<setprecision(8)<<shrageptmn_kopiec;
                    cout<<endl;

                }

            }






    plik.close();
    zapis.close();
    czas.close();
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

