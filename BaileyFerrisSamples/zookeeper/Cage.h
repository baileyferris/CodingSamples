#ifndef Cage_H
#define Cage_H

#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>

using namespace std;

class Cage{
public:
    pair<int, int> loc;
    double distance;
    size_t previous;
    size_t ID;
    char type; // w(wild), s(safe), b(border)

    Cage() : distance{numeric_limits<double>::infinity()}, previous{0}, ID{0}, type{'b'} {}

    Cage(pair<int, int> l_in, double d_in, size_t p_in, size_t ID_in, char t_in)
        : loc{l_in}, distance{d_in}, previous{p_in}, ID{ID_in}, type{t_in}
    {}

    double findDist(const Cage &right) const{
        if(loc.first == right.loc.first){
            return abs(loc.second - right.loc.second);
        } else if(loc.second == right.loc.second){
            return abs(loc.first - right.loc.first);
        } else {
            double a = abs(loc.first - right.loc.first);
            double b = abs(loc.second - right.loc.second);
            return sqrt((a * a) + (b * b));
        }
    }
};

#endif // Cage_H
