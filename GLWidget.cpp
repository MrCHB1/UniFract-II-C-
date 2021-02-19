#include "GLWidget.h"
#include <string.h>
//#include <GL/glu.h>
#include <stddef.h>
#include <iostream>

const char* vertexSource = R""""(
#version 410 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texcoord;
out vec2 aTexcoord;
void main() {
   gl_Position = vec4(pos, 0.0, 1.0);
   aTexcoord = texcoord;
}
)"""";

const char* fragmentSource = R""""(
#version 410 core

#extension GL_ARB_shading_language_420pack : enable

#pragma optionNV(fastmath off)
#pragma optionNV(fastprecision off)

#define M_PI 3.14159265358

uniform int itr;
uniform double zoom;
uniform dvec2 screenSize;
uniform dvec2 offset;
uniform dvec4 offset128;

uniform bool useQuadPrec;

uniform sampler1D gradient;

uniform float layer;
uniform float time;

uniform double bailout;

double n = 0.0;
double threshold = bailout;

uniform vec3 col1;
uniform vec3 col2;
uniform vec3 col3;
uniform vec3 col4;
uniform vec3 col5;
uniform vec3 col6;
uniform vec3 col7;
uniform vec3 col8;
uniform vec3 col9;
uniform vec3 col10;
uniform vec3 col11;

uniform vec3 inColor;

in vec2 aTexcoord;
uniform dvec2 cFormula;

bool escaped = false;

vec3 palette[11] = vec3[11](vec3(col1), vec3(col2), vec3(col3), vec3(col4), vec3(col5), vec3(col6), vec3(col7), vec3(col8), vec3(col9), vec3(col10), vec3(col11));

/* 
0 - Mandelbrot
1 - Burning Ship
2 - Tricorn / Mandelbar
*/
dvec2 z;

uniform int FRACTAL_TYPE;
uniform float POWER;

uniform bool juliaEnabled;
uniform bool smoothColoring;
uniform bool perturbationEnabled;

uniform dvec2 Start;

#define twopi 6.283185
#define halfpi twopi / 4

// Experimental double functions

double pow_d(double a, double b) {
    int e = int(b);
    struct U {
        double d;
        int x[2];
    };
    U u;
    u.d = a;
    u.x[1] = int((b-e)*(u.x[1] - 1072632447) + 1072632447);
    u.x[0] = 0;

    double r = 1.0;
    while (bool(e)) {
        if (bool(e & 1)) {
            r *= a;
        }
        a *= a;
        e >>= 1;
    }
    return r * u.d;
}

double atan2_d(double y, double x) {
    const double ONEQTR_PI = M_PI / 4.0;
    const double THRQTR_PI = 3.0 * M_PI / 4.0;
    double r, angle;
    double abs_y = abs(y) + 1e-10;
    if (x < 0.0) {
        r = (x + abs_y) / (abs_y - x);
        angle = THRQTR_PI;
    } else {
        r = (x - abs_y) / (x + abs_y);
        angle = ONEQTR_PI;
    }
    angle += (0.1963*r*r-0.9817)*r;
    if (y < 0.0)
        return -angle;
    else
        return angle;
}

double f2 = 2.0;
double f3 = 6.0;
double f4 = 24.0;
double f5 = 120.0;
double f6 = 720.0;
double f7 = 5040.0;
double f8 = 40320.0;
double f9 = f8*9.0;
double f10 = f9*10.0;
double f11 = f10*11.0;
double f12 = f11*12.0;
double f13 = f12*13.0;
double f14 = f13*14.0;
double f15 = f14*15.0;

double fact(double x) {
    double res = 1.0;
    for (double i = 1.0; i <= x; i++) {
        res *= i;
    }
    return res;
}

double sin_d(double x) {
    int i = 1;
    double cur = x;
    double acc = 1.0;
    double fc = 1.0;
    double p = x;
    while (abs(acc) > .00000001 && i < 100) {
        fc *= ((2.0*double(i))*(2.0*double(i)+1.0));
        p *= -1.0 * (x*x);
        acc = p / fc;
        cur += acc;
        i++;
    }
    return cur;
}

double cos_d(double x) {
    double t, s;
    int p = 0;
    s = 1.0;
    t = 1.0;
    while (abs(t/s) > .00000001) {
        p++;
        t = (-t*x*x)/((2.0*double(p)-1.0)*(2.0*double(p)));
        s += t;
    }
    return s;
}

dvec2 set(double a) {
    dvec2 z;
    z.x = a;
    z.y = 0.0;
    return z;
}

dvec2 add(dvec2 dsa, dvec2 dsb) {
    dvec2 dsc;
    double t1, t2, e;

    t1 = dsa.x + dsb.x;
    e = t1 - dsa.x;
    t2 = ((dsb.x - e) + (dsa.x - (t1 - e))) + dsa.y + dsb.y;

    dsc.x = t1 + t2;
    dsc.y = t2 - (dsc.x - t1);
    return dsc;
}

void split(precise double a, precise double hi, precise double lo) {
    precise double temp;
    if (a > 6.69692879491417e+299 || a < -6.69692879491417e+299) {
        a *= 3.7252902984619140625e-09;
        temp = 8192.0 * a;
        hi = temp - (temp - a);
        lo = a - hi;
        hi *= 268435456.0;
        lo *= 268435456.0;
    } else {
        temp = 8192.0 * a;
        hi = temp - (temp - a);
        lo = a - hi;
    }
}

double fms(precise double a, precise double b, precise double c) {
    return fma(a, b, -c);
}

double two_prod(precise double a, precise double b, precise double err) {
    double a_hi, a_lo, b_hi, b_lo;
    double p = a * b;
    split(a, a_hi, a_lo);
    split(b, b_hi, b_lo);
    err = ((a_hi * b_hi - p) + a_hi * b_lo + a_lo * b_hi) + a_lo * b_lo;
    return p;
}

double quick_two_sum(precise double a, precise double b, precise double err) {
    precise double s = a + b;
    err = b - (s - a);
    return s;
}

dvec2 mul(dvec2 dsa, dvec2 dsb) {
    precise dvec2 dsc;
    double c11, c21, c2, e, t1, t2;
    double a1, a2, b1, b2, cona, conb;
    const double split = 134217729.0;

    cona = dsa.x * split;
    conb = dsb.x * split;
    a1 = cona - (cona - dsa.x);
    b1 = conb - (conb - dsb.x);
    a2 = dsa.x - a1;
    b2 = dsb.x - b1;

    c11 = dsa.x * dsb.x;
    c21 = a2 * b2 + (a2 * b1 + (a1 * b2 + (a1 * b1 - c11)));

    c2 = dsa.x * dsb.y + dsa.y * dsb.x;

    t1 = c11 + c2;
    e = t1 - c11;
    t2 = dsa.y * dsb.y + ((c2 - e) + (c11 - (t1 - e))) + c21;

    dsc.x = t1 + t2;
    dsc.y = t2 - (dsc.x - t1);
    return dsc;
}

/*dvec2 mul(precise dvec2 a, precise dvec2 b) {
    precise double p1, p2;
    p1 = two_prod(a.x, b.x, p2);
    p2 += (a.x * b.y + a.y * b.x);
    p1 = quick_two_sum(p1, p2, p2);
    return dvec2(p1, p2);
}*/

dvec2 sub(dvec2 dsa, dvec2 dsb) {
    return add(dsa, mul(set(-1.0), dsb));
}

double cmp(dvec2 a, dvec2 b) {
    if (a.x == b.x && a.y == b.y)
        return 0.0;
    else if (a.x > b.x || (a.x == b.x && a.y > b.y))
        return 1.0;
    else if (a.x < b.x || (a.x == b.x && a.y < b.y))
        return -1.0;
}

dvec2 abs128(dvec2 a) {
    dvec2 c;
    if (cmp(a, set(0.0)) == -1.0) {
        c.x = -a.x;
        c.y = -a.y;
    } else if (cmp(a, set(0.0)) == 0.0 || cmp(a, set(0.0)) == 1.0) {
        c = a;
    }
    return c;
}

// DOUBLE128

// DOUBLE256 (Farthest I could go)

dvec4 set256(double a) {
    precise dvec4 z;
    z.x = a;
    z.y = 0.0;
    z.z = 0.0;
    z.w = 0.0;
    return z;
}

dvec4 add256(dvec4 dsa, dvec4 dsb) {
    dvec4 dsc;
    dvec2 t1, t2, e;

    t1 = add(dsa.xy, dsb.xy);
    e = sub(t1, dsa.xy);
    t2 = add(add(add(sub(dsb.xy, e), sub(dsa.xy, sub(t1, e))), dsa.zw), dsb.zw);

    dsc.xy = add(t1, t2);
    dsc.zw = sub(t2, sub(dsc.xy, t1));
    return dsc;
}

dvec2 c_pow(dvec2 a, float power) {
    double r = sqrt(a.x*a.x+a.y*a.y);
    float theta = atan(float(a.y), float(a.x));
    dvec2 z = pow(float(r), power) * dvec2(cos(power*theta), sin(power*theta));
    return z;
}

dvec2 c_abspow(dvec2 a, float power) {
    double r = sqrt(a.x*a.x+a.y*a.y);
    float theta = atan(float(z.y), float(z.x));
    dvec2 z = pow(float(r), power) * dvec2(abs(cos(power*theta)), abs(sin(power*theta)));
    return z;
}

dvec2 c_powre(dvec2 a, float power) {
    double r = sqrt(a.x*a.x+a.y*a.y);
    float theta = atan(float(z.y), float(-abs(z.x)));
    dvec2 z = pow(float(r), power) * dvec2(cos(power*theta), sin(power*theta));
    return z;
}

dvec2 c_powim(dvec2 a, float power) {
    double r = sqrt(a.x*a.x+a.y*a.y);
    float theta = atan(float(-abs(z.y)), float(z.x));
    dvec2 z = pow(float(r), power) * dvec2(cos(power*theta), sin(power*theta));
    return z;
}

dvec2 c_powcel(dvec2 a, float power) {
    double r = sqrt(a.x*a.x+a.y*a.y);
    float theta = atan(float(z.y), float(z.x));
    dvec2 z = pow(float(r), power) * dvec2(abs(cos(power*theta)), sin(power*theta));
    return z;
}

dvec2 c_times(dvec2 a, dvec2 b) {
    return dvec2(a.x*b.x-a.y*b.y,a.x*b.y+a.y*b.x);
}

dvec4 c_times128(dvec4 a, dvec4 b) {
    dvec4 o;
    o.xy = sub(mul(a.xy,b.xy), mul(a.zw,b.zw));
    o.zw = add(mul(a.xy,b.zw), mul(a.xy,b.zw));
    return o;
}

dvec2 c_celtimes(dvec2 a, dvec2 b) {
    return dvec2(abs(a.x*b.x-a.y*b.y),a.x*b.y+a.y*b.x);
}

dvec2 c_celtimesre(dvec2 a, dvec2 b) {
    return dvec2(abs(a.x*b.x-a.y*b.y),-(abs(a.x)*b.y+a.y*abs(b.x)));
}

dvec2 c_celtimesim(dvec2 a, dvec2 b) {
    return dvec2(abs(a.x*b.x-a.y*b.y),-(a.x*abs(b.y)+abs(a.y)*b.x));
}

double diffabs(double c, double d) {
    double cd = c+d;
    if (c >= 0.0) {
        if (cd >= 0.0) {
            return d;
        } else {
            return -d - 2.0 * c;
        }
    } else {
        if (cd > 0.0) {
            return d + 2.0 * c;
        } else {
            return -d;
        }
    }
}

dvec2 c_powi(dvec2 a, int power) {
    dvec2 o = a;
    for (int i = 1; i < power; i++) {
        o = c_times(o, a);
    }
    return o;
}

dvec2 c_powceli(dvec2 a, int power) {
    dvec2 o = a;
    for (int i = 1; i < power; i++) {
        o = c_celtimes(o, a);
    }
    return o;
}

dvec2 c_powcelrei(dvec2 a, int power) {
    dvec2 o = a;
    for (int i = 1; i < power; i++) {
        o = c_celtimesre(o, a);
    }
    return o;
}

dvec2 c_powcelimi(dvec2 a, int power) {
    dvec2 o = a;
    for (int i = 1; i < power; i++) {
        o = c_celtimesim(o, a);
    }
    return o;
}

dvec2 c_div(dvec2 a, dvec2 b) {
    return dvec2((a.x*b.x+a.y*b.y)/(b.x*b.x+b.y*b.y), (a.y*b.x-a.x*b.y)/(b.x*b.x+b.y*b.y));
}

bool slopes = true;
bool inverted = false;

bool distanceEstimation = false;

double mandelbrot(dvec2 c) {
    z = vec2(Start.x, Start.y);
    dvec2 dz = dvec2(1.0, 0.0);

    if (smoothColoring) {
        double c2 = dot(c, c);
    }

    if (slopes) {
        double h2 = 1.5;
        int angle = 45;

        int R = 100;
    }

    const double B = 256.0;
    double l = 0.0;

    for (int i = 0; i < itr; i++) {
        dvec2 dznew;
        dvec2 znew;

        double zrsqr = z.x*z.x;
        double zisqr = z.y*z.y;
        double zrzrsqr = zrsqr*zrsqr;
        double zizisqr = zisqr*zisqr;
        double zrzisqr = zrsqr*zisqr;

        //dvec2 zc = c_pow(z.x, z.y, POWER);
        //dvec2 zabsr = c_pow(-abs(z.x), z.y, POWER);
        //dvec2 zabsi = c_pow(z.x, -abs(z.y), POWER);
    
        if (FRACTAL_TYPE == 0) {
            if (POWER == int(POWER)) {
                if (POWER == 2) {
                    znew = c_powi(z, int(POWER)) + c;
                    if (POWER == 2)
                        dznew = c_times(dvec2(2.0, 0.0), c_times(z, dz))+1.0;
                } else {
                    znew = c_powi(z, int(POWER)) + c;
                }
            } else
                znew = c_pow(z, POWER) + c;
        } else if (FRACTAL_TYPE == 1) {
            if (POWER == int(POWER)) {
                znew = dvec2(c_powi(z, int(POWER)).x, -(c_powi(z, int(POWER))).y) + c;
                if (POWER == 2) {
                    dznew = c_times(dvec2(2.0, 0.0), c_times(dvec2(z.x,-z.y), dvec2(dz.x,-dz.y)))+1.0;
                }
            } else
                znew = dvec2(c_pow(z, POWER).x, -c_pow(z, POWER).y) + c;
        } else if (FRACTAL_TYPE == 2) {
            if (POWER == int(POWER))
                znew = c_powi(dvec2(abs(z.x), -abs(z.y)), int(POWER)) + c;
            else {
                if (z.x < 0.0) {
                    z.x = -z.x;
                }
                if (z.y < 0.0) {
                    z.y = -z.y;
                }
                znew = dvec2(c_pow(abs(z), POWER).x, -c_pow(abs(z), POWER).y) + c;
            }
        } else if (FRACTAL_TYPE == 3) {
            if (POWER == int(POWER))
                znew = c_powi(dvec2(-abs(z.x), z.y), int(POWER)) + c;
            else
                znew = c_powre(z, POWER) + c;
        } else if (FRACTAL_TYPE == 4) {
            if (POWER == int(POWER))
                znew = dvec2(c_powi(z, int(POWER)).x, -(c_powi(dvec2(z.x, abs(z.y)), int(POWER))).y) + c;
            else
                znew = c_powim(z, POWER) + c;
        } else if (FRACTAL_TYPE == 5) {
            if (POWER == int(POWER))
                znew = dvec2(abs(c_powi(z, int(POWER)).x), c_powi(z, int(POWER)).y) + c;
            else
                znew = c_powcel(z, POWER) + c;
        } else if (FRACTAL_TYPE == 6) {
            if (POWER == int(POWER))
                znew = dvec2(abs(c_powi(z, int(POWER)).x), -(c_powi(z, int(POWER)).y)) + c;
            else
                znew = dvec2(c_powcel(z, POWER).x, -c_powcel(z, POWER).y) + c;
        } else if (FRACTAL_TYPE == 7) {
            if (POWER == int(POWER))
                znew = dvec2(abs(c_powi(z, int(POWER))).x, -(c_powi(dvec2(abs(z.x), z.y), int(POWER))).y) + c;
            else
                znew = dvec2(c_powcel(z, POWER).x, -(c_pow(dvec2(abs(z.x), z.y), POWER)).y) + c;
        } else if (FRACTAL_TYPE == 8) {
            if (POWER == int(POWER))
                znew = dvec2(abs(c_powi(z, int(POWER)).x), -abs(c_powi(z, int(POWER)).y)) + c;
            else
                znew = dvec2(abs(c_pow(z, POWER)).x, -abs(c_pow(z, POWER)).y) + c;
        } else if (FRACTAL_TYPE == 9) {
            znew = c_powcelimi(z, int(POWER)) + c;
        } else if (FRACTAL_TYPE == 10) {
            if (POWER == int(POWER)) {
                if (mod(i,4)==0) {
                    znew = c_powi(z, int(POWER)) + c;
                }
                if (mod(i,4)==1) {
                    znew = dvec2(c_powi(z, int(POWER)).x, -abs(c_powi(z, int(POWER))).y) + c;
                }
                if (mod(i,4)==2) {
                    znew = c_powi(z, int(POWER)) + c;
                }
                if (mod(i,4)==3) {
                    znew = c_powi(z, int(POWER)) + c;
                }
            } else {
                if (mod(i,4)==0) {
                    znew = c_pow(z, POWER) + c;
                }
                if (mod(i,4)==1) {
                    znew = dvec2(c_pow(z, POWER).x, -abs(c_pow(z, POWER)).y) + c;
                }
                if (mod(i,4)==2) {
                    znew = c_pow(z, POWER) + c;
                }
                if (mod(i,4)==3) {
                    znew = c_pow(z, POWER) + c;
                }
            }
        } else if (FRACTAL_TYPE == 11) {
           if (POWER == int(POWER))
                znew = dvec2(c_powre(z, int(POWER)).x, -c_powre(dvec2(abs(z.x), z.y), int(POWER)).y) + c;
            else
                znew = dvec2(c_powre(z, POWER).x, -c_powre(z, POWER).y) + c;
        } else if (FRACTAL_TYPE == 12) {
            //znew = c_powcel(abs(z.x), z.y, POWER) + c;
            if (POWER == int(POWER))
                znew = dvec2(abs(c_powceli(dvec2(abs(z.x), z.y), int(POWER)).x), c_powceli(dvec2(abs(z.x), z.y), int(POWER)).y) + c;
            else
                znew = dvec2(c_powcel(dvec2(z.x, z.y), POWER).x, c_powcel(dvec2(abs(z.x), z.y), POWER).y) + c;
        } else if (FRACTAL_TYPE == 13) {
            znew = c_times(c, c_times(z, dvec2(1.0, 0.0)-z));
        } else if (FRACTAL_TYPE == 14) {
            // None
        }

        z = znew;
        if (distanceEstimation)
            dz = dznew;
        if(dot(z, z) > threshold) {
            escaped = true;
            break;
        }
        if (dot(z, z) < threshold && i == itr - 1) {
            return double(0.0);
        }
        n++;
    }

    if (smoothColoring) {
        double sl = n - log2(log2(float(dot(z,z)))) + 4.0;

        double al = smoothstep(-0.1, 0.0, sin(0.5*6.2831));
        n = mix(n, sl, al);
    }

    if (distanceEstimation) {
        double h2 = 0.75;
        double angle = 45.0;
        vec2 v = exp(vec2(c_div(c_times(dvec2(0.0, 1.0), c_times(dvec2(angle, 0.0), c_times(dvec2(2.0, 0.0), dvec2(3.14159265358, 0.0)))), dvec2(360.0, 0.0))));
        dvec2 u = c_div(z, dz);
        u = c_div(u, dvec2(dot(u,u), 0.0));
        double t = u.x*v.x+u.y*v.y;
        t = t/(1.0+h2);
        if (t < 0.0) t = 0.0;
        n = t/(zoom/8.0);
    }

    return n / float(itr);
}

double julia(dvec2 z) {
    dvec2 c = dvec2(Start.x, Start.y);

    if (smoothColoring) {
        double c2 = dot(c, c);
    }

    const double B = 256.0;
    double l = 0.0;

    for (int i = 0; i < itr; i++) {
        dvec2 znew;

        double zrsqr = z.x*z.x;
        double zisqr = z.y*z.y;
        double zrzrsqr = zrsqr*zrsqr;
        double zizisqr = zisqr*zisqr;
        double zrzisqr = zrsqr*zisqr;

        //dvec2 zc = c_pow(z.x, z.y, POWER);
        //dvec2 zabsr = c_pow(-abs(z.x), z.y, POWER);
        //dvec2 zabsi = c_pow(z.x, -abs(z.y), POWER);
    
        if (FRACTAL_TYPE == 0) {
            if (POWER == int(POWER))
                znew = c_powi(z, int(POWER)) + c;
            else
                znew = c_pow(z, POWER) + c;
        } else if (FRACTAL_TYPE == 1) {
            if (POWER == int(POWER))
                znew = dvec2(c_powi(z, int(POWER)).x, -(c_powi(z, int(POWER))).y) + c;
            else
                znew = dvec2(c_pow(z, POWER).x, -c_pow(z, POWER).y) + c;
        } else if (FRACTAL_TYPE == 2) {
            if (POWER == int(POWER))
                znew = dvec2(c_powi(z, int(POWER)).x, -abs(c_powi(z, int(POWER))).y) + c;
            else
                znew = dvec2(c_pow(z, POWER).x, -abs(c_pow(z, POWER)).y) + c;
        } else if (FRACTAL_TYPE == 3) {
            if (POWER == int(POWER))
                znew = c_powi(dvec2(-abs(z.x), z.y), int(POWER)) + c;
            else
                znew = c_powre(z, POWER) + c;
        } else if (FRACTAL_TYPE == 4) {
            if (POWER == int(POWER))
                znew = dvec2(c_powi(z, int(POWER)).x, -(c_powi(dvec2(z.x, abs(z.y)), int(POWER))).y) + c;
            else
                znew = c_powim(z, POWER) + c;
        } else if (FRACTAL_TYPE == 5) {
            if (POWER == int(POWER))
                znew = dvec2(abs(c_powi(z, int(POWER)).x), c_powi(z, int(POWER)).y) + c;
            else
                znew = c_powcel(z, POWER) + c;
        } else if (FRACTAL_TYPE == 6) {
            if (POWER == int(POWER))
                znew = dvec2(abs(c_powi(z, int(POWER)).x), -(c_powi(z, int(POWER)).y)) + c;
            else
                znew = c_powcel(dvec2(z.x, -z.y), POWER) + c;
        } else if (FRACTAL_TYPE == 7) {
            if (POWER == int(POWER))
                znew = dvec2(abs(c_powi(z, int(POWER))).x, -(c_powi(dvec2(abs(z.x), z.y), int(POWER))).y) + c;
            else
                znew = dvec2(c_powcel(z, POWER).x, -(c_pow(dvec2(abs(z.x), z.y), POWER)).y) + c;
        } else if (FRACTAL_TYPE == 8) {
            if (POWER == int(POWER))
                znew = dvec2(c_powcel(z, int(POWER)).x, -c_powcelrei(z, int(POWER)).y) + c;
            else
                znew = dvec2(abs(c_pow(z, POWER)).x, -abs(c_pow(z, POWER)).y) + c;
        } else if (FRACTAL_TYPE == 9) {
            znew = c_powcelimi(z, int(POWER)) + c;
        } else if (FRACTAL_TYPE == 10) {
            if (POWER == int(POWER)) {
                if (mod(i,4)==0) {
                    znew = c_powi(z, int(POWER)) + c;
                }
                if (mod(i,4)==1) {
                    znew = dvec2(c_powi(z, int(POWER)).x, -abs(c_powi(z, int(POWER))).y) + c;
                }
                if (mod(i,4)==2) {
                    znew = c_powi(z, int(POWER)) + c;
                }
                if (mod(i,4)==3) {
                    znew = c_powi(z, int(POWER)) + c;
                }
            } else {
                if (mod(i,4)==0) {
                    znew = c_pow(z, POWER) + c;
                }
                if (mod(i,4)==1) {
                    znew = dvec2(c_pow(z, POWER).x, -abs(c_pow(z, POWER)).y) + c;
                }
                if (mod(i,4)==2) {
                    znew = c_pow(z, POWER) + c;
                }
                if (mod(i,4)==3) {
                    znew = c_pow(z, POWER) + c;
                }
            }
        } else if (FRACTAL_TYPE == 11) {
           if (POWER == int(POWER))
                znew = dvec2(c_powre(z, int(POWER)).x, -c_powre(dvec2(abs(z.x), z.y), int(POWER)).y) + c;
            else
                znew = dvec2(c_powre(z, POWER).x, -c_powre(z, POWER).y) + c;
        } else if (FRACTAL_TYPE == 12) {
            //znew = c_powcel(abs(z.x), z.y, POWER) + c;
            if (POWER == int(POWER))
                znew = dvec2(abs(c_powceli(dvec2(abs(z.x), z.y), int(POWER)).x), c_powceli(dvec2(abs(z.x), z.y), int(POWER)).y) + c;
            else
                znew = dvec2(c_powcel(dvec2(z.x, z.y), POWER).x, c_powcel(dvec2(abs(z.x), z.y), POWER).y) + c;
        } /*else if (FRACTAL_TYPE == 13) {
            znew.x = z.x*z.x - z.y*z.y + c.x;
            znew.y = 2.0 * z.x  * z.y + c.y;
        }*/

        z = znew;
        if(dot(z, z) > threshold) {
            escaped = true;
            break;
        }
        n++;
    }

    if (smoothColoring) {
        double sl = n - log2(log2(float(dot(z,z)))) + 4.0;

        double al = smoothstep(-0.1, 0.0, sin(0.5*6.2831));
        n = mix(n, sl, al);
    }

    return n / float(itr);
}

double mandelInv(dvec2 c) {
    return mandelbrot(c_div(dvec2(1.0, 0.0), c));
}

double mandelbrot3d(dvec2 c, float zlayer) {
    dvec3 z = vec3(Start.x, Start.y, zlayer);
    for (int i = 0; i < itr; i++) {
        dvec3 znew;

        znew.x = (z.x*z.x)-(z.y*z.y)-(z.z*z.z)+c.x;
        znew.y = (2.0*z.x*z.y)+c.y;
        znew.z = (-2.0*z.x*zlayer);

        z = znew;

        if ((z.x*z.x)+(z.y*z.y)+(z.z*z.z)>threshold) break;
        n++;
    }

    return n / float(itr);
}

float x = 1.0 / 9.0;

vec3 mapToColor(float t, vec3 c1, vec3 c2, vec3 c3, vec3 c4, vec3 c5, vec3 c6, vec3 c7, vec3 c8, vec3 c9, vec3 c10, vec3 c11) {
    if (t < x) return mix(c1, c2, t/x);
    else if (t < 2.0 * x) return mix(c2, c3, (t - x)/x);
    else if (t < 3.0 * x) return mix(c3, c4, (t - 2.0*x)/x);
    else if (t < 4.0 * x) return mix(c4, c5, (t - 3.0*x)/x);
    else if (t < 5.0 * x) return mix(c5, c6, (t - 4.0*x)/x);
    else if (t < 6.0 * x) return mix(c6, c7, (t - 5.0*x)/x);
    else if (t < 7.0 * x) return mix(c7, c8, (t - 6.0*x)/x);
    else if (t < 8.0 * x) return mix(c8, c9, (t - 7.0*x)/x);
    else if (t < 9.0 * x) return mix(c9, c10, (t - 8.0*x)/x);
    else if (t < 10.0 * x) return mix(c10, c11, (t - 9.0*x)/x);

    return c10;
}

bool glitchedPixel = false; 
double mandelbrotPerturbation(dvec2 c, dvec2 dc) {
    dvec2 z = dvec2(Start.x, Start.y);
    dvec2 dz = dvec2(Start.x, Start.y);
    dvec2 glitchedC = dvec2(0.0, 0.0);
    for (int i = 0; i < itr; i++) {
        dvec2 dznew;
        dvec2 znew;
        if (FRACTAL_TYPE == 0) {
            dz = c_times(2.0*z+dz,dz) + dc;
            //dz = c_times(dvec2(2.0, 0.0), c_times(z, dn)) + c_powi(dn, 2) + dc;
            z = c_times(z, z) + c;
            if (glitchedPixel) {
                dz = c_times(2.0*z+dz, dz) + glitchedC;
                z = c_times(z, z) + c;
            }
        } else if (FRACTAL_TYPE == 1) {
            dznew.x = 2.0*z.x*dz.x+(dz.x*dz.x)-(dz.y*dz.y)-2.0*dz.y*z.y + dc.x;
            dznew.y = dc.y - 2.0*(z.x*dz.y+dz.x*z.y+dz.x*dz.y);
            znew.x = z.x*z.x - z.y*z.y + c.x;
            znew.y = c.y - (2.0*z.x*z.y);
            dz = dznew;
            z = znew;
        } else if (FRACTAL_TYPE == 2) {
            dznew.x = 2.0*z.x*dz.x+(dz.x*dz.x)-(dz.y*dz.y)-2.0*dz.y*z.y + dc.x;
            dznew.y = dc.y - 2.0*diffabs(z.x*z.y,z.x*dz.y+dz.x*z.y+dz.x*dz.y);
            znew.x = z.x*z.x - z.y*z.y + c.x;
            znew.y = -2.0*abs(z.x*z.y)+c.y;
            dz = dznew;
            z = znew;
        }
        //dz = dznew;
        //z = znew;
        if (dot(z+dz, z+dz) > threshold) {
            escaped = true;
            break;
        }
        if (dot(z+dz, z+dz)/dot(z, z) < 1e-6) {
            glitchedPixel = true;
            glitchedC = z + dz;
            break;
        }
        n++;
    }
    return n/float(itr);
}

dvec2[100] deepZoomPoint(int depth) {
    dvec2[100] v;
    double xn_r = offset.x;
    double xn_i = offset.y;

    for (int i = 0; i != depth; i++) {
        double re = xn_r + xn_r;
        double im = xn_i + xn_i;

        dvec2 c = dvec2(re, im);
        v[i] = c;

        if (re > 1024.0 || im > 1024.0 || re < -1024.0 || im < -1024.0)
            return v;
        
        xn_r = xn_r * xn_r - xn_i * xn_i + offset.x;
        xn_i = re * xn_i + offset.y;
    }
    return v;
}

double mandelbrot128(dvec2 cx, dvec2 cy) {
    dvec4 z = dvec4(0.0);
    double c2 = dot(cx.x, cy.x);
    const double B = 256.0;
    double l = 0.0;
    for (int i = 0; i < itr; i++) {
        dvec4 znew;
        if (FRACTAL_TYPE == 0) {
            znew.xy = add(sub(mul(z.xy, z.xy), mul(z.zw, z.zw)), cx);
            znew.zw = add(add(mul(z.xy, z.zw), mul(z.xy, z.zw)), cy);
        } else if (FRACTAL_TYPE == 1) {
            znew.xy = add(sub(mul(z.xy, z.xy), mul(z.zw, z.zw)), cx);
            znew.zw = add(mul(set(-2.0), mul(z.xy,z.zw)), cy);
        } else if (FRACTAL_TYPE == 2) {
            znew.xy = add(sub(mul(z.xy, z.xy), mul(z.zw, z.zw)), cx);
            znew.zw = add(mul(set(-2.0), abs128(mul(z.xy,z.zw))), cy);
        } else if (FRACTAL_TYPE == 3) {
            znew.xy = add(sub(mul(z.xy, z.xy), mul(z.zw, z.zw)), cx);
            znew.zw = add(mul(set(-2.0), mul(abs128(z.xy), z.zw)), cy);
        } else if (FRACTAL_TYPE == 4) {
            znew.xy = add(sub(mul(z.xy, z.xy), mul(z.zw, z.zw)), cx);
            znew.zw = add(mul(set(-2.0), mul(z.xy, abs128(z.zw))), cy);
        }
        z = znew;
        if (cmp(add(mul(z.xy, z.xy), mul(z.zw, z.zw)), set(threshold)) == 1.0) {
            escaped = true;
            break;
        }
        n++;
    }

    double sl = n - log2(log2(float(dot(z.xz,z.xz)))) + 4.0;

    double al = smoothstep(-0.1, 0.0, sin(0.5*6.2831));
    n = mix(n, sl, al);

    return n/float(itr);
}

double mandelbrotPerturbation128(dvec2 cx, dvec2 cy, dvec2 dcx, dvec2 dcy) {
    dvec4 z = dvec4(set(Start.x), set(Start.y));
    dvec4 dz = dvec4(set(Start.x), set(Start.y));
    dvec4 glitchedC = dvec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < itr; i++) {
        dvec4 dznew;
        dvec4 znew;
        dznew = dvec4(add(c_times128(dvec4(add(mul(set(2.0),z.xy),dz.xy), add(mul(set(2.0),z.zw),dz.zw)), dz).xy, dcx), add(c_times128(dvec4(add(mul(set(2.0),z.xy),dz.xy), add(mul(set(2.0),z.zw),dz.zw)), dz).zw, dcy));
        znew.xy = add(sub(mul(z.xy, z.xy), mul(z.zw, z.zw)), cx);
        znew.zw = add(add(mul(z.xy, z.zw), mul(z.xy, z.zw)), cy);
        dz = dznew;
        z = znew;
        if (cmp(mul(mul(add(z.xy,dz.xy), add(z.xy,dz.xy)),mul(add(z.zw,dz.zw), add(z.zw,dz.zw))), set(threshold)) == 1.0) {
            escaped = true;
            break;
        }
        n++;
    }
    return n/float(itr);
}

int glitchedPixels = 0;

void main() {
    const dvec2 coord = dvec2(gl_FragCoord.xy);
    vec2 resolution = vec2(3.0, 3.0);
    vec2 direction = vec2(1.0, 0.5);
    vec2 off1 = vec2(1.3846153846) * direction;
    vec2 off2 = vec2(3.2307692308) * direction;
    dvec2 z = vec2(Start.x, Start.y);
    dvec2 p = (coord - screenSize / 2.0);
    dvec2 p2 = (coord - screenSize / 2.0) / zoom - offset;
    dvec2 znew;

    double t;
    if (!juliaEnabled) {
        dvec2 c = -offset;
        dvec2 dc = p / zoom;
        
        if (inverted) {
            t = mandelInv(c + dc);
        } else {
            //double t2 = mandelbrot(c + dc);
            if (useQuadPrec) {
                if (zoom >= 10e+15)
                    t = mandelbrot128(sub(mul(sub(set(coord.x), mul(set(screenSize.x), set(0.5))), set(1.0/zoom)), offset128.xy), sub(mul(sub(set(coord.y), mul(set(screenSize.y), set(0.5))), set(1.0/zoom)), offset128.zw));
                else
                    t = perturbationEnabled ? mandelbrotPerturbation(c, dc) : mandelbrot(c + dc);
            } else
                t = perturbationEnabled ? mandelbrotPerturbation(c, dc) : mandelbrot(c + dc);
        }
        dc += -offset - c;
    } else
        t = julia(((coord - screenSize / 2) / zoom) - offset);

    //double t3d;
    //t3d = mandelbrot3d(((coord - screenSize / 2) / zoom) - offset, 1.0);

    if (gl_FragCoord.x < 40)
        gl_FragColor = vec4(1.0);

    vec4 color = escaped && !glitchedPixel ? vec4(mapToColor(float(mod(t, itr)), vec3(palette[0]), vec3(palette[1]), vec3(palette[2]), vec3(palette[3]), vec3(palette[4]), vec3(palette[5]), vec3(palette[6]), vec3(palette[7]), vec3(palette[8]), vec3(palette[9]), vec3(palette[10])), 1.0) : vec4(inColor, 1.0);
    //color += escaped && !glitchedPixel ? vec4(mapToColor(float(mod(t, itr)), vec3(palette[0]), vec3(palette[1]), vec3(palette[2]), vec3(palette[3]), vec3(palette[4]), vec3(palette[5]), vec3(palette[6]), vec3(palette[7]), vec3(palette[8]), vec3(palette[9]), vec3(palette[10])), 1.0) : vec4(inColor, 1.0);

    //color.rgb += (t < 0.0) ? vec3(0.0) : 0.5 + 0.5*cos(vec3(pow(float(zoom), 0.22)*t*0.05) + vec3(3.0, 3.5, 4.0));

    gl_FragColor = color;
}
)"""";

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
    this->setCursor(Qt::CrossCursor);
    this->setAutoFillBackground(false);
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
    }

    program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infoLog << "\n";
    }
    
    //glUseProgram(program);
}

void GLWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
}

void GLWidget::paintGL() {
    float vertices[24] = {
        -1.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f, -1.0f,

        -1.0f,  1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f
    };

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(program);

    glUniform2d(glGetUniformLocation(program, "screenSize"), 550.0, 420.0);
    glUniform2d(glGetUniformLocation(program, "offset"), this->offsetX, this->offsetY);
    glUniform4d(glGetUniformLocation(program, "offset128"), offsetX128.hi, offsetX128.lo, offsetY128.hi, offsetY128.lo);
    glUniform1f(glGetUniformLocation(program, "useQuadPrec"), this->useQuadPrec);
    glUniform1d(glGetUniformLocation(program, "zoom"), this->zoom);
    glUniform1i(glGetUniformLocation(program, "itr"), this->itr);
    glUniform1i(glGetUniformLocation(program, "FRACTAL_TYPE"), this->curFractal);
    glUniform1f(glGetUniformLocation(program, "POWER"), this->power);
    glUniform2d(glGetUniformLocation(program, "Start"), this->StartX, this->StartY);
    glUniform1f(glGetUniformLocation(program, "juliaEnabled"), this->isJulia);
    glUniform1f(glGetUniformLocation(program, "perturbationEnabled"), this->perturbationEnabled);
    glUniform1f(glGetUniformLocation(program, "smoothColoring"), this->smoothColoring);
    glUniform1d(glGetUniformLocation(program, "bailout"), this->escRad);

    glUniform3f(glGetUniformLocation(program, "col1"), (float)col1R / 100.0f, (float)col1G / 100.0f ,(float)col1B / 100.0f);
    glUniform3f(glGetUniformLocation(program, "col2"), (float)col2R / 100.0f, (float)col2G / 100.0f, (float)col2B / 100.0f);
    glUniform3f(glGetUniformLocation(program, "col3"), (float)col3R / 100.0f, (float)col3G / 100.0f, (float)col3B / 100.0f);
    glUniform3f(glGetUniformLocation(program, "col4"), (float)col4R / 100.0f, (float)col4G / 100.0f, (float)col4B / 100.0f);
    glUniform3f(glGetUniformLocation(program, "col5"), (float)col5R / 100.0f, (float)col5G / 100.0f, (float)col5B / 100.0f);
    glUniform3f(glGetUniformLocation(program, "col6"), (float)col6R / 100.0f, (float)col6G / 100.0f, (float)col6B / 100.0f);
    glUniform3f(glGetUniformLocation(program, "col7"), (float)col7R / 100.0f, (float)col7G / 100.0f, (float)col7B / 100.0f);
    glUniform3f(glGetUniformLocation(program, "col8"), (float)col8R / 100.0f, (float)col8G / 100.0f, (float)col8B / 100.0f);
    glUniform3f(glGetUniformLocation(program, "col9"), (float)col9R / 100.0f, (float)col9G / 100.0f, (float)col9B / 100.0f);
    glUniform3f(glGetUniformLocation(program, "inColor"), (float)inColorR / 255.0f, (float)inColorG / 255.0f, (float)inColorB / 255.0f);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glDeleteProgram(program);
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    double dx = (event->pos().x() - 550.0 / 2.0) / zoom - offsetX;
    double dy = (420.0 - event->pos().y() - 420.0 / 2.0) / zoom - offsetY;

    offsetX = -dx;
    offsetY = -dy;

    if (event->angleDelta().y() < 0)
        zoom /= 1.3;
    else
        zoom *= 1.3;
    
    dx = (event->pos().x() - 550.0 / 2.0) / zoom;
    dy = (420.0 - event->pos().y() - 420.0 / 2.0) / zoom;
    offsetX += dx;
    offsetY += dy;

    update();

    glUniform1d(glGetUniformLocation(program, "zoom"), this->zoom);
    glUniform2d(glGetUniformLocation(program, "offset"), this->offsetX, this->offsetY);
    glUniform4d(glGetUniformLocation(program, "offset128"), offsetX128.hi, offsetX128.lo, offsetY128.hi, offsetY128.lo);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    update();

    if (event->button() == Qt::LeftButton) {
        dragging = true;
        draggingParam = false;

        oldX = event->pos().x();
        oldY = event->pos().y();
    }

    if (event->button() == Qt::RightButton && isJulia) {
        draggingParam = true;
        dragging = false;

        oldStartX = event->pos().x();
        oldStartY = event->pos().y();
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    update();

    if (dragging) {
        offsetX += (event->pos().x() - oldX) / zoom;
        offsetY += (oldY - event->pos().y()) / zoom;

        oldX = event->pos().x();
        oldY = event->pos().y();

        glUniform2d(glGetUniformLocation(program, "offset"), this->offsetX, this->offsetY);
    }

    if (draggingParam && isJulia) {
        StartX += (event->pos().x() - oldStartX) / zoom;
        StartY += (oldStartY - event->pos().y()) / zoom;

        oldStartX = event->pos().x();
        oldStartY = event->pos().y();

        glUniform2d(glGetUniformLocation(program, "Start"), this->StartX, this->StartY);
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        offsetX += event->pos().x() - oldX;
        offsetY += oldY - event->pos().y();

        dragging = false;
        draggingParam = false;
    }

    if (draggingParam && isJulia) {
        dragging = false;
        draggingParam = false;
    }

    update();
}