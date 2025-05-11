#include<iostream>
#include<memory>



class Person{
    public:
        std::string name;
        int age;
};

int main(){

    std::unique_ptr<Person> p1 = std::make_unique<Person>();

    p1->name = "Stergios";
    p1->age=23;

   std::cout <<"Name is : "  << p1->name << " and age : " << p1->age <<std::endl;

    std::shared_ptr<Person> p2 = std::make_shared<Person>();
    p2->age=23;
    p2->name="Hrisi";
    p2->name = p1->name;
    std::cout <<"Name is : "  << p2->name << " and age : " << p2->age <<std::endl;


    std::shared_ptr<Person> p3 = std::make_shared<Person>();
    p3->age = p2->age;
    p3->name = p2->name;

    std::cout <<"Name is : "  << p3->name << " and age : " << p3->age <<std::endl;
    
    //std::unique_ptr<Person> p4 = p1;
    //std::unique_ptr<Person> p4 = p2;
    std::shared_ptr<Person> p4 = p2;
    std:: cout << "p4 :" << p4->age <<" " <<p4->name <<std::endl;
    //So in order to copy a pointer it has both to be shared pointers ?

    


   std::shared_ptr<Person> p2 = std::make_shared<Person>();
std::shared_ptr<Person> p3 = p2; // Copy shared_ptr to shared_ptr
    
    
    return 0;
}