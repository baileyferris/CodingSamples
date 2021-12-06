#pragma once
#include <string>

class FreqTree {
public:
	/*!
	 *	\brief Constructs a FreqTree node using only the frequency.
	 */
	FreqTree(const unsigned int& freq);
	/*!
	 *	\brief Constructs a FreqTree node using both children.  
	 *	If either child is null, constructs empty node.
	 *
	 *	\param left
	 *	Left child.
	 *
	 *	\param right
	 *	Right child.
	 */
	FreqTree(FreqTree* const& left,
				   FreqTree* const& right);
	/*!
	 *	\brief Deletes node including child nodes.
	 */
	~FreqTree();

	/*!
	 *	\brief Gets node pointer from following code.
	 *	
	 *	\param code
	 *	Binary code represented as a string to follow from this position
	 *	to reach proper descendant.
	 *
	 *	\return 
	 *	Pointer to descendant identified by the code.
	 */
	FreqTree* getNodeFromCode(const std::string& code);
	/*!
	 *	\brief Gets depth of node from the root.
	 *
	 *	\return
	 *	Node depth from root.
	 *	i.e. root 0, child of root 1, etc.
	 */
	unsigned int getDepthOfNode();
	/*!
	 *	\brief Gets the code to reach this node from the root.
	 *
	 *	\return
	 *	Binary code to reach node from root represented as character string.
	 */
	std::string getCode();
	/*!
	 *	\brief Gets frequency of this node.
	 *
	 *	\return
	 *	This node's frequency.
	 */
	unsigned int getFreq();
private:
	/*!
	 *	\brief Gets code required to reach child from the root.
	 *
	 *	\param child
	 *	Child node (of this node) chosen as destination.
	 *
	 *	\return
	 *	Binary code to reach child node from the root.
	 */
	std::string getCode(FreqTree* child);
	FreqTree* m_parent;
	FreqTree* m_left;
	FreqTree* m_right;
	unsigned int m_freq;
};