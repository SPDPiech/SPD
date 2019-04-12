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
    string NajlepszaKolejnosc;

    ProblemPrzeplywowy Problem1;
    Problem1.WczytajDane("Dane.txt");
    //Problem1.PrzegladZupelny(&NajlepszaKolejnosc);
    //Cmax = Problem1.AlgorytmJohnsona(&NajlepszaKolejnosc);
    //Problem1.ZmodyfikowanyNEH(&NajlepszaKolejnosc);

    cout<<"NEH: "<<endl;
    Cmax = Problem1.AkceleracjaNEH(&NajlepszaKolejnosc);
    cout<<"Cmax "<<Cmax<<endl;
    cout<<"NK: "<<NajlepszaKolejnosc<<endl;
    cout<<endl;
    //Problem1.AkceleracjaNEH(&NajlepszaKolejnosc);
    //Problem1.Test();
/*
    cout<<"Wyzarzanie temperatura poczatkowa 100, parametr 0.90, temperatura koncowa 50"<<endl;
    Cmax = Problem1.AlgorytmWyzarzaniaNEH(&NajlepszaKolejnosc, 100, 0.90, 50,1);
    cout<<"Cmax "<<Cmax<<endl;
    cout<<"NK: "<<NajlepszaKolejnosc<<endl;
    cout<<endl;

    cout<<"Wyzarzanie temperatura poczatkowa 100, parametr 0.99, temperatura koncowa 50"<<endl;
    Cmax = Problem1.AlgorytmWyzarzaniaNEH(&NajlepszaKolejnosc, 100, 0.99, 50,1);
    cout<<"Cmax "<<Cmax<<endl;
    cout<<"NK: "<<NajlepszaKolejnosc<<endl;
    cout<<endl;

    cout<<"Wyzarzanie temperatura poczatkowa 100, parametr 0.80, temperatura koncowa 50"<<endl;
    Cmax = Problem1.AlgorytmWyzarzaniaNEH(&NajlepszaKolejnosc, 100, 0.80, 50,1);
    cout<<"Cmax "<<Cmax<<endl;
    cout<<"NK: "<<NajlepszaKolejnosc<<endl;
    cout<<endl;

    cout<<"Losowy Wyzarzanie temperatura poczatkowa 100, parametr 0.90, temperatura koncowa 50"<<endl;
    Cmax = Problem1.AlgorytmWyzarzaniaLosowy(&NajlepszaKolejnosc, 100, 0.90, 50,1);
    cout<<"Cmax "<<Cmax<<endl;
    cout<<"NK: "<<NajlepszaKolejnosc<<endl;
    cout<<endl;

    cout<<"Losowy Wyzarzanie temperatura poczatkowa 100, parametr 0.99, temperatura koncowa 50"<<endl;
    Cmax = Problem1.AlgorytmWyzarzaniaLosowy(&NajlepszaKolejnosc, 100, 0.99, 50,1);
    cout<<"Cmax "<<Cmax<<endl;
    cout<<"NK: "<<NajlepszaKolejnosc<<endl;
    cout<<endl;

    cout<<"Losowy Wyzarzanie temperatura poczatkowa 100, parametr 0.80, temperatura koncowa 50"<<endl;
    Cmax = Problem1.AlgorytmWyzarzaniaLosowy(&NajlepszaKolejnosc, 100, 0.80, 50,1);
    cout<<"Cmax "<<Cmax<<endl;
    cout<<"NK: "<<NajlepszaKolejnosc<<endl;
    cout<<endl;

    cout<<"Wyzarzanie temperatura poczatkowa 100, parametr 0.90, temperatura koncowa 10"<<endl;
    Cmax = Problem1.AlgorytmWyzarzaniaNEH(&NajlepszaKolejnosc, 100, 0.90, 10,1);
    cout<<"Cmax "<<Cmax<<endl;
    cout<<"NK: "<<NajlepszaKolejnosc<<endl;
    cout<<endl;

    cout<<"Wyzarzanie temperatura poczatkowa 100, parametr 0.90, temperatura koncowa 30"<<endl;
    Cmax = Problem1.AlgorytmWyzarzaniaNEH(&NajlepszaKolejnosc, 100, 0.90, 30,1);
    cout<<"Cmax "<<Cmax<<endl;
    cout<<"NK: "<<NajlepszaKolejnosc<<endl;
    cout<<endl;

    cout<<"Wyzarzanie temperatura poczatkowa 100, parametr 0.90, temperatura koncowa 50"<<endl;
    Cmax = Problem1.AlgorytmWyzarzaniaNEH(&NajlepszaKolejnosc, 100, 0.90, 60,1);
    cout<<"Cmax "<<Cmax<<endl;
    cout<<"NK: "<<NajlepszaKolejnosc<<endl;
    cout<<endl;
*/
    //Problem1.TestAlgorytmow(10,3);
    Problem1.Compare("Dane.txt");

    return 0;
}
