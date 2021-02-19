#ifndef QUADRUPLE_H
#define QUADRUPLE_H

#include <iostream>
#include <vector>

class Quadruple {
public:
    double hi;
    double lo;
    Quadruple(int a);
    Quadruple(int aHi, int aLo);
    Quadruple(float a);
    Quadruple(float aHi, float aLo);
    Quadruple(double a);
    Quadruple(double aHi, double aLo);
    Quadruple operator+(const int& b);
    Quadruple operator+(const float& b);
    Quadruple operator+(const double& b);
    Quadruple operator+(const Quadruple& b);
    Quadruple operator-(const int& b);
    Quadruple operator-(const float& b);
    Quadruple operator-(const double& b);
    Quadruple operator-(const Quadruple& b);
    Quadruple operator*(const int& b);
    Quadruple operator*(const float& b);
    Quadruple operator*(const double& b);
    Quadruple operator*(const Quadruple& b);
    Quadruple operator/(const int& b);
    Quadruple operator/(const float& b);
    Quadruple operator/(const double& b);
    Quadruple operator/(const Quadruple& b);

    bool operator==(const int& b);
    bool operator==(const float& b);
    bool operator==(const double& b);
    bool operator==(const Quadruple& b);
    bool operator>(const int& b);
    bool operator>(const float& b);
    bool operator>(const double& b);
    bool operator>(const Quadruple& b);
    bool operator>=(const int& b);
    bool operator>=(const float& b);
    bool operator>=(const double& b);
    bool operator>=(const Quadruple& b);

    operator int() const { return (int)(hi+lo); }
    operator float() const { return (float)(hi+lo); }
    operator double() const { return hi+lo; }
};

#endif