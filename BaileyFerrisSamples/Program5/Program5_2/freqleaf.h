#pragma once
#include "freqtree.h"

class FreqLeaf:public FreqTree{
public:
	/*!
	 *	\brief Constructs FreqTree leaf object from val and freq.
	 *
	 *	\param val
	 *	Character represented by leaf.
	 *	
	 *	\param freq
	 *	Frequency of character represented by leaf.
	 */
	FreqLeaf(const char& val, const unsigned int& freq);
	/*!
	 *	\brief Returns character represented by this leaf.
	 *
	 *	\return
	 *	Value of character this leaf represents.
	 */
	char getVal();
private:
	char m_val;
};