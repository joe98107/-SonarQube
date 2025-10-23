#ifndef LOGICAL_FLAWS_H
#define LOGICAL_FLAWS_H

#include <vector>
#include <string>

// 基类
class Shape {
public:
    virtual double getArea() const { return 0.0; }
    virtual ~Shape() = default; 
};

// 派生类
class Circle : public Shape {
public:
    Circle(double r) : radius(r) {}
    double getArea() const override;
private:
    double radius;
};

class FlawedLogic {
public:
    const int& getInvalidReference();
    int convertString(const std::string& str);
    int add(int a, int b);
    int calculateAverage(const std::vector<int>& numbers);
    double divide(double numerator, double denominator);
};

#endif // LOGICAL_FLAWS_H