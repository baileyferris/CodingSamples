/*
 * Bailey Ferris
 * Project 4 - Euchre
 * Player.cpp
 *
 */

#include "Player.h"
#include "Card.h"
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <vector>

using namespace std;

// SIMPLE CLASS DECLARATION
class Simple : public Player {
public:
  //MODIFIES name, hand_size
  //EFFECTS creates simple player with name "name_in" and hand_size = 0
  Simple(string name_in);

  //EFFECTS returns player's name
  virtual const string & get_name() const;

  //REQUIRES player has less than MAX_HAND_SIZE cards
  //EFFECTS  adds Card c to Player's hand
  virtual void add_card(const Card &c);

  //REQUIRES round is 1 or 2
  //MODIFIES order_up_suit
  //EFFECTS If Player wishes to order up a trump suit then return true and
  //  change order_up_suit to desired suit.  If Player wishes to pass, then do
  //  not modify order_up_suit and return false.
  //  For a simple player: (round 1) order up if Player has two or more face cards the
  //  same suit as the upcard (round 2) order up the suit the same color as the
  //  upcard if they have 1+ face cards of the suit
  //  **screw the dealer** (in round 2) if making reaches dealer, dealer orders
  //  suit the same color as upcard
  virtual bool make_trump(const Card &upcard, const Player &dealer,
                          int round, string &order_up_suit) const;

  //REQUIRES Player has at least one card
  //MODIFIES hand
  //EFFECTS  Player adds one card to hand and removes one card from hand.
  //  For a simple Player: Player discards the lowest card, including upcard
  virtual void add_and_discard(const Card &upcard);

  //REQUIRES Player has at least one card
  //MODIFIES hand, hand_size
  //EFFECTS  Leads one Card from Player's hand according to their strategy
  //  For simple Player: Player leads the highest non-trump in their hand.
  //  If Player only has trump cards, they lead the highest trump card in
  //  their hand
  //  "Lead" means to play the first Card in a trick.  The card
  //  is removed the player's hand.
  virtual Card lead_card(const string &trump);

  //REQUIRES Player has at least one card
  //MODIFIES hand, hand_size
  //EFFECTS  Plays one Card from Player's hand according to their strategy.
  //  For simple Player: if Player can follow suit, Player plays the highest 
  //  card in their had that follows suit. Otherwise, they play the lowest
  //  card in their hand
  //  The card is removed from the player's hand.
  virtual Card play_card(const Card &led_card, const string &trump);

  // Maximum number of cards in a player's hand
  static const int MAX_HAND_SIZE = 5;

private:
  string name;
  vector<Card> hand;
  int hand_size;

  //REQUIRES Player has at least one card
  //MODIFIES hand, hand_size
  //EFFECTS removes one Card from Player's hand
  void remove_card(const Card &card_out);
};

// HUMAN CLASS DECLARATION

class Human : public Player {
public:
  //MODIFIES name, hand_size
  //EFFECTS creates human Player with name "name_in" and hand_size = 0
  Human(string name_in);
    
  //EFFECTS returns player's name
  virtual const string & get_name() const;

  //REQUIRES player has less than MAX_HAND_SIZE cards
  //EFFECTS  adds Card c to Player's hand
  virtual void add_card(const Card &c);

  //REQUIRES round is 1 or 2
  //MODIFIES order_up_suit, stdout
  //EFFECTS If Player wishes to order up a trump suit then return true and
  //  change order_up_suit to desired suit.  If Player wishes to pass, then do
  //  not modify order_up_suit and return false.
  //  For human Player: prints Player's hand and prompts user to orderup or
  //  pass.
  virtual bool make_trump(const Card &upcard, const Player &dealer,
                          int round, string &order_up_suit) const;

  //REQUIRES Player has at least one card
  //MODIFIES stdout, hand
  //EFFECTS  Player adds one card to hand and removes one card from hand.
  //  For human Player: prints Player's hand and an option to discard the
  //  upcard. Prompts user to choose a card to discard
  virtual void add_and_discard(const Card &upcard);

  //REQUIRES Player has at least one card
  //MODIFIES hand, stdout
  //EFFECTS  Leads one Card from Player's hand according to their strategy
  //  For human Player: prints Player's hand, and prompts the user to choose
  //  a card to lead.
  //  "Lead" means to play the first Card in a trick.  The card
  //  is removed the player's hand.
  virtual Card lead_card(const string &trump);

  //REQUIRES Player has at least one card
  //MODIFIES hand, stdout
  //EFFECTS  Plays one Card from Player's hand according to their strategy.
  //  For human Player: prints Player's hand, and prompts the user to choose
  //  a card to play.
  //  The card is removed from the player's hand.
  virtual Card play_card(const Card &led_card, const string &trump);

  // Maximum number of cards in a player's hand
  static const int MAX_HAND_SIZE = 5;

private:
  string name;
  vector<Card> hand;
  int hand_size;

  //REQUIRES Player has at least one card
  //MODIFIES hand, hand_size
  //EFFECTS  removes one Card from Player's hand
  void remove_card(const Card &card_out);

  //MODIFIES stdout
  //EFECTS   prints human player's hand, in ascending order
  void print_hand(const vector<Card> &hand_copy) const;
};

// SIMPLE CLASS MEMBER FUNCTION IMPLEMENTATIONS

Simple::Simple(string name_in) {
  name = name_in;
  hand_size = 0;
}

const string & Simple::get_name() const {
  return name;
}

void Simple::add_card(const Card &c) {
  hand.push_back(c);
  ++hand_size;
}

bool Simple::make_trump(const Card &upcard, const Player &dealer,
                          int round, string &order_up_suit) const {
  string up_suit = upcard.get_suit();
  int num_trumps = 0;
  if (round == 1) {
    //determine number of trumps in player's hand
    for(int i = 0; i < hand_size; ++i) {
      if (hand[i].is_right_bower(up_suit)) {
        ++num_trumps;
      }
      else if (hand[i].is_left_bower(up_suit)) {
        ++num_trumps;
      }
      else if (hand[i].is_trump(up_suit) && hand[i].is_face()) {
        ++num_trumps;
      }
    }
    //player has 2+ trumps, orderup the up_suit
    if (num_trumps >= 2) {
      order_up_suit = up_suit;
        return true;
    }
  }

  else if (round == 2) {
    //if Player is the dealer **screw the dealer**
    if(&this->name == &dealer.get_name()) {
      order_up_suit = Suit_next(up_suit);
        return true;
    }
    //determine number of cards that are face cards the same color as upcard
    for(int i = 0; i < hand_size; ++i) {
      if((hand[i].get_suit() == Suit_next(up_suit)) && hand[i].is_face()) {
        ++num_trumps;
      }
    }
    //if Player has 1+ cards same color as upcard, orderup
    if(num_trumps >= 1) {
      order_up_suit = Suit_next(up_suit);
        return true;
    }
  }
  return false;
}

void Simple::add_and_discard(const Card &upcard) {
  Card smallest = hand[0];
  int index = 0;

  //determine smallest card already in hand
  for(int i = 0; i < hand_size; ++i) {
    if(Card_less(hand[i], smallest, upcard.get_suit())) {
      smallest = hand[i];
        index = i;
    }
  }
  //if upcard is larger than smallest card in hand, replace with upcard
  if(Card_less(smallest, upcard, upcard.get_suit())) {
    hand[index] = upcard;
  }
}

Card Simple::lead_card(const string &trump) {
  vector<Card> non_trump;
  int non_size = 0;

  //make vector of only non-trump cards in Player's hand
  for(int i = 0; i < hand_size; ++i) {
    if(!(hand[i].is_trump(trump))) {
      non_trump.push_back(hand[i]);
        ++non_size;
    }
  }
  //determine largest non-trump card in Player's hand and remove it
  if(non_size > 0) {
    Card non_trump_big = non_trump[0];
      for(int j = 0; j < non_size; ++j) {
        if(Card_less(non_trump_big, non_trump[j], trump)) {
          non_trump_big = non_trump[j];
        }
      }
      remove_card(non_trump_big);
      return non_trump_big;
  }
  //if no non-trump cards, determine largest trump and remove it
  Card trump_big = hand[0];
  for(int k = 0; k < hand_size; ++k) {
    if(Card_less(trump_big, hand[k], trump)) {
      trump_big = hand[k];
    }
  }
  remove_card(trump_big);
  return trump_big;
}

Card Simple::play_card(const Card &led_card, const string &trump) {
  string led_suit = led_card.get_suit(trump);
  vector<Card> match_suit;
  int size = 0;

  //make vector of cards in hand that match led-suit
  for(int i = 0; i < hand_size; ++i) {
    if(hand[i].get_suit(trump) == led_suit) {
      match_suit.push_back(hand[i]);
        ++size;
    }
  }
  //determine largest card in Player's hand that matches suit and remove it
  if(size > 0) {
    Card biggest = match_suit[0];
    for(int j = 0; j < size; ++j) {
      if(Card_less(biggest, match_suit[j], led_card, trump)) {
        biggest = match_suit[j];
      }
    }
    remove_card(biggest);
    return biggest;
  }

  //if no cards that match suit, determine lowest card in hand and remove it
  Card smallest = hand[0];
  for(int k = 0; k < hand_size; ++k) {
    if(Card_less(hand[k], smallest, led_card, trump)) {
      smallest = hand[k];
    }
  }
  remove_card(smallest);
  return smallest;
}

void Simple::remove_card(const Card &card_out) {
  for(int i = 0; i < hand_size; ++i) {
    if(hand[i] == card_out) {
      hand[i] = hand[hand_size - 1];
    }
  }
  hand.pop_back();
  --hand_size;
}

// HUMAN CLASS MEMBER FUNCTION IMPLEMENTATIONS

Human::Human(string name_in) {
  name = name_in;
  hand_size = 0;
}

const string & Human::get_name() const {
  return name;
}

void Human::add_card(const Card &c) {
  hand.push_back(c);
  ++hand_size;
}

bool Human::make_trump(const Card &upcard, const Player &dealer,
                          int round, string &order_up_suit) const {
  //print player's hand in ascending order
  vector<Card> hand_copy;
  for(int i = 0; i < hand_size; ++i) {
    hand_copy.push_back(hand[i]);
  }
  sort(hand_copy.begin(), hand_copy.end());
  print_hand(hand_copy);

  string choice;
  cout << "Human player " << name << ", please enter a suit, or \"pass\":" << endl;
  cin >> choice;

  if(choice != "pass") {
    order_up_suit = choice;
    return true;
  }
  return false;
}

void Human::add_and_discard(const Card &upcard) {
  //print player's hand in ascending order
  vector<Card> hand_copy;
  for(int i = 0; i < hand_size; ++i) {
    hand_copy.push_back(hand[i]);
  }
  sort(hand_copy.begin(), hand_copy.end());
  print_hand(hand_copy);

  int choice;
  cout << "Discard upcard: [-1]" << endl;
  cout << "Human player " << name << ", please select a card to discard:" << endl;
  cin >> choice;

  if(choice != -1) {
    for(int j = 0; j < hand_size; ++j) {
      if(hand[j] == hand_copy[choice]) {
        hand[j] = upcard;
      }
    }
  }
}

Card Human::lead_card(const string &trump) {
  //print player's hand in ascending order
  vector<Card> hand_copy;
  for(int i = 0; i < hand_size; ++i) {
    hand_copy.push_back(hand[i]);
  }
  sort(hand_copy.begin(), hand_copy.end());
  print_hand(hand_copy);

  int choice;
  cout << "Human player " << name << ", please select a card:" << endl;
  cin >> choice;

  for(int j = 0; j < hand_size; ++j) {
    if(hand[j] == hand_copy[choice]){
      remove_card(hand[j]);
    }
  }
  return hand_copy[choice];
}

Card Human::play_card(const Card &led_card, const string &trump) {
  //print player's hand in ascending order
  vector<Card> hand_copy;
  for(int i = 0; i < hand_size; ++i) {
    hand_copy.push_back(hand[i]);
  }
  sort(hand_copy.begin(), hand_copy.end());
  print_hand(hand_copy);

  int choice;
  cout << "Human player " << name << ", please select a card:" << endl;
  cin >> choice;

  for(int j = 0; j < hand_size; ++j) {
    if(hand[j] == hand_copy[choice]){
      remove_card(hand[j]);
    }
  }
  return hand_copy[choice];
}

void Human::print_hand(const vector<Card> &hand_copy) const {
  for(int i = 0; i < hand_size; ++i) {
    cout << "Human player " << name << "'s hand: [" << i << "] ";
      cout << hand_copy[i] << endl;
  }
}

void Human::remove_card(const Card &card_out) {
  for(int i = 0; i < hand_size; ++i) {
    if(hand[i] == card_out) {
      hand[i] = hand[hand_size - 1];
    }
  }
  hand.pop_back();
  --hand_size;
}

// IMPLEMENTATIONS OF OTHER FUNCTIONS IN PLAYER.H

Player * Player_factory(const string &name, const string &strategy) {
  if(strategy == "Simple") {
    return new Simple(name);
  }
  if(strategy == "Human") {
    return new Human(name);
  }
  assert(false);
  exit(1);
}

ostream & operator<<(ostream &os, const Player &p) {
  os << p.get_name();
  return os;
}

