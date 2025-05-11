#include<iostream>
using namespace std;

struct Utility
{
    static int add(int a, int b){
        return a+b;
    }

    static int count;

    static string name;
   
};

int Utility::count =0;
string Utility::name = "Stergios";




int main(){
    for(int i=0; i<3 ; i++)
    cout << Utility::add(3,4) << " " << Utility::count++ << endl;


     Utility utl;

     Utility utl2;

    cout << utl.name << " " << utl2.name ;


    return 0;
}