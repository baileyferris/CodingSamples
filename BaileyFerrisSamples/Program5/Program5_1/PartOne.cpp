#include <fstream>
#include <vector>
#include <string>
#include <utility>

// Ferris, Bailey

/*!
 *	\brief Opens the file <filename> and returns the characters
 *	in the file as a vector of ascii characters.
 *
 *	\param filename
 *	Name of the file to open.
 *
 *	\return
 *	Returns a vector of char, int pairs containing all the frequency of 
 *	characters within the file in ASCII order.
 */
std::vector<std::pair<char,int>> calculateFrequencies(const std::string &filename){
	const int SAMPLE_SIZE = 128;
	const int INITIAL_COUNT = 0;
	//create a set of 256 integers for counting occurrences of each character
	std::vector<int> sampleSpace(SAMPLE_SIZE, INITIAL_COUNT);
	std::ifstream inFile(filename, std::ios_base::in);
	while (!inFile.eof()) {
		char x;
		inFile.get(x);
		if(inFile.eof()){
			break;
		}
		++sampleSpace[(int) x];//increment count of found character

	}
	inFile.close();

	std::vector<std::pair<char, int>> outVect;
	for (int i = 0; i < SAMPLE_SIZE; ++i) {
		if (sampleSpace[i] != 0) {
			outVect.push_back(std::pair<char, int>((char)i, sampleSpace[i]));
		}
	}

	return outVect;
}

/*!
 *	\brief Writes the frequency file from freqs.  Writes
 *	'\n' as "LF" in the output file.
 *
 *	\param filename
 *	Name for the file to write.
 *
 *	\param freqs
 *	Vector of character integer pairs which
 *	contains the frequencies to write.
 */
void writeFreqFile(const std::string &filename, 
	const std::vector<std::pair<char, int>> &freqs){
	std::ofstream outFile(filename, std::ios_base::out);

	for (auto itr = freqs.begin(); itr != freqs.end(); ++itr) {
		std::string val = "LF";
		if(itr->first != '\n')
			val=itr->first;
		outFile << val << ' ' << itr->second << '\n';
	}

	outFile.close();
}

/*!
 *	\brief Main program execution
 *	Reads a text file "clear.txt" and creates 
 *	a text file "freq.txt" which contains the
 *	characters in "clear.txt" in ASCII order
 *	along with their frequencies, one per line.
 *	'\n' is represented as "LF" in this file.
 */
int main(int argc, char* argv[]){
	const std::string IN_FILE= "clear.txt";
	std::vector<std::pair<char, int>> freqs=calculateFrequencies(IN_FILE);
	const std::string OUT_FILE = "freq.txt";
	writeFreqFile(OUT_FILE, freqs);

	return 0;
}