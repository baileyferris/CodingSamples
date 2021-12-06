#ifndef CARD_H
#define CARD_H
/* Card.h
 *
 * Represents a single playing card
 *
 * by Andrew DeOrio
 * awdeorio@umich.edu
 * 2014-12-21
 */

#include <iostream>
#include <string>


class Card {
 public:
  // rank and suit names
  static const std::string RANK_TWO;
  static const std::string RANK_THREE;
  static const std::string RANK_FOUR;
  static const std::string RANK_FIVE;
  static const std::string RANK_SIX;
  static const std::string RANK_SEVEN;
  static const std::string RANK_EIGHT;
  static const std::string RANK_NINE;
  static const std::string RANK_TEN;
  static const std::string RANK_JACK;
  static const std::string RANK_QUEEN;
  static const std::string RANK_KING;
  static const std::string RANK_ACE;
  
  static const std::string SUIT_SPADES;
  static const std::string SUIT_HEARTS;
  static const std::string SUIT_CLUBS;
  static const std::string SUIT_DIAMONDS;

  //REQUIRES rank is one of "Two", "Three", "Four", "Five", "Six", "Seven",
  //  "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"
  //  suit is one of "Spades", "Hearts", "Clubs", "Diamonds"  
  //EFFECTS Initializes Card to specified rank and suit
  Card(const std::string &rank_in, const std::string &suit_in);

  //EFFECTS Returns the rank
  std::string get_rank() const;

  //EFFECTS Returns the suit.  Does not consider trump.
  std::string get_suit() const;

  //EFFECTS Returns the suit
  //HINT: the left bower is the trump suit!
  std::string get_suit(const std::string &trump) const;

  //EFFECTS Returns true if card is a face card (Jack, Queen, King or Ace)
  bool is_face() const;

  //EFFECTS Returns true if card is the Jack of the trump suit
  bool is_right_bower(const std::string &trump) const;

  //EFFECTS Returns true if card is the Jack of the next suit
  bool is_left_bower(const std::string &trump) const;

  //EFFECTS Returns true if the card is a trump card.  All cards of the trump
  // suit are trump cards.  The left bower is also a trump card.
  bool is_trump(const std::string &trump) const;

 private:
  std::string rank;
  std::string suit;
};

//EFFECTS Returns true if lhs is lower value than rhs.
//  Does not consider trump.
bool operator<(const Card &lhs, const Card &rhs);

//EFFECTS Returns true if lhs is higher value than rhs.
//  Does not consider trump.
bool operator>(const Card &lhs, const Card &rhs);

//EFFECTS Returns true if lhs is same card as rhs.
//  Does not consider trump.
bool operator==(const Card &lhs, const Card &rhs);

//EFFECTS Returns true if lhs is not the same card as rhs.
//  Does not consider trump.
bool operator!=(const Card &lhs, const Card &rhs);

//EFFECTS returns the next suit, which is the suit of the same color
std::string Suit_next(const std::string &suit);

//EFFECTS Prints Card to stream, for example "Two of Spades"
std::ostream & operator<<(std::ostream &os, const Card &card);

//EFFECTS Returns true if a is lower value than b.  Uses trump to determine
// order, as described in the spec.
bool Card_less(const Card &a, const Card &b, const std::string &trump);

//EFFECTS Returns true if a is lower value than b.  Uses both the trump suit
//  and the suit led to determine order, as described in the spec.
bool Card_less(const Card &a, const Card &b, const Card &led_card, const std::string &trump);

#endif
