/* 
 * Ferris, Bailey
 *
 * Program 5, Part Four A
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
    const size_t set_size = 8;
    bitset<set_size> bset;
    int bit_pos = set_size - 1;
    string code;

    while (!inFile.eof()) {
        char x;
        inFile.get(x);
        if(inFile.eof()){
            break;
        }
        for (auto itr = codeVect.begin(); itr != codeVect.end(); ++itr) {
            if (itr->first == x) {
                code = itr->second;
                for (auto itr2 = code.begin(); itr2 != code.end(); ++itr2){
                    if(bit_pos < 0){
                        outFile << (char)bset.to_ulong();
                        bset.reset();
                        bit_pos = set_size - 1;
                    }
                    if(*itr2 == '1'){
                        bset.set(bit_pos);
                    }
                    bit_pos--;
                }
            }
        } 
    }
    if(bit_pos != set_size - 1){
        outFile << (char)bset.to_ulong();
    }
    inFile.close();
    outFile.close();
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
void writeCodedFile2(const string& IN_FILE, const string& OUT_FILE,
	const vector<pair<char, string>>& codeVect) {

	ofstream outFile(OUT_FILE, ios_base::out|ios_base::binary);
	ifstream inFile(IN_FILE, ios_base::in);

	string code;
	CodeTree root(codeVect);
	std::map<char, CodeTree*> map = root.getLeaves();

	while (!inFile.eof()) {
		char x;
		inFile.get(x);
		if (inFile.eof()) {
			break;
		}

		code += map.at(x)->getStringCode();
		if (code.size() >= 8) {
			short outval = 0;
			for (int i = 0; i < 8; ++i) {
				outval <<= 1;
				if (code[i] == '1') {
					outval |= 1;
				}
			}
			outFile.put(outval);
			code = code.substr(8, std::string::npos);
		}
		
	}

	inFile.close();

	if (!code.empty()) {
		short outval = 0;
		for (int i = 0; i < code.size(); ++i) {
			outval <<= 1;
			if (code[i] == '1') {
				outval |= 1;
			}
		}

		outval <<= (8-code.size());
		outFile.put(outval);
	
	}
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
    const string OUT_FILE = "codedalt.txt";
    writeCodedFile2(IN_FILE, OUT_FILE, codes);

    return 0;
}