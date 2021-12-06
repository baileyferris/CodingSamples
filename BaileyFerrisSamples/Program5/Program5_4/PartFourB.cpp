/* 
 * Ferris, Bailey
 *
 * Program 5, Part Four B
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
#include <bitset>
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
 *  \brief Writes decoded version of IN_FILE to OUT_FILE
 *
 *  \param IN_FILE
 *  Name of file to decode.
 *
 *  \param OUT_FILE
 *  Name of file to write.

 *  \param codeVect
 *  Vector containing character, code as a string pairs
 */
void writeDeCodedFile2(const string& IN_FILE, const string& OUT_FILE,
	const vector<pair<char, string>>& codeVect) {

	ofstream outFile(OUT_FILE, ios_base::out);
	ifstream inFile(IN_FILE, ios_base::in|ios_base::binary);

	CodeTree root(codeVect);
	std::pair<bool, CodeTree*> pos(false, &root);

	string str = "";
	while (!inFile.eof()) {
		char x;
		inFile.get(x);

		if (inFile.eof()) {
			break;
		}
		short y = (short)x;
		for (int i = 0; i < 8; ++i) {
			bool val = 0x80 & y;
			y <<= 1;
			pos = pos.second->readCode(val);
			if (pos.first) {
				outFile << pos.second->getVal();
				pos.second = &root;
				pos.first = false;
			}
		}
	}
	inFile.close();
	outFile.close();
}
/*! 
 *  \brief Writes decoded version of IN_FILE to OUT_FILE
 *
 *  \param IN_FILE
 *  Name of file to decode.
 *
 *  \param OUT_FILE
 *  Name of file to write.

 *  \param codeVect
 *  Vector containing character, code as a string pairs
 */
void writeDeCodedFile(const string &IN_FILE, const string &OUT_FILE,
    const vector<pair<char, string>>& codeVect) {

    ofstream outFile(OUT_FILE, ios_base::out);
    ifstream inFile(IN_FILE, ios_base::in);
    const size_t set_size = 8;
    string str = "";
    while (!inFile.eof()) {
        char x;
        inFile.get(x);

        if(inFile.eof()){
            break;
        }

        bitset<set_size> bset(x);

        for(int i = set_size - 1; i >= 0; --i){
            str = str + to_string(bset[i]);
            for (auto itr = codeVect.begin(); itr != codeVect.end(); ++itr) {
                if (itr->second == str) {
                    outFile << itr->first;
                    str = "";
                }
            }
        }

        
        
    }
    inFile.close();
    outFile.close();
}


/*!
 *  \brief Main program execution.
 *  Reads char, code pairs from file and reads a text file called "codedalt.txt"
 *  and decodes it using "codetable.txt", and writes the decoded version to
 *  "decodedalt.txt"
 */
int main(int argc, char* argv[]){
    const string CODE_FILE= "codetable.txt";
    vector<pair<char, string> > codes = getCodes(CODE_FILE);

    const string IN_FILE= "codedalt.txt";
    const string OUT_FILE = "decodedalt.txt";
    writeDeCodedFile2(IN_FILE, OUT_FILE, codes);

    return 0;
}