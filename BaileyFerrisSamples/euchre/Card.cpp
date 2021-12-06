/*
 * Bailey Ferris
 * Project 4 - Euchre
 * Card.cpp
 *
 */

#include "Card.h"
#include <string>

using namespace std;

const string Card::RANK_TWO = "Two";
const string Card::RANK_THREE = "Three";
const string Card::RANK_FOUR = "Four";
const string Card::RANK_FIVE = "Five";
const string Card::RANK_SIX = "Six";
const string Card::RANK_SEVEN = "Seven";
const string Card::RANK_EIGHT = "Eight";
const string Card::RANK_NINE = "Nine";
const string Card::RANK_TEN = "Ten";
const string Card::RANK_JACK = "Jack";
const string Card::RANK_QUEEN = "Queen";
const string Card::RANK_KING = "King";
const string Card::RANK_ACE = "Ace";

const string Card::SUIT_SPADES = "Spades";
const string Card::SUIT_HEARTS = "Hearts";
const string Card::SUIT_CLUBS = "Clubs";
const string Card::SUIT_DIAMONDS = "Diamonds";

////////////////////////////////////////////////////////////////////////////////
// Suits in order from lowest suit to highest suit.
static const string SUIT_NAMES_BY_WEIGHT[] = {
  Card::SUIT_SPADES,
  Card::SUIT_HEARTS,
  Card::SUIT_CLUBS,
  Card::SUIT_DIAMONDS
};
static const int NUM_SUITS = 4;

// Ranks in order from lowest rank to highest rank.
static const string RANK_NAMES_BY_WEIGHT[] = {
  Card::RANK_TWO,
  Card::RANK_THREE,
  Card::RANK_FOUR,
  Card::RANK_FIVE,
  Card::RANK_SIX,
  Card::RANK_SEVEN,
  Card::RANK_EIGHT,
  Card::RANK_NINE,
  Card::RANK_TEN,
  Card::RANK_JACK,
  Card::RANK_QUEEN,
  Card::RANK_KING,
  Card::RANK_ACE
};
static const int NUM_RANKS = 13;

Card::Card(const string &rank_in, const string &suit_in) {
  rank = rank_in;
  suit = suit_in;
}

string Card::get_rank() const {
  return rank;
}

string Card::get_suit() const {
  return suit;
}

string Card::get_suit(const string &trump) const {
  if(rank == RANK_JACK) {
    if(suit == Suit_next(trump))
      return trump;
  }
  return suit;
}

bool Card::is_face() const {
  if (rank == RANK_JACK || rank == RANK_QUEEN || rank == RANK_KING || rank == RANK_ACE) {
    return true;
  }
  return false;
}

bool Card::is_right_bower(const string &trump) const {
  if (rank == RANK_JACK && suit == trump) {
    return true;
  }
  return false;
}

bool Card::is_left_bower(const string &trump) const {
  if (rank == RANK_JACK && suit == Suit_next(trump)) {
    return true;
  }
  return false;
}

bool Card::is_trump(const string &trump) const {
  if (suit == trump || is_left_bower(trump)) {
    return true;
  }
  return false;
}

bool operator<(const Card &lhs, const Card &rhs) {
  //consider ranks first
  int lhs_rank;
  int rhs_rank;

  //determine index of the ranks of each card
  for (int j = 0; j < NUM_RANKS; ++j) {
    if (lhs.get_rank() == RANK_NAMES_BY_WEIGHT[j]) {
      lhs_rank = j;
    }
    if (rhs.get_rank() == RANK_NAMES_BY_WEIGHT[j]) {
      rhs_rank = j;
    } 
  }
  //compare rank indexes
  if (lhs_rank < rhs_rank) {
    return true;
  }
  //if cards have the same rank, compare suits
  else if (lhs_rank == rhs_rank) {
    int lhs_suit;
    int rhs_suit;

    //determine index of the suits of each card
    for(int i = 0; i < NUM_SUITS; ++i) {
      if (lhs.get_suit() == SUIT_NAMES_BY_WEIGHT[i]) {
        lhs_suit = i;
      }
      if (rhs.get_suit() == SUIT_NAMES_BY_WEIGHT[i]) {
        rhs_suit = i;
      }
    }
    //compare suit indexes
    if (lhs_suit < rhs_suit) {
      return true;
    }
  }
  return false;
}

bool operator>(const Card &lhs, const Card &rhs) {
  //consier ranks first
  int lhs_rank;
  int rhs_rank;

  //determine index of the ranks of each card
  for (int j = 0; j < NUM_RANKS; ++j) {
    if (lhs.get_rank() == RANK_NAMES_BY_WEIGHT[j]) {
      lhs_rank = j;
    }
    if (rhs.get_rank() == RANK_NAMES_BY_WEIGHT[j]) {
      rhs_rank = j;
    } 
  }
  //compare rank indexes
  if (lhs_rank > rhs_rank) {
    return true;
  }
  //if cards have the same rank, compare suits
  else if (lhs_rank == rhs_rank) {
    int lhs_suit;
    int rhs_suit;

    //determine index of the suits of each card
    for(int i = 0; i < NUM_SUITS; ++i) {
      if (lhs.get_suit() == SUIT_NAMES_BY_WEIGHT[i]) {
        lhs_suit = i;
      }
      if (rhs.get_suit() == SUIT_NAMES_BY_WEIGHT[i]) {
        rhs_suit = i;
      }
    }
    //compare suit indexes
    if (lhs_suit > rhs_suit) {
      return true;
    }
  }
  return false;
}

bool operator==(const Card &lhs, const Card &rhs) {
  //cards are the same if they have the same rank and the same suit
  if(lhs.get_rank() == rhs.get_rank() && lhs.get_suit() == rhs.get_suit()) {
    return true;
  }
  return false;
}

bool operator!=(const Card &lhs, const Card &rhs) {
  //if cards have different ranks or different suits, they are not the same
  if(lhs.get_rank() != rhs.get_rank() || lhs.get_suit() != rhs.get_suit()) {
    return true;
  }
  return false;
}

string Suit_next(const string &suit) {
  if (suit == Card::SUIT_SPADES) {
    return Card::SUIT_CLUBS;
  }
  else if (suit == Card::SUIT_CLUBS) {
    return Card::SUIT_SPADES;
  }
  else if (suit == Card::SUIT_HEARTS) {
    return Card::SUIT_DIAMONDS;
  }
  else {
    return Card::SUIT_HEARTS;
  }
}

ostream & operator<<(ostream &os, const Card &card) {
  os << card.get_rank() << " of " << card.get_suit();
  return os;
}

bool Card_less(const Card &a, const Card &b, const string &trump) {
  //right bower is always highest
  if(b.is_right_bower(trump)) {
    return true;
  }
  if(a.is_right_bower(trump)) {
    return false;
  }

  //left bower is bigger than every card except right bower
  if (b.is_left_bower(trump) && !a.is_right_bower(trump)) {
    return true;
  }

  //trumps are higher than all non-trumps
  if (a.is_trump(trump) && !b.is_trump(trump)) {
    return false;
  }
  if (!a.is_trump(trump) && b.is_trump(trump)) {
    return true;
  }

  //when both are trump, compare ranks
  if (a.is_trump(trump) && b.is_trump(trump)) {
    if(a < b) {
      return true;
    }
  }

  //when both are non-trump, compare ranks
  if (!a.is_trump(trump) && !b.is_trump(trump)) {
    if (a < b) {
      return true;
    }
  }
  return false;
}

bool Card_less(const Card &a, const Card &b, const Card &led_card, const string &trump) {
  //consider trump first
  //right bower is always highest
  if(b.is_right_bower(trump)) {
    return true;
  }
  if(a.is_right_bower(trump)) {
    return false;
  }

  //left bower is bigger than every card except right bower
  if (b.is_left_bower(trump) && !a.is_right_bower(trump)) {
    return true;
  }

  //trumps are higher than all non-trumps
  if (a.is_trump(trump) && !b.is_trump(trump)) {
    return false;
  }
  if (!a.is_trump(trump) && b.is_trump(trump)) {
    return true;
  }

  //when both are trump, compare ranks
  if (a.is_trump(trump) && b.is_trump(trump)) {
    if(a < b) {
      return true;
    }
  }

  //when neither card is trump, consider the led suit
  string led_suit = led_card.get_suit(trump);
  if (!a.is_trump(trump) && !b.is_trump(trump)) {
    //card of led suit is higher than card of non-led suit
    if(a.get_suit(trump) != led_suit && b.get_suit(trump) == led_suit) {
      return true;
    }
    if(a.get_suit(trump) == led_suit && b.get_suit(trump) != led_suit) {
      return false;
    }

    //when both cards are not led-suit, compare ranks
    if(a.get_suit(trump) != led_suit && b.get_suit(trump) != led_suit) {
      if(a < b) {
        return true;
      }
    }
    //when both cards are led-suit, compare ranks
    if(a.get_suit(trump) == led_suit && b.get_suit(trump) == led_suit) {
      if(a < b) {
        return true;
      }
    }
  }
  return false;
}

