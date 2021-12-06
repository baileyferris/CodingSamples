#ifndef PartC_H
#define PartC_H

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <deque>

#include "Cage.h"
#include "PartA.h"
#include "PartB.h"

using namespace std;

class Opt{
public:

    Opt() : bestDist{numeric_limits<double>::infinity()}, currDist{0} {}

    Opt(vector<Cage> &c_in) : cages{c_in}, currDist{0} {
        for(size_t i = 0; i < cages.size(); ++i){
            vector<double> row;
            for(size_t j = 0; j < cages.size(); ++j){
                row.push_back(cages[i].findDist(cages[j]));
            }
            distMat.push_back(row);
        }
        Fast bound(cages);
        bestDist = bound.runFASTTSP();
        bestPath = bound.getPath();

        for(size_t i = 1; i < bestPath.size(); ++i){
            unvisited.push_back(cages[bestPath[i]]);
        } 
    }

    void runOPTTSP(){
        currPath.push_back(0);
        gen_perms();
    }

    bool promising(){
        if(currDist > bestDist){
            return false;
        }
        if(unvisited.size() < 4){
            return true;
        }

        vector<Cage> uvec;
        for(size_t i = 0; i < unvisited.size(); ++i){
            uvec.push_back(unvisited[i]);
        }

        Mst low_bound(uvec);
        double min = low_bound.runMST();

        double back = distMat[currPath.back()][unvisited[0].ID];
        double front = distMat[0][unvisited[0].ID];
        for(size_t i = 1; i < unvisited.size(); i++){
            if(distMat[currPath.back()][unvisited[i].ID] < back){
                back = distMat[currPath.back()][unvisited[i].ID];
            }
            if(distMat[0][unvisited[i].ID] < front){
                front = distMat[0][unvisited[i].ID];
            }
        }
        min = min + front + back;

        if(currDist + min > bestDist){
            return false;
        } else {
            return true;
        }
    }

    void gen_perms(){
        if(unvisited.empty()){
            double toadd = distMat[0][currPath.back()];
            currDist += toadd;
            if(currDist < bestDist){
                bestPath = currPath;
                bestDist = currDist;
            }
            currDist -= toadd;
            return;
        }
        if(!promising()){
            return;
        }
        for(unsigned k = 0; k != unvisited.size(); k++){
            double toadd = distMat[currPath.back()][unvisited.front().ID];
            currDist += toadd;
            currPath.push_back(unvisited.front().ID);
            unvisited.pop_front();

            gen_perms();

            unvisited.push_back(cages[currPath.back()]);
            currPath.pop_back();
            currDist -= toadd;
        }
    }

    void printPath(ostringstream &os){
        os << bestDist << endl;
        for(size_t i = 0; i < bestPath.size() - 1; ++i){
            os << bestPath[i] << " ";
        }
        os << bestPath.back() << endl;
    }

private:
    vector<Cage> cages;
    deque<Cage> unvisited;
    vector<vector<double>> distMat;
    vector<size_t> bestPath;
    vector<size_t> currPath;
    double bestDist; // init to result of FASTTSP
    double currDist;
};



#endif // PartC_H
