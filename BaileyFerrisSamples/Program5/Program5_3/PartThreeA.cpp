/*
 * Ferris, Bailey
 *
 * Program 5, Part Three A
 *
 *
 *
 */

#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <iostream>
#include "codetree.h"

using namespace std;


/*!
 *  \brief Opens the file <filename> and returns a 
 *  vector containing character, code as a string pairs
 *
 *  \param filename
 *  Name of the code file to open.
 *
 *  \return
 *  Returns a vector containing character, code as a string pairs
 */
vector<pair<char,string> > getCodes(const string &filename){
    vector<pair<char, string> > outVect;
    ifstream inFile(filename, ios_base::in);
    while (!inFile.eof()) {
        char x;
        inFile.get(x);
        if(inFile.eof()){
            break;
        }

        if (x == 'L') {
            inFile.get(x);
            if (x == 'F') {
                x = '\n';
            }
            else {
                x = 'L';
            }
        }

        string code;
        inFile >> code;
        outVect.push_back(pair<char, string>(x, code));

        inFile.get(x);//clear the newline at the end of record
    }
    inFile.close();

    return outVect;
}

/*! 
 *  \brief Writes coded version of IN_FILE to OUT_FILE
 *
 *  \param IN_FILE
 *  Name of file to code.
 *
 *  \param OUT_FILE
 *  Name of file to write.

 *  \param codeVect
 *  Vector containing character, code as a string pairs
 */
void writeCodedFile(const string &IN_FILE, const string &OUT_FILE,
    const vector<pair<char, string>>& codeVect) {

    ofstream outFile(OUT_FILE, ios_base::out);
    ifstream inFile(IN_FILE, ios_base::in);
	CodeTree root(codeVect);
	std::map<char, CodeTree*> map = root.getLeaves();

    while (!inFile.eof()) {
        char x;
        inFile.get(x);
        if(inFile.eof()){
            break;
        }

		outFile << map.at(x)->getStringCode();
		/**
        for (auto itr = codeVect.begin(); itr != codeVect.end(); ++itr) {
            if (itr->first == x) {
                outFile << itr->second;
            }
        } 
		*/
    }
    inFile.close();
    outFile.close();
}


/*!
 *  \brief Main program execution.
 *  Reads char, code pairs from file and reads a text file called "clear.txt"
 *  and outputs a coded version to a text file "coded.txt"
 */
int main(int argc, char* argv[]){
    const string CODE_FILE= "codetable.txt";
    vector<pair<char, string> > codes = getCodes(CODE_FILE);

    const string IN_FILE= "clear.txt";
    const string OUT_FILE = "coded.txt";
    writeCodedFile(IN_FILE, OUT_FILE, codes);

    return 0;
}