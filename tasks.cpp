#include <iostream>
#include <string>
using namespace std;

int main() {
    cout << "Enter two numbers: ";
    string a, b;
    cin >> a >> b;

    try {
        // Try parsing as integers
        int num1 = stoi(a);
        int num2 = stoi(b);
        // Perform operations with num1 and num2
    } catch (std::invalid_argument&) {
        try {
            // Try parsing as floats
            float num1 = stof(a);
            float num2 = stof(b);
            // Perform operations with num1 and num2
        } catch (std::invalid_argument&) {
            cout << "Invalid number format!" << endl;
        } catch (std::out_of_range&) {
            cout << "Number too large!" << endl;
        }
    } catch (std::out_of_range&) {
        cout << "Integer too large!" << endl;
    }

    return 0;
}