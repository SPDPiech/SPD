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
    string order;
    ofstream czas;

    czas.open("czas.txt");

    ProblemPrzeplywowy Problem1;
    //Problem1.WczytajDane("Dane.txt");
    //Problem1.PrzegladZupelny(&NajlepszaKolejnosc);
    //Cmax = Problem1.AlgorytmJohnsona(&NajlepszaKolejnosc);
    //Problem1.ZmodyfikowanyNEH(&NajlepszaKolejnosc);

    //Problem1.AkceleracjaNEH(&NajlepszaKolejnosc);
    //Problem1.Test();
    //Problem1.TestAlgorytmow(10,3);

    //cout<<"Wynik Schrage: "<<Problem1.Schrage(&NajlepszaKolejnosc)<<endl;
    //cout<<"Wynik Schrage z podzialem: "<<Problem1.SchrageZPodzialem()<<endl;

    //cout<<"Wynik Schrage z kopcem: "<<Problem1.ShrageNaKopcu(&NajlepszaKolejnosc)<<endl;
    //cout<<"Wynik Schrage z podzialem na kopcu: "<<Problem1.ShrageZPodzialemNaKopcu()<<endl;
    Problem1.Compare("Dane-1.txt");
   /* for (int i=3000; i<10000; i+=100)
    {
                    Problem1.GenerujLosoweDane(i, 3);
                    auto start4 = std::chrono::system_clock::now();
                    Problem1.Schrage(&order);
                    auto stop4 = std::chrono::system_clock::now();
                    std::chrono::duration<double>t_shrage =stop4-start4;



                    auto start1 = std::chrono::system_clock::now();
                    Problem1.SchrageZPodzialem();
                    auto stop1= std::chrono::system_clock::now();
                    std::chrono::duration<double>t_shrageptmn =stop1-start1;

                    auto start=std::chrono::system_clock::now();
                    Problem1.ShrageNaKopcu(&order);
                    auto stop=std::chrono::system_clock::now();
                    std::chrono::duration<double>t_shrage2 =stop-start;

                    auto start3=std::chrono::system_clock::now();
                    Problem1.ShrageZPodzialemNaKopcu();
                    auto stop3=std::chrono::system_clock::now();
                    std::chrono::duration<double>t_shrageptmn2 =stop-start;

                    czas<<i<<"\t"<<t_shrage.count()<<"\t"<<t_shrageptmn.count()<<"\t"<<t_shrage2.count()<<"\t"<<t_shrageptmn2.count()<<endl;

                    cout<<i<<endl;


    }*/
    czas.close();

    return 0;
}
