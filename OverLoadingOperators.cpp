#include <iostream>
#include <vector>




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

class Vector3 {
    public : 
        int x;
        int y;
        int z;

        Vector3 (int a, int b, int c) 
        { 
            x = a;
            y = b;
            z = c;
        }

        int operator [] (int const& num) 
        { 
            if(num == 0 ){
                return x;
            } else if ( num == 1){
                return y;
            } else if ( num == 2){
                return z;
            }
            return 0;
        }

        Vector3 Multiply ( const Vector3& other) 
        {   
            return Vector3(x * other.x, y * other.y, z * other.z);
        }

        Vector3 operator * ( const Vector3& other) { 
            return Multiply(other);
        }
};



int main () { 

    Point a(2,5); 
    Point b(5,5);
    Point c = a +b;
    std::cout << c;


    Vector3 v1(10,20,30);
    Vector3 v2(30,40,50);

    Vector3 v3 = v1 * v2;

    std::cout << v1[1] << std::endl;
    std::cout << v3.x << " " << v3.y << " " << v3.z << std::endl;

}