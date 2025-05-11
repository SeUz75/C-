#include <iostream>
using namespace std;

class Person{
    private:
        int age;
        string name;
        string job;

    public:
        void setage(int a){ age =a;}
        void setname(string n){ name = n;}
        void setjob(string j){ job =j;}

        int getage(){return age;}
        string getname(){return name;}
        string getjob(){return job;}

        ~Person(){
            cout << "Object destroyed.";
        }
};


int main(){

    Person person2; // Stack memory
    Person *person1 = new Person(); // Heap memory... needs destructor

    person2.setage(10);
    person2.setjob("Dev");
    person2.setname("Batman");

    person1->setage(20);
    person1->setjob("Waiter");
    person1->setname("George");

    cout << person2.getage() << " " << person2.getname() << " " << person2.getjob() << endl;

    cout << person1->getage() << " " << person1->getname() << " " << person1->getjob() <<endl;




    return 0;
}