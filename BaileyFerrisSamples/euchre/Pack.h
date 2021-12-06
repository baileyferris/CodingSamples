#ifndef PACK_H
#define PACK_H
/* Pack.h
 *
 * Represents a pack of playing cards
 *
 * by Andrew DeOrio <awdeorio@umich.edu>
 * 2014-12-21
 */


#include "Card.h"
#include <vector>
#include <string>

class Pack {
 public:
  //EFFECTS Creates an empty pack
  Pack();

  //MODIFIES stdout
  //EFFECTS  Initializes Pack by reading from pack_filename.
  //  If file IO fails, prints an error message and calls exit(EXIT_FAILURE).
  Pack(const std::string &pack_filename);

  //REQUIRES cards remain in the Pack
  //EFFECTS  Returns the next card in the pack and increments the next pointer
  Card deal_one();

  //EFFECTS Resets next pointer to first card in the Pack
  void reset();

  //EFFECTS  Shuffles the Pack and resets the next pointer.  See the project
  // spec for a description of the shuffling procedure
  void shuffle();

  //EFFECTS returns true if there are no more cards left in the pack
  bool empty() const;

 private:
  static const int PACK_SIZE = 24;
  std::vector<Card> cards;
  int next; //index of next card to be dealt
};

#endif
