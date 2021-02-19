#include "Quadruple.h"

Quadruple::Quadruple(int a) {
    hi = (double)a;
    lo = 0.0;
}

Quadruple::Quadruple(int aHi, int aLo) {
    hi = (double)aHi;
    lo = (double)aLo;
}

Quadruple::Quadruple(float a) {
    hi = (double)a;
    lo = 0.0;
}

Quadruple::Quadruple(float aHi, float aLo) {
    hi = (double)aHi;
    lo = (double)aLo;
}

Quadruple::Quadruple(double a) {
    hi = a;
    lo = 0.0;
}

Quadruple::Quadruple(double aHi, double aLo) {
    hi = aHi;
    lo = aLo;
}

Quadruple Quadruple::operator+(const int& b) {
    Quadruple dsc = Quadruple(0.0, 0.0);
    double e;

    double t1 = hi + (double)b;
    e = t1 - hi;
    double t2 = (((double)b - e) + (hi - (t1 - e))) + lo;

    dsc.hi = t1 + t2;
    dsc.lo = t2 - (dsc.hi - t1);
    return dsc;
}

Quadruple Quadruple::operator+(const float& b) {
    Quadruple dsc = Quadruple(0.0, 0.0);
    double e;

    double t1 = hi + (double)b;
    e = t1 - hi;
    double t2 = (((double)b - e) + (hi - (t1 - e))) + lo;

    dsc.hi = t1 + t2;
    dsc.lo = t2 - (dsc.hi - t1);
    return dsc;
}

Quadruple Quadruple::operator+(const double& b) {
    Quadruple dsc = Quadruple(0.0, 0.0);
    double e;

    double t1 = hi + b;
    e = t1 - hi;
    double t2 = ((b - e) + (hi - (t1 - e))) + lo;

    dsc.hi = t1 + t2;
    dsc.lo = t2 - (dsc.hi - t1);
    return dsc;
}

Quadruple Quadruple::operator+(const Quadruple& b) {
    Quadruple dsc = Quadruple(0.0, 0.0);
    double e;

    double t1 = hi + b.hi;
    e = t1 - hi;
    double t2 = ((b.hi - e) + (hi - (t1 - e))) + lo + b.lo;

    dsc.hi = t1 + t2;
    dsc.lo = t2 - (dsc.hi - t1);
    return dsc;
}

Quadruple Quadruple::operator-(const int& b) {
    return operator+(-b);
}

Quadruple Quadruple::operator-(const float& b) {
    return operator+(-b);
}

Quadruple Quadruple::operator-(const double& b) {
    return operator+(-b);
}

Quadruple Quadruple::operator-(const Quadruple& b) {
    return operator+(Quadruple(-b.hi,-b.lo));
}

Quadruple Quadruple::operator*(const int& b) {
    double split = 134217729;

    double cona = hi * split;
    double conb = (double)b * split;
    double a1 = cona - (cona - hi);
    double b1 = conb - (conb - (double)b);
    double a2 = hi - a1;
    double b2 = (double)b - b1;

    double c11 = hi * (double)b;
    double c21 = a2 * b2 + (a2 * b1 + (a1 * b2 + (a1 * b1 - c11)));

    double c2 = hi * 0.0 + lo * (double)b;

    double t1 = c11 + c2;
    double e = t1 - c11;
    double t2 = lo * 0.0 + ((c2 - e) + (c11 - (t1 - e))) + c21;

    Quadruple dsc = Quadruple(0.0,0.0);
    dsc.hi = t1 + t2;
    dsc.lo = t2 - (dsc.hi - t1);
    return dsc;
}

Quadruple Quadruple::operator*(const float& b) {
    double split = 134217729;

    double cona = hi * split;
    double conb = (double)b * split;
    double a1 = cona - (cona - hi);
    double b1 = conb - (conb - (double)b);
    double a2 = hi - a1;
    double b2 = (double)b - b1;

    double c11 = hi * (double)b;
    double c21 = a2 * b2 + (a2 * b1 + (a1 * b2 + (a1 * b1 - c11)));

    double c2 = hi * 0.0 + lo * (double)b;

    double t1 = c11 + c2;
    double e = t1 - c11;
    double t2 = lo * 0.0 + ((c2 - e) + (c11 - (t1 - e))) + c21;

    Quadruple dsc = Quadruple(0.0,0.0);
    dsc.hi = t1 + t2;
    dsc.lo = t2 - (dsc.hi - t1);
    return dsc;
}

Quadruple Quadruple::operator*(const double& b) {
    double split = 134217729;

    double cona = hi * split;
    double conb = b * split;
    double a1 = cona - (cona - hi);
    double b1 = conb - (conb - b);
    double a2 = hi - a1;
    double b2 = b - b1;

    double c11 = hi * b;
    double c21 = a2 * b2 + (a2 * b1 + (a1 * b2 + (a1 * b1 - c11)));

    double c2 = hi * 0.0 + lo * b;

    double t1 = c11 + c2;
    double e = t1 - c11;
    double t2 = lo * 0.0 + ((c2 - e) + (c11 - (t1 - e))) + c21;

    Quadruple dsc = Quadruple(0.0,0.0);
    dsc.hi = t1 + t2;
    dsc.lo = t2 - (dsc.hi - t1);
    return dsc;
}

Quadruple Quadruple::operator*(const Quadruple& b) {
    double split = 134217729;

    double cona = hi * split;
    double conb = b.hi * split;
    double a1 = cona - (cona - hi);
    double b1 = conb - (conb - b.hi);
    double a2 = hi - a1;
    double b2 = b.hi - b1;

    double c11 = hi * b.hi;
    double c21 = a2 * b2 + (a2 * b1 + (a1 * b2 + (a1 * b1 - c11)));

    double c2 = hi * b.lo + lo * b.hi;

    double t1 = c11 + c2;
    double e = t1 - c11;
    double t2 = lo * b.lo + ((c2 - e) + (c11 - (t1 - e))) + c21;

    Quadruple dsc = Quadruple(0.0, 0.0);
    dsc.hi = t1 + t2;
    dsc.lo = t2 - (dsc.hi - t1);
    return dsc;
}

Quadruple Quadruple::operator/(const int& b) {
    return operator*(1.0/(double)b);
}

Quadruple Quadruple::operator/(const float& b) {
    return operator*(1.0f/b);
}

Quadruple Quadruple::operator/(const double& b) {
    return operator*(1.0/b);
}

Quadruple Quadruple::operator/(const Quadruple& b) {
    return operator*(Quadruple(1.0/b.hi,1.0/b.lo));
}

bool Quadruple::operator==(const int& b) {
    return hi == (double)b && lo == 0.0;
}

bool Quadruple::operator==(const float& b) {
    return hi == (double)b && lo == 0.0;
}

bool Quadruple::operator==(const double& b) {
    return hi == b && lo == 0.0;
}

bool Quadruple::operator==(const Quadruple& b) {
    return hi == b.hi && lo == b.lo;
}

bool Quadruple::operator>(const int& b) {
    return hi > double(b) || (hi == double(b) && lo > 0.0);
}

bool Quadruple::operator>(const float& b) {
    return hi > double(b) || (hi == double(b) && lo > 0.0);
}

bool Quadruple::operator>(const double& b) {
    return hi > b || (hi == b && lo > 0.0);
}

bool Quadruple::operator>(const Quadruple& b) {
    return hi > b.hi || (hi == b.hi && lo > b.lo);
}

bool Quadruple::operator>=(const int& b) {
    return hi >= (double)b || (hi == (double)b && lo >= 0.0);
}

bool Quadruple::operator>=(const float& b) {
    return hi >= (double)b || (hi == (double)b && lo >= 0.0);
}

bool Quadruple::operator>=(const double& b) {
    return hi >= b || (hi == b && lo >= 0.0);
}

bool Quadruple::operator>=(const Quadruple& b) {
    return hi >= b.hi || (hi == b.hi && lo >= b.lo);
}