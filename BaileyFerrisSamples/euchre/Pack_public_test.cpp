#include "Pack.h"
#include "Card.h"
#include <cassert>
#include <iostream>
using namespace std;

// THIS TEST CASE WILL ONLY TELL YOU IF YOUR CODE COMPILES.
// YOU NEED TO WRITE YOUR OWN COMPREHENSIVE TEST CASES IN Pack_tests.cpp


int main() {
  Pack pack("pack.in");

  Card first_card = pack.deal_one();
  assert(first_card == Card(Card::RANK_NINE, Card::SUIT_SPADES));

  assert(!pack.empty());

  pack.reset();

  first_card = pack.deal_one();
  assert(first_card == Card(Card::RANK_NINE, Card::SUIT_SPADES));

  pack.shuffle();

  first_card = pack.deal_one();
  assert(first_card == Card(Card::RANK_QUEEN, Card::SUIT_SPADES));

  cout << "PASS" << endl;

  return 0;
}
