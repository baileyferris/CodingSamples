#include "freqtree.h"

FreqTree::FreqTree(const unsigned int& freq) :m_parent(0),
m_left(0), m_right(0), m_freq(freq) {}

FreqTree::FreqTree(FreqTree* const& left,
	FreqTree* const& right) :m_parent(0),
	m_left(left), m_right(right) {
	if (left != 0 && right != 0) {
		m_freq = left->m_freq + right->m_freq;
		left->m_parent = this;
		right->m_parent = this;
	}
	else {
		m_left = 0;
		m_right = 0;
		m_freq = 0;
	}
}

FreqTree::~FreqTree() {
	delete m_left;
	delete m_right;
}

FreqTree* FreqTree::getNodeFromCode(
	const std::string& code) {
	switch (code[0])
	{
	case 0:
		return this;
	case '0':
		if (m_left != 0) {
			return m_left->getNodeFromCode(code.substr(1, code.size()));
		}
		else {
			return this;
		}
		break;
	case '1':
		if (m_right != 0) {
			return m_right->getNodeFromCode(code.substr(1, code.size()));
		}
		else {
			return this;
		}
		break;
	default:
		break;
	}

	return 0;//If it gets here we are experiencing an error.
}
std::string FreqTree::getCode() {
	std::string out;
	if (m_parent != 0) {
		out = m_parent->getCode(this);
	}
	return out;
}
std::string FreqTree::getCode(FreqTree* child) {
	std::string out;
	if (m_parent != 0) {
		out = m_parent->getCode(this);
	}
	if (child == m_left) {
		out += "0";
	}
	else {
		out += "1";
	}

	return out;
}

unsigned int FreqTree::getDepthOfNode(){
	if (m_parent == 0) {
		return 0;
	}

	return 1 + m_parent->getDepthOfNode();
}

unsigned int FreqTree::getFreq() {
	return m_freq;
}