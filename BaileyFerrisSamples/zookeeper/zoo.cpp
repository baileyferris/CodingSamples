#include <iostream>
#include <getopt.h>
#include <iomanip>
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>

#include "Cage.h"
#include "PartA.h"
#include "PartB.h"
#include "PartC.h"


using namespace std;


int main(int argc, char * argv[]){
    ios_base::sync_with_stdio(false);

    ostringstream os;

    cout << std::setprecision(2); // Always show 2 decimal places
    cout << std::fixed; // Disable scientific notation for large numbers

    os << std::setprecision(2);
    os << std::fixed;

    string mode;

    //These are used with getopt_long()
    opterr = true; // Give us help with errors
    int choice;
    int option_index = 0;
    option long_options[] = {
        { "mode",  required_argument,  nullptr, 'm'  },
        { "help",  no_argument,        nullptr, 'h'  },
        { nullptr, 0,                  nullptr, '\0' }
    };

    while ((choice = getopt_long(argc, argv, "m:h", long_options, &option_index)) != -1) {
        switch(choice) {
        case 'h':
            //TODO: write helpful message
            cout << "print help message" << endl;
            exit(0);

        case 'm':
            mode = optarg;
            if (mode != "MST" && mode != "OPTTSP" && mode != "FASTTSP") {
                cerr << "Error: invalid mode " << mode << endl;
                exit(1);
            } // if
            break;

        default:
            cerr << "Error: invalid option" << endl;
            exit(1);
        } // switch
    } // while

    size_t num_cages = 0;
    cin >> num_cages;

    vector<Cage> cages;
    Cage cage_in;
    int x;
    int y;
    if(mode == "MST"){
        bool seen_w = false;
        bool seen_s = false;
        bool seen_b = false;

        while(cin >> x >> y){
            cage_in.loc.first = x;
            cage_in.loc.second = y;
            if(x < 0 && y < 0){
                cage_in.type = 'w';
                if(!seen_w){
                    seen_w = true;
                }
            } else if((x == 0 && y <= 0) || (y == 0 && x <= 0)){
                cage_in.type = 'b';
                if(!seen_b){
                    seen_b = true;
                }
            } else {
                cage_in.type = 's';
                if(!seen_s){
                    seen_s = true;
                }
            }
            cages.push_back(cage_in);
        }
        if(!seen_b && seen_s && seen_w){
            cerr << "Cannot construct MST" << endl;
            exit(1);
        }
        Mst m(cages);
        m.runMST();
        m.printMST(os);
    } else if(mode == "FASTTSP"){
        while(cin >> x >> y){
            cage_in.loc.first = x;
            cage_in.loc.second = y;
            cages.push_back(cage_in);
        }
        Fast f(cages);
        f.runFASTTSP();
        f.printPath(os);
    } else {
        size_t id_in = 0;
        while(cin >> x >> y){
            cage_in.loc.first = x;
            cage_in.loc.second = y;
            cage_in.ID = id_in++;
            cages.push_back(cage_in);
        }
        Opt o(cages);
        o.runOPTTSP();
        o.printPath(os);
    }
    cout << os.str();

    return 0;
}


