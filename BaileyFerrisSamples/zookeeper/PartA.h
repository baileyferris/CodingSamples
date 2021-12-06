#ifndef PartA_H
#define PartA_H

#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>

#include "Cage.h"

using namespace std;

class Mst{
public:

    Mst() : totalDist{0} {}

    Mst(vector<Cage> &c_in) : cages{c_in}, totalDist{0} {}

    double runMST(){
        vector<bool> visited(cages.size(), false);
        Cage current = cages[0];
        size_t currIndex = 0;
        visited[0] = true;
        cages[0].distance = 0;

        while( !(all_of(visited.begin(), visited.end(), [](bool v){ return v; })) ){
            double minDist = numeric_limits<double>::infinity();

            //update distances
            for(size_t i = 1; i < cages.size(); ++i){
                if(visited[i] == false && 
                  (cages[i].type == current.type || cages[i].type == 'b' || current.type == 'b')){
                    double currDist = current.findDist(cages[i]);
                    if(currDist < cages[i].distance){
                        cages[i].distance = currDist;
                        cages[i].previous = currIndex;
                    }
                }
            }
            //find next node(smallest dist) to visit
            for(size_t i = 1; i < cages.size(); ++i){
                if(visited[i] == false && cages[i].distance < minDist){
                    minDist = cages[i].distance;
                    currIndex = i;
                }
            }
            totalDist += cages[currIndex].distance;
            visited[currIndex] = true;
            current = cages[currIndex];
        }
        return totalDist;
    }

    void printMST(ostringstream &os){
        os << totalDist << endl;
        for(size_t i = 1; i < cages.size(); ++i){
            if(i < cages[i].previous){
                os << i << " " << cages[i].previous << endl;
            } else {
                os << cages[i].previous << " " << i << endl;
            }
        }
    }

private:
    vector<Cage> cages;
    double totalDist;
};

#endif // PartA_H
