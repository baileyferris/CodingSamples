#ifndef PartB_H
#define PartB_H

#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>

#include "Cage.h"

using namespace std;

class Fast{
public:

    Fast(vector<Cage> &c_in) : cages{c_in}, totalDist{0} {}

    Fast(vector<Cage> &c_in, vector<size_t> &p_in) : cages{c_in}, path{p_in}, totalDist{0} {} 

    double runFASTTSP(){
        vector<bool> visited(cages.size(), false);
        path.push_back(0);
        visited[0] = true;
        Cage current = cages[0];
        size_t nextIndex;

        //nearest neighbor
        while(!(all_of(visited.begin(), visited.end(), [](bool v){ return v; }))){

            double minDist = numeric_limits<double>::infinity();

            for(size_t i = 1; i < cages.size(); ++i){
                double currDist = current.findDist(cages[i]);
                if(visited[i] == false && currDist < minDist){
                    minDist = currDist;
                    nextIndex = i;
                }
            }
            path.push_back(nextIndex);
            visited[nextIndex] = true;
            current = cages[nextIndex];
            totalDist += minDist;
        }
        //connect last visited to the beginning
        totalDist += current.findDist(cages[0]);

        //2-opt
        for(size_t i = 0; i < path.size() - 1; ++i){
            double currDist;
            double newDist;
            for(size_t j = i + 1; j < path.size() - 1; ++j){
                currDist = cages[path[i]].findDist(cages[path[i + 1]]) + cages[path[j]].findDist(cages[path[j + 1]]);
                newDist = cages[path[i]].findDist(cages[path[j]]) + cages[path[i + 1]].findDist(cages[path[j + 1]]);
                if(newDist != 0 && currDist > newDist){
                    reverse(path.begin() + i + 1, path.begin() + j + 1);
                    totalDist -= currDist - newDist;
                }
                currDist = cages[path[i]].findDist(cages[path[i + 1]]) + cages[path[path.size() - 1]].findDist(cages[path[0]]);
                newDist = cages[path[i]].findDist(cages[path[path.size() - 1]]) + cages[path[i + 1]].findDist(cages[path[0]]);
                if(currDist > newDist){
                    reverse(path.begin() + i + 1, path.end());
                    totalDist -= currDist - newDist;
                }
            }
        }
        return totalDist;
    }

    void printPath(ostringstream &os){
        os << totalDist << endl;
        for(size_t i = 0; i < path.size() - 1; ++i){
            os << path[i] << " ";
        }
        os << path[path.size() - 1] << endl;
    }

    vector<size_t> getPath(){
        return path;
    }

private:
    vector<Cage> &cages;
    vector<size_t> path;
    double totalDist;
};

#endif // PartB_H
