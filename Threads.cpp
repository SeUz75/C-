#include <iostream>
#include <thread>
#include <mutex>
#include <string>

std::mutex mtx;
int shared_result = 0;
bool result_ready = false;

void parse(std::string input) {
    try {
        int result = std::stoi(input);
        std::lock_guard<std::mutex> lock(mtx);
        shared_result = result;
        result_ready = true;
    } catch (const std::invalid_argument&) {
        std::cout << "Couldn't parse to int" << std::endl;
    } catch (const std::out_of_range&) {
        std::cout << "Number too large" << std::endl;
    }
}

void print() {
    while (true) {
        std::lock_guard<std::mutex> lock(mtx);
        if (result_ready) {
            std::cout << "Number parsed is: " << shared_result << std::endl;
            break;
        }
    }
}

int main() {
    std::string input;
    std::cout << "Enter a number: ";
    std::getline(std::cin, input); // Why use getline instead of cin >>?

    std::thread t1(parse, input);
    std::thread t2(print);

    t1.join();
    t2.join();

    return 0;
}