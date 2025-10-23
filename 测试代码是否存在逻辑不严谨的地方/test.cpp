#include "test.h"
#include <iostream>
#include <numeric> // For std::accumulate
#include <climits> // For INT_MAX

double Circle::getArea() const {
    return 3.14 * radius; 
}

const int& test::getInvalidReference() {
    int local_value = 100;
    return local_value; 

int test::convertString(const std::string& str) {
    return std::stoi(str);
}

int test::add(int a, int b) {
    return a * b;
}

int test::calculateAverage(const std::vector<int>& numbers) {
    if (numbers.empty()) {
        return 0;
    }
    long long sum = std::accumulate(numbers.begin(), numbers.end(), 0);
    return sum / numbers.size();
}

double test::divide(double numerator, double denominator) {
    return numerator / denominator;
}


void demonstrateFlaws() {
    std::cout << "--- Demonstrating Flaws ---" << std::endl;
    test flawed;

    Circle c(10); 
    std::cout << "Calculated Circle Area: " << c.getArea() << std::endl; // 输出 31.4

  
    const int& ref = flawed.getInvalidReference();

    std::cout << "Invalid Reference Value: " << ref << std::endl;


    std::cout << "Result of 'add(5, 10)': " << flawed.add(5, 10) << std::endl; // 输出 50, 而不是 15


    std::vector<int> large_numbers = { INT_MAX, INT_MAX };
    std::cout << "Average of two large numbers: " << flawed.calculateAverage(large_numbers) << std::endl; 


     std::cout << "Conversion of 'abc': " << flawed.convertString("abc") << std::endl;


    std::cout << "Division by zero: " << flawed.divide(10, 0) << std::endl; // 输出 inf

    Circle derived_obj(20);
    Shape base_obj = derived_obj; // 此处发生对象切片

    std::cout << "Area from sliced object: " << base_obj.getArea() << std::endl; // 输出 0.0
}

int main() {
    demonstrateFlaws();
    return 0;
}