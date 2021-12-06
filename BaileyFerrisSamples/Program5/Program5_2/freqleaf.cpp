#include "freqleaf.h"

FreqLeaf::FreqLeaf(const char& val, const unsigned int& freq):
	FreqTree(freq),m_val(val) {}
char FreqLeaf::getVal() {
	return m_val;
}