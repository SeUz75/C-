#include<iostream>
using namespace std;


struct person
    {
        int age;
        string name;
        string job;
    };

static  void printPerno(person p1){
    cout << p1.age << " " <<p1.name << " " <<p1.job << " " << endl;
}

person updatePerson(person p1){
    p1.age = 10;
    p1.job = "Slave";
    p1.name= "Bob";


    return p1;
}

void countCalls(){
   static int calls =0;
   calls++;
   cout << "Called " << calls << " times" ;
}


int main(){
    string message = "Hello bruh";
    std:: cout << message<< endl;

    string firstname, lastname;
    firstname = "John";
    lastname = "Down";
    string fullname = firstname + " " + lastname; 
    cout << fullname << endl;


    // String length is good to know how to get is :
    string txt = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    cout << "The length of the string is : " << txt.size() << endl;

    // Also can get a specific item of the sting 
    cout << txt[0] << txt[txt.length()-1];

    // Special characters \' \" \\

    txt = "We are the so-called \"Vikings\" from the north.";
    cout << txt << endl;

    txt = "It\'s alright.";
    cout << txt << endl;

    txt = "The character \\ is called backslash.";
    cout << txt << endl;



    // Break Continue section
    for (int i = 0; i < 10; i++) {
    if (i == 4) {
        break; // ok loop breaks when i reaches 4.
    }
    cout << i << "\n";
    } 



    for (int i = 0; i < 10; i++) {
    if (i == 4) {
        continue; // Ok loop skips when i=4
    }
    cout << i << "\n";
    } 


    // C++ Arrays

    // Create an array of strings
    string cars[5] = {"Volvo", "BMW", "Ford", "Mazda", "Tesla"};

    // Loop through strings
    for (int i = 0; i < 5; i++) {
    cout << cars[i] << "\n";
    }

    // For each loop
    for (string car :cars) {
      cout << car << "\n";
    }

    
    cout << sizeof(cars)/sizeof(cars[0]);
    int sizeofArray =sizeof(cars)/sizeof(cars[0]);
    for(int i =0; i<sizeofArray; i++){
        cout << cars[i] << "\n";
    }


    // MultiDimentional arrays !
    string letters[2][2]; // so here we have 2 rows and 2 columns

    letters[0][0] = "1";
    letters[1][0] = "2";
    letters[0][1]= "3";
    letters[1][1]= "4";

    for(int i = 0; i< 2 ; i++){
        for(int j= 0; j<2 ; j++){
            cout << letters[i][j] << endl;
        }
    }
    
    person person1 ;

    person1.age = 15;
    person1.name = "Stergios";
    person1.job = "One of the biggest developers in the world";


    person person2[] = {{16,"Hrisi", "Best friend and good dev as Stergios"}, {15, "KoKo", "Brother in law"}};

    person *person4 = new person();

    person4->age = 15;
    person4->job = "C++ dev";
    person4->name = "Stergios";

   delete person4;



    // Practise Exercise Pointer to Stack struct

        person p1;

        p1.name = "George";
        p1.age = 16;
        p1.job = "Taxi driver";

        person* ptr = &p1;

        ptr->age = 15;
        ptr->name = "John";
        ptr->job = "Sailor";

        cout << p1.name << " " << p1.age << " " << p1.job;



    // Dynamic Array

    person* people = new person[3];

    people[0].age = 15;
    people[0].name = "Stergios";
    people[0].job = "Working";

    people[1].age = 16;
    people[1].name = "Penny";
    people[1].job = "Doctor";

    people[2].age = 20;
    people[2].name = "Mekto";
    people[2].job = "dev";

    for(int i=0 ; i<3 ; i++){
        cout << people[i].name << " " << people[i].age << " " <<  people[i].job << endl;
    }

   delete[] people;



   person p23 = {20,"Jhon" , "Good person"};

   printPerno(p23);

   p23 = updatePerson(p23);

   printPerno(p23);

    countCalls();
    countCalls();
    countCalls();

    return 0;
}


