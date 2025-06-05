#include <iostream>
#include <vector>



class Vector3 {
    public : 
        int x;
        int y;
        int z;

        int operator [] (int const& num) { 
            if(num == 0 ){
                return x;
            } else if ( num == 1){
                return y;
            } else if ( num == 2){
                return z;
            }
        }
};


class Point { 

    public :
        Point (int a, int b){
            x = a ;
            y = b;
        }

        int x;
        int y;

        friend Point operator+ (Point const& a, Point const& b) {

            Point result( (a.x + b.x), (a.y + b.y) );

            return result;
        }
        // Now im ready that operator<< must be a non-member function 
        friend std::ostream& operator<<(std::ostream &out, const Point &p);
};

// Define operator<< outside the class
std::ostream& operator<<(std::ostream &out, const Point &p) {
    out << "(" << p.x << ", " << p.y << ")";
    return out;
}



int main () { 

    Point a(2,5); 
    Point b(5,5);
    Point c = a +b;
    std::cout << c;


    Vector3 v;
    v.x = 10;
    v.y = 20;
    v.z = 30;

    std::cout << v[1];
}