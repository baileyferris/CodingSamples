#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <iostream>
#include "freqleaf.h"

/*! 
 *	\brief Mergesort on values from [vectStart, vectEnd) with comparator cmp.
 *
 *	\param vectStart
 *	Beginning of vector subset to sort.
 *
 *	\param vectEnd
 *	Position after end of vector subset to sort.
 *
 *	\param cmp
 *	boolean returning comparator function such that
 *	cmp(a,a) = false
 *	cmp(a,b) = a<b
 *	cmp(a,b) =true->cmp(b,a) = false
 *	cmp(b,a) = true->cmp(a,b) = false
 *
 *	\return
 *	Returns sorted vector copy.
 */
std::vector<std::pair<char, int>> mergesort(
	std::vector<std::pair<char, int>>::iterator vectStart,
	std::vector<std::pair<char,int>>::iterator vectEnd,
	bool cmp(std::pair<char, int>, std::pair<char, int>)) {
	std::vector<std::pair<char, int>> out(vectEnd - vectStart);
	if (vectStart + 1 == vectEnd) {
		out[0] = *vectStart;
		return out;
	}

	auto mid = vectStart + (((int)(vectEnd-vectStart)+1)/ 2);
	auto left = mergesort(vectStart, mid,cmp);
	auto right = mergesort(mid, vectEnd, cmp);
	auto ltr = left.begin();
	auto rtr = right.begin();
	auto otr = out.begin();
	for (; ltr != left.end() && rtr != right.end();++otr) {
		if (cmp(*rtr, *ltr)) {
			*otr = *rtr;
			++rtr;
		}
		else {
			*otr = *ltr;
			++ltr;
		}
	}
	for (; ltr != left.end(); ++ltr, ++otr) {
		*otr = *ltr;
	}
	for (; rtr != right.end(); ++rtr, ++otr) {
		*otr = *rtr;
	}

	return out;
}

/*!
 *	\brief Mergesort on values from [vectStart, vectEnd) with comparator cmp.
 *
 *	\param vectStart
 *	Beginning of vector subset to sort.
 *
 *	\param vectEnd
 *	Position after end of vector subset to sort.
 *
 *	\param cmp
 *	boolean returning comparator function such that
 *	cmp(a,a) = false
 *	cmp(a,b) = a<b
 *	cmp(a,b) =true->cmp(b,a) = false
 *	cmp(b,a) = true->cmp(a,b) = false
 *
 *	\return
 *	Returns sorted vector copy.
 */
std::vector<FreqLeaf*> mergesort(
	std::vector<FreqLeaf*>::iterator vectStart,
	std::vector<FreqLeaf*>::iterator vectEnd,
	bool cmp(FreqLeaf*, FreqLeaf*)) {
	std::vector<FreqLeaf*> out(vectEnd - vectStart);
	if (vectStart + 1 == vectEnd) {
		out[0] = *vectStart;
		return out;
	}

	auto mid = vectStart + (((int)(vectEnd - vectStart) + 1) / 2);
	auto left = mergesort(vectStart, mid, cmp);
	auto right = mergesort(mid, vectEnd, cmp);
	auto ltr = left.begin();
	auto rtr = right.begin();
	auto otr = out.begin();
	for (; ltr != left.end() && rtr != right.end(); ++otr) {
		if (cmp(*rtr, *ltr)) {
			*otr = *rtr;
			++rtr;
		}
		else {
			*otr = *ltr;
			++ltr;
		}
	}
	for (; ltr != left.end(); ++ltr, ++otr) {
		*otr = *ltr;
	}
	for (; rtr != right.end(); ++rtr, ++otr) {
		*otr = *rtr;
	}

	return out;
}

/*!
 *	\brief Reads character frequency pairs from file and returns
 *	them as a vector.  LF in file is changed to '\n' in output.
 *
 *	\param filename
 *	name of file to read
 *
 *	\return
 *	vector of char, int pairs from the file
 */
std::vector<std::pair<char, int>> readFreqs(
	const std::string& filename) {
	std::ifstream inFile(filename, std::ios_base::in);

	std::vector<std::pair<char, int>> freqVect;

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

		int y = 0;
		inFile >> y;
		if (y > 0) {
			freqVect.push_back(std::pair<char, int>(x, y));
		}
		inFile.get(x);//clear the newline at the end of record
	}

	return freqVect;
}

/*! 
 *	\brief Writes codes in codevect to file filename.
 *
 *	\param filename
 *	Name of file to write.
 *
 *	\param codeVect
 *	Vector containing character, code as a string pairs to write.
 */
void writeCodes(const std::string &filename,
	const std::vector<std::pair<char, std::string>>& codeVect) {
	std::ofstream outFile(filename, std::ios_base::out);

	for (auto itr = codeVect.begin(); itr != codeVect.end(); ++itr) {
		if (itr->first == '\n') {
			outFile << "LF " << itr->second << '\n';
		}
		else {
			outFile << itr->first << ' ' << itr->second << '\n';
		}
	}

	outFile.close();
}

/*! 
 *	\brief Converts char,int pairs into a sorted vector of FreqLeaf pointers
 *	sorted on the frequency of the FreqLeaf pointed to by the pointer.
 *
 *	\param vect
 *	Vector of char, int pairs holding frequency of characters.
 *
 *	\return
 *	Sorted list of FreqLeaf pointers.
 */
std::vector<FreqLeaf*> getSortedVectorOfLeaves(
	const std::vector<std::pair<char, int>>& vect) {
	std::vector<std::pair<char, int>> copyVect(vect);
	copyVect=mergesort(copyVect.begin(), copyVect.end(), [](std::pair<char, int> lhs,
		std::pair<char, int> rhs) {
			return lhs.second < rhs.second;
		});
	std::vector<FreqLeaf*> leafList;
	for (auto itr = copyVect.begin(); itr != copyVect.end(); ++itr) {
		leafList.push_back(new FreqLeaf(itr->first, itr->second));
	}

	return leafList;
}

/*!
 *	\brief Generates codes from a vector of character, frequency pairs.
 *
 *	\param freqVect
 *	Vector of character, frequency pairs to use for code generation.
 *
 *	\return
 *	Vector of character, code pairs where codes are the string representation of binary strings.
 */
std::vector<std::pair<char, std::string>> generateCodes(
	const std::vector<std::pair<char,int>> &freqVect){
	//generate leaves from char, int pairs
	//and add leaves to node vector
	//sorted on frequency
	std::vector<FreqLeaf*> leaves = getSortedVectorOfLeaves(freqVect);
	//place nodes into node list in sorted order(was just sorted)
	std::list<FreqTree*> nodes;
	for (auto itr = leaves.begin(); itr != leaves.end(); ++itr) {
		nodes.push_back(*itr);
	}

	//while number of nodes in list > 1
	while (nodes.size() > 1) {
		//pop smallest 2 nodes, create parent from them
		FreqTree* nodeA=nodes.front();
		nodes.pop_front();
		FreqTree* nodeB=nodes.front();
		nodes.pop_front();
		//insert parent into list in sorted order
		//like insertion sort but iteration reversed
		FreqTree* parent=new FreqTree(nodeA, nodeB);
		auto itr = nodes.begin();
		while (itr != nodes.end() &&
			(*itr)->getFreq() < parent->getFreq()) {
			++itr;
		}
		nodes.insert(itr, parent);
	}
	//sort leaves by ascii order
	leaves = mergesort(leaves.begin(), leaves.end(),
		[](FreqLeaf* lhs, FreqLeaf* rhs) {
			return lhs->getVal() < rhs->getVal();
		});
	std::vector<std::pair<char, std::string>> out;
	//insert char, string pairs into out from leaves
	for (auto itr = leaves.begin(); itr != leaves.end(); ++itr) {
		out.push_back(std::pair<char,std::string>(
			(*itr)->getVal(), (*itr)->getCode()));
	}

	//Clean up the tree
	delete nodes.front();
	
	return out;
}

/*!
 *	\brief Main program execution.
 *	Reads char, frequency pairs from file and outputs file of char, code pairs.
 */
int main(int argc, char* argv[]){
	const std::string FREQ_FILE = "freq.txt";
	const std::string CODE_FILE = "codetable.txt";
	std::vector<std::pair<char, int>> freqVect = readFreqs(FREQ_FILE);
	std::vector<std::pair<char, std::string>> codeVect=
		generateCodes(freqVect);
	writeCodes(CODE_FILE,codeVect);
	return 0;
}