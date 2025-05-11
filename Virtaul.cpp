#include<iostream>


// class Person {
// public:
//     ~Person() { std::cout << "Person destroyed" << std::endl; }
// };

// class Student : public Person {
// public:
//     ~Student() { std::cout << "Student destroyed" << std::endl; }
// };

// int main() {
//     Person* p = new Student();
//     delete p; // What destructors are called?
//     return 0;
// }

class Person {
public:
    virtual ~Person() { std::cout << "Person destroyed" << std::endl; }
};
class Student : public Person {
    // No destructor defined
};
int main() {
    Person* p = new Student();
    delete p; // Calls default Student destructor, then Person destructor
    return 0;
}