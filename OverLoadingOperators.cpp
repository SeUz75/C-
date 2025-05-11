#include <iostream>



class Point { 

    public :
        Point (int a, int b){
            x = a ;
            b = y;
        }

        int x;
        int y;

        friend Point operator+ (Point const& a, Point const& b) {

            Point result( (a.x + b.x), (a.y + b.y) );

            return result;
        }

        std::ostream & operator << ( Point& result);
    
    friend std::ostream & operator << ( std::ostream &out, const Point &c) { 
        out << "("<<c.x  << ","  << c.y << ")";
        out << std::endl;

        return out;
    }
};


int main () { 

    Point a(2,5); 
    Point b(5,5);
    Point c = a +b;
    std::cout << c;
}