#include "johnson.h"
using namespace std;

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


flowship::flowship(): tasks(0), machines(0), data(NULL){;}


flowship::~flowship()
{
    if (data !=NULL)
    {
        for (int i=0; i<tasks; i++)
            delete[] data[i];
        delete[] data;
    }
}

void flowship::LoadData(string file)
{
    fstream plik(file.c_str(), ios::in);

    if(plik.good())
    {
        plik>>tasks>>machines;

        data = new int*[tasks];
        for(int i=0; i<tasks; i++)
            data[i] = new int[machines];

        for (int i=0; i<tasks; i++)
            for(int j=0; j<machines; j++)
            plik>>data[i][j];
    }
    else cerr<<"nie udalo sie wczytac pliku"<<endl;
}

int flowship::Cmax(string order)
{
    int prespos=0;
    int prevpos=0;
    string prestask="";
    int presenttask=0;
    int *machcmax;
    int cmax=0;

    machcmax = new int[machines];
    for (int i=0; i<machines; i++)
        machcmax[i]=0;

    while((prespos=order.find(",",prespos+1)) != string::npos)
    {
        prestask="";
        prestask.insert(0, order, prevpos, prespos-prevpos);
        presenttask=atoi(prestask.c_str())-1;

        machcmax[0]=machcmax[0]+data[presenttask][0];

        for (int i=1; i<machines; i++)
        {
            if (machcmax[i-1]>machcmax[i])
                machcmax[i]=machcmax[i-1]+data[presenttask][i];
            else if(machcmax[i-1]<=machcmax[i])
                machcmax[i]=machcmax[i]+data[presenttask][i];
        }

        prevpos=prespos+1;
    }

    cmax=machcmax[machines-1];

    delete[] machcmax;

    return cmax;



}

int flowship::CompleteReview(string *bestord)
{
    string order="";
    string bestorder="";

    int ordercmax=99999999;
    int cmax=99999999;

    char tempchar[32];

    int *temp= new int[tasks];
    for (int i=1; i<=tasks; i++)
        temp[i-1]=i;

    do
    {
        order="";
        for(int i=0; i<tasks; i++)
        {
            order.append(itoa(temp[i],tempchar, 10));
            order.append(",");
        }

        ordercmax=Cmax(order);

            if(ordercmax<cmax)
            {
               cmax=ordercmax;
               bestorder=order;
            }




    }while(next_permutation(temp, temp+tasks));

    delete[] temp;
    *bestord=bestorder;
    return cmax;
}

int flowship::JohnsonsAlg(string *bestorder)
{
    string order;
    char tempchar[32];
    int cmaxmin=99;
    int taskno=0;
    int machineno=0;

    vector<int> list1;
    vector<int> list2;
    vector<int> tasklist;
    vector<int>::iterator taskiterator;

    if (machines==2)
    {
        for (int i=1; i<=tasks; i++)
            tasklist.push_back(i);

        list1.clear();
        list2.clear();

        while(!tasklist.empty())
        {


            vector<int>::iterator i=tasklist.begin();
            for(; i!=tasklist.end();i++)
            {
                for(int j=0; j<2; j++)
                {
                    if(cmaxmin>data[*i-1][j])
                    {
                        cmaxmin=data[*i-1][j];
                        taskno=*i;
                        taskiterator=i;
                        machineno=j;
                    }

                }
            }
            if(machineno==0)
            {
                list1.push_back(taskno);
                tasklist.erase(taskiterator);
                cmaxmin=999999;
            }
            else if(machineno==1)
            {
                list2.insert(list2.begin(),taskno);
                tasklist.erase(taskiterator);
                cmaxmin=999999;
            }

        }

        list1.insert(list1.end(),list2.begin(),list2.end());
        order="";
        for(size_t k=0; k<list1.size();k++)
        {
            order.append(itoa(list1[k],tempchar,10));
            order.append(",");
        }


        *bestorder=order;
        return Cmax(order);
    }

    else if(machines==3)
    {
        int **combinedata = new int*[tasks];
        for (int i=0; i<tasks; i++)
            combinedata[i]=new int[2];

        for (int i=0; i<tasks; i++)
            for(int j=0; j<2; j++)
            combinedata[i][j]=data[i][j]+data[i][j+1];

        for (int i=1; i<=tasks; i++)
            tasklist.push_back(i);

        list1.clear();
        list2.clear();

        while(!tasklist.empty())
        {


            vector<int>::iterator i=tasklist.begin();
            for(; i!=tasklist.end();i++)
            {
                for(int j=0; j<2; j++)
                {
                    if(cmaxmin>combinedata[*i-1][j])
                    {
                        cmaxmin=combinedata[*i-1][j];
                        taskno=*i;
                        taskiterator=i;
                        machineno=j;
                    }

                }
            }
            if(machineno==0)
            {
                list1.push_back(taskno);
                tasklist.erase(taskiterator);
                cmaxmin=999999;
            }
            else if(machineno==1)
            {
                list2.insert(list2.begin(),taskno);
                tasklist.erase(taskiterator);
                cmaxmin=999999;
            }

        }
        list1.insert(list1.end(),list2.begin(),list2.end());
        order="";
        for(size_t k=0; k<list1.size();k++)
        {
            order.append(itoa(list1[k],tempchar,10));
            order.append(",");
        }

        for (int i=0; i<tasks; i++)
            delete[] combinedata[i];
        delete[] combinedata;

        *bestorder=order;
        return Cmax(order);

    }

   else cout<<"Algorytm dziala dla problemow 2 i 3-maszynowych"<<endl;
}
/*
int flowship::NEHAlg(string *bestorder)
{
    vector<NEHData> weight;

    for (int i=0; i<tasks; i++)
    {
        weight.push_back(NEHData(i,0));
        for (int j=0; j<machines; j++)
            weight[i].Value=weight[i].Value+data[i][j];
    }

    vector<int> sortedtab;
    int maxvalue=0;
    int taskno=0;

    vector<NEHData>::iterator i=weight.begin();
    vector<NEHData>::iterator _iterator;

    while(!weight.empty())
    {
        i=weight.begin();
        for(; i!=weight.end(); i++)
            for(int j=0; j<machines; j++)
            {
                if(i->Value > maxvalue)
                {
                    maxvalue=i->Value;
                    taskno=i->TaskNumber;
                    _iterator=i;

                }

            }

        sortedtab.push_back(taskno);
        weight.erase(_iterator);
        taskno=0;
        maxvalue=0;
    }


    string list2;
    string list1;

    string finalcombination="";
    int cmaxmin=5555555;
    int cmax=0;
    int position=0;
    char tempchar[32];

    for(size_t k = 0; k < sortedtab.size(); k++)
    {
        position = 0;
        cmaxmin = 300000000;
    for(size_t l = 0; l <= k; l++)
        {
            list1 = "";
            list1 = list2;
            if(l==0)
                position=-1;
            else
                position = list1.find(',' , position+1);
            list1.insert(position+1, itoa(sortedtab[k] + 1, tempchar, 10));

            if(sortedtab[k] + 1 < 10)
            {
                list1.insert(position+2, ",");
            }
            else if(sortedtab[k] + 1 >= 10)
            {
                list1.insert(position+3, ",");

            }else if(sortedtab[k] + 1 >= 100)
            {
                list1.insert(position+4, ",");

            }else if(sortedtab[k] + 1 >= 1000)
            {
                list1.insert(position+5, ",");
            }


            //cout<<list1<<endl;
            cmax = Cmax(list1);
            //cout<<cmax<<endl;
            if(cmaxmin > cmax)
            {
                cmaxmin = cmax;
                finalcombination = list1;
            }
        }
        list2 = finalcombination;
    }


    *bestorder = finalcombination;
    return cmaxmin;
}

*/
int flowship::NEHAlg(string* order)
{
    vector<NEHData> ZsumowaneWagi;

    for(uint32_t i = 0; i < tasks; i++)
    {
        ZsumowaneWagi.push_back(NEHData(i, 0));
        for(uint32_t j = 0; j < machines; j++)
        {
            ZsumowaneWagi[i].Value += data[i][j];
        }
    }

    vector<uint32_t> PosortowanaTablica;
    uint32_t MaxWartosc = 0;
    uint32_t NumerZadania = 0;
    vector<NEHData>::iterator i = ZsumowaneWagi.begin();
    vector<NEHData>::iterator Iterator;



    while(!ZsumowaneWagi.empty())
    {
        i = ZsumowaneWagi.begin();
        for(; i != ZsumowaneWagi.end(); i++)
        {
            for(uint32_t j = 0; j < machines; j++)
            {
                if(i->Value > MaxWartosc)
                {
                    MaxWartosc = i->Value;
                    NumerZadania = i->TaskNumber;
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
    uint32_t _cmax;
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

            _cmax = Cmax(Lista1);

            if(NajlepszeCmax > _cmax)
            {
                NajlepszeCmax = _cmax;
                OstatecznaKombinacja = Lista1;
            }
        }
        Lista2 = OstatecznaKombinacja;
    }

    //cout<<"Czas potrzebny dla neh-a: "<<GetTickCount() - CzasRozpoczecia<<endl;

    //cout<<"Najlepsza kombinacja "<<OstatecznaKombinacja<<endl;
    //cout<<"Najlepsze Cmax "<<NajlepszeCmax<<endl;
    *order = OstatecznaKombinacja;
    return NajlepszeCmax;
}
int flowship::AccNEH(string* order)
{
    vector<NEHData> ZsumowaneWagi;

    for(uint32_t i = 0; i < tasks; i++)
    {
        ZsumowaneWagi.push_back(NEHData(i, 0));
        for(uint32_t j = 0; j < machines; j++)
        {
            ZsumowaneWagi[i].Value += data[i][j];
        }
    }

    vector<uint32_t> PosortowanaTablica;
    uint32_t MaxWartosc = 0;
    uint32_t NumerZadania = 0;
    vector<NEHData>::iterator i = ZsumowaneWagi.begin();
    vector<NEHData>::iterator Iterator;

    while(!ZsumowaneWagi.empty())
    {
        i = ZsumowaneWagi.begin();
        for(; i != ZsumowaneWagi.end(); i++)
        {
            for(uint32_t j = 0; j < machines; j++)
            {
                if(i->Value > MaxWartosc)
                {
                    MaxWartosc = i->Value;
                    NumerZadania = i->TaskNumber;
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
    uint32_t _cmax = 99999999;

    string Tymczasowy;
    string OstatecznaKombinacja = "";
    char Pomocnicza[32];
    int Pozycja = 0;
    int Pozycja1 = 0;

    uint32_t* SciezkaPom = new uint32_t [machines];
    for(uint32_t i = 0; i < machines; i++)
        SciezkaPom[i] = 0;

    uint32_t* WartoscMax = new uint32_t[machines+1];
    for(uint32_t i = 0; i <= tasks; i++)
        WartoscMax[i] = 0;

    uint32_t** SciezkaD = new uint32_t*[machines];
    for(uint32_t i = 0; i < tasks; i++)
        SciezkaD[i] = new uint32_t[machines];

    uint32_t** SciezkaW = new uint32_t*[machines];
    for(uint32_t i = 0; i < tasks; i++)
        SciezkaW[i] = new uint32_t[machines];

    //int CzasRozpoczecia = GetTickCount();
    for(uint32_t i = 0; i < PosortowanaTablica.size(); i++)
    {
        AktualneZadanie = PosortowanaTablica[i];
        SciezkaD[AktualneZadanie][0] = data[AktualneZadanie][0];
        SciezkaW[AktualneZadanie][machines-1] = data[AktualneZadanie][machines-1];
        for(uint32_t k = 1; k < machines; k++)
        {
            SciezkaD[AktualneZadanie][k] = data[AktualneZadanie][k] + SciezkaD[AktualneZadanie][k-1];
            SciezkaW[AktualneZadanie][machines-k-1] = data[AktualneZadanie][machines-k-1] + SciezkaW[AktualneZadanie][machines-k] ;
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


                    for(uint32_t k = 0; k < machines; k++)
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

                    for(uint32_t k = 0; k < machines; k++)
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

                    SciezkaPom[0] = SciezkaD[PoprzednieZadanie][0]+data[AktualneZadanie][0];

                    if(WartoscMax[j] < SciezkaPom[0]+SciezkaW[NastepneZadanie][0])
                            WartoscMax[j] = SciezkaPom[0]+SciezkaW[NastepneZadanie][0];

                    for(uint32_t k = 1; k < machines; k++)
                    {

                        SciezkaPom[k] = data[AktualneZadanie][k] + max(SciezkaD[PoprzednieZadanie][k], SciezkaPom[k-1]);

                        if(WartoscMax[j] < SciezkaPom[k] + SciezkaW[NastepneZadanie][k])
                        {
                            WartoscMax[j] = SciezkaPom[k] + SciezkaW[NastepneZadanie][k];
                        }
                    }
                }
            }



            _cmax = 999999999;
            for(uint32_t j = 0; j <= i; j++)
            {
                if(WartoscMax[j] < _cmax)
                {
                    _cmax = WartoscMax[j];
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
                    SciezkaD[AktualneZadanie1][0] = data[AktualneZadanie1][0];
                    for(uint32_t j = 1; j < machines; j++)
                    {
                        SciezkaD[AktualneZadanie1][j] = data[AktualneZadanie1][j] + SciezkaD[AktualneZadanie1][j-1];
                    }

                }else
                {
                    SciezkaD[AktualneZadanie1][0] = SciezkaD[PoprzednieZadanie1][0] + data[AktualneZadanie1][0];
                    for(uint32_t j = 1; j < machines; j++)
                    {
                        SciezkaD[AktualneZadanie1][j] = data[AktualneZadanie1][j] + max(SciezkaD[AktualneZadanie1][j-1], SciezkaD[PoprzednieZadanie1][j]);
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
                    SciezkaW[AktualneZadanie1][machines-1] = data[AktualneZadanie1][machines-1];
                    for(int j = machines-2; j >= 0; j--)
                    {
                        SciezkaW[AktualneZadanie1][j] = data[AktualneZadanie1][j] + SciezkaW[AktualneZadanie1][j+1];
                    }

                }else
                {
                    SciezkaW[AktualneZadanie1][machines-1] = SciezkaW[PoprzednieZadanie1][machines-1] + data[AktualneZadanie1][machines-1];
                    for(int j = machines-2; j >= 0; j--)
                    {
                        SciezkaW[AktualneZadanie1][j] = data[AktualneZadanie1][j] + max(SciezkaW[AktualneZadanie1][j+1], SciezkaW[PoprzednieZadanie1][j]);
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

    for(uint32_t i = 0; i < tasks; i++)
    {
        delete[] SciezkaD[i];
        delete[] SciezkaW[i];
    }

    delete[] SciezkaD;
    delete[] SciezkaW;

    delete[] WartoscMax;

    cout<<"Cmax: "<<_cmax<<endl;
    cout<<"Kombinacja: "<<OstatecznaKombinacja<<endl;

    *order = OstatecznaKombinacja;
    return _cmax;
}

void flowship::PrintData()
{
    if(data==NULL || tasks==0 || machines==0)
    {
        cerr<<"Brak danych do wyswietlenia"<<endl;
    }
    else
    {
        cout<<tasks<<" "<<machines<<endl;
        for (int i=0; i<tasks; i++)
        {

            cout<<endl;
            for(int j=0; j<machines; j++)
                cout<<data[i][j]<<" ";
        }
        cout<<endl;
    }

}

void flowship::RandomData(int _tasks, int _machines)
{
    if (data==NULL)
    {
        tasks=_tasks;
        machines=_machines;

        data  = new int*[tasks];
        for(int i=0; i<tasks; i++)
            data[i]= new int[machines];

        for(int i=0; i<tasks; i++)
            for(int j=0; j<machines; j++)
                data[i][j]=rand()%50;

    }

    else
    {

        for(int i=0; i<tasks; i++)
            delete[] data[i];

       delete[] data;

        tasks=_tasks;
        machines=_machines;

        data = new int*[tasks];
        for(int i=0; i<tasks; i++)
            data[i]= new int[machines];

        for(int i=0; i<tasks; i++)
            for(int j=0; j<machines; j++)
                data[i][j]=rand()%50;
    }
}

int flowship::ModNEH(string *order)
{
    vector<NEHData> ZsumowaneWagi;

    for(uint32_t i = 0; i < tasks; i++)
    {
        ZsumowaneWagi.push_back(NEHData(i, 0));
        for(uint32_t j = 0; j < tasks; j++)
        {
            ZsumowaneWagi[i].Value += data[i][j];
        }
    }

    vector<uint32_t> PosortowanaTablica;
    uint32_t MaxWartosc = 0;
    uint32_t NumerZadania = 0;
    vector<NEHData>::iterator i = ZsumowaneWagi.begin();
    vector<NEHData>::iterator Iterator;

    while(!ZsumowaneWagi.empty())
    {
        i = ZsumowaneWagi.begin();
        for(; i != ZsumowaneWagi.end(); i++)
        {
            for(uint32_t j = 0; j < machines; j++)
            {
                if(i->Value > MaxWartosc)
                {
                    MaxWartosc = i->Value;
                    NumerZadania = i->TaskNumber;
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
    uint32_t _cmax;
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

            _cmax = Cmax(Lista1);

            if(NajlepszeCmax > _cmax)
            {
                NajlepszeCmax = _cmax;
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

                    _cmax = Cmax(Lista2);

                    if(NajlepszeCmax > _cmax)
                    {
                        NajlepszeCmax = _cmax;
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

                _cmax = Cmax(Lista1);

                if(NajlepszeCmax > _cmax)
                {
                    NajlepszeCmax = _cmax;
                    OstatecznaKombinacja = Lista1;
                }
            }
            Lista2 = OstatecznaKombinacja;
        }
    }

    //cout<<"Najlepsza kombinacja "<<OstatecznaKombinacja<<endl;
    //cout<<"Najlepsze Cmax "<<NajlepszeCmax<<endl;
    *order = OstatecznaKombinacja;
    return NajlepszeCmax;
}


void flowship::AlgTesting(int _tasks, int _machines)
{
    string bestorder="";
    int cmax;


    if(_tasks >= 10)
        _tasks = 10;

    if(_machines >= 3)
        _machines = 3;

    cout<<"____________________________________________________________________________"<<endl;
    cout.width(12);
    cout<<left<<"Ilosc zadan";
    cout.width(13);
    cout<<left<<"Ilosc maszyn";
    cout.width(27);
    cout<<left<<"Przeglad (Czas/Kolejnosc)";
    cout.width(27);
    cout<<left<<"Johnson (Czas/Kolejnosc)"<<endl;

    for (int i=1; i<=_tasks; i++)
        for (int j=2; j<=_machines; j++)
        {
            RandomData(i,j);
            cmax = CompleteReview(&bestorder);
            cout.width(12);
            cout<<left<<i;
            cout.width(13);
            cout<<left<<j;
            cout.width(5);
            cout<<left<<cmax;
            cout.width(22);
            cout<<left<<bestorder;
            cmax = JohnsonsAlg(&bestorder);
            cout.width(5);
            cout<<left<<cmax;
            cout.width(22);
            cout<<left<<bestorder;
            cout<<endl;
        }


}

void flowship::Compare(string file)
{

    string linia;
    fstream plik;
    ofstream zapis;
    size_t position;
    int _tasks, _machines;
    int neh, johnson;
    clock_t start1,start2, stop1, stop2;
    double t_neh, t_johnson;
    string order;
    int i=0;


    cout<<"_______________________________________________________________________"<<endl;
    cout.width(27);
    cout<<left<<"Ilosc zadan";
    cout.width(27);
    cout<<left<<"Neh (Cmax/Czas)";
    cout.width(27);
    cout<<left<<"Johnson (Cmax/Czas)"<<endl;


    plik.open(file.c_str(), ios::in);
    zapis.open("zapis.txt");

        while(!plik.eof())
        {


            getline(plik, linia);
            position=linia.find("data");
            if(position!=string::npos)
            {


                //cout << linia << endl; //wyœwietlenie linii
                plik>>_tasks>>_machines;
                if (data==NULL)
                    {
                        tasks=_tasks;
                        machines=_machines;

                        data  = new int*[tasks];
                        for(int i=0; i<tasks; i++)
                            data[i]= new int[machines];

                        for(int i=0; i<tasks; i++)
                        for(int j=0; j<machines; j++)
                            plik>>data[i][j];

                        //cout<<tasks<<" "<<machines<<endl;

                    }
                else
                {


                    for(int i=0; i<tasks; i++)
                    delete[] data[i];

                    delete[] data;

                    tasks=_tasks;
                    machines=_machines;

                    data = new int*[tasks];
                    for(int i=0; i<tasks; i++)
                        data[i]= new int[machines];

                    for(int i=0; i<tasks; i++)
                        for(int j=0; j<machines; j++)
                            plik>>data[i][j];
                    }
                    //PrintData();
                    start1=clock();
                    neh=NEHAlg(&order);
                    stop1=clock();
                    t_neh=(double)(stop1-start1)/CLOCKS_PER_SEC;;
                    start2=clock();
                    //johnson=AccNEH(&order);
                    stop2=clock();
                    t_johnson=(double)(stop2-start2)/CLOCKS_PER_SEC;
                    zapis<<tasks<<"\t"<<neh<<"\t"<<t_neh<<"\t"<<johnson<<"\t"<<t_johnson<<endl;

                    cout.width(27);
                    cout<<left<<tasks;
                    cout.width(12);
                    cout<<left<<neh;
                    cout.width(12);
                    cout<<left<<setprecision(8)<<t_neh;
                    cout.width(12);
                    cout<<left<<johnson;
                    cout.width(12);
                    cout<<setprecision(8)<<t_johnson;
                    cout<<endl;

                    cout<<i<<endl;
                    i++;

                }

            }






    plik.close();
    zapis.close();
}

