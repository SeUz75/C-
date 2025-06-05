#include <iostream>



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
}