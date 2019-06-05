
#include "johnson.h"
using namespace std;

int main()
{
    srand(time(NULL));
    flowship problem1;
    string dane="data.txt";
    int review=0;
    int cmax=0;
    string order="";
    problem1.LoadData(dane);
    //problem1.RandomData(8, 4);
    //problem1.PrintData();
    cout<<"Przeglad zupelny:"<<endl;
    //review=problem1.CompleteReview(&order);
    cout<<"Najlepsza kolejnosc: "<<order<<endl;
    cout<<"Minimalny czas: "<<review<<endl;
    cout<<endl<<"Algorytm NEH:"<<endl;
    cmax=problem1.ModNEH(&order);
    //cout<<"Najlepsza kolejnosc: "<<order<<endl;
    //cout<<"Minimalny czas: "<<cmax<<endl;
    //problem1.AlgTesting(10, 3);
    problem1.Compare(dane);

    return 0;
}
