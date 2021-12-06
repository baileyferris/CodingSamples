/*
 * Bailey Ferris
 * Project 4 - Euchre
 * Pack_tests.cpp
 *
 */

#include "Pack.h"
#include <iostream>
#include <cassert>
using namespace std;

// TEST FUNCTION PROTOTYPES

//tests Pack.cpp Pack() constructors
void test_Pack();

//tests Pack.cpp deal_one() function
void test_deal_one();

//tests Pack.cpp reset() function
void test_reset();

//tests Pack.cpp shuffle() function
void test_shuffle();

//tests Pack.cpp empty() function
void test_empty();

int main() {

    test_Pack();
    test_deal_one();
    test_reset();
    test_shuffle();
    test_empty();

  cout << "Pack_tests passed" << endl;
  return 0;
}

// IMPLEMENT YOUR TEST FUNCTIONS HERE
void test_Pack() {
    Pack pack1;
    assert(pack1.empty() == true);
    
    Pack pack2("pack.in");
    Card first_card = pack2.deal_one();
    assert(first_card == Card(Card::RANK_NINE, Card::SUIT_SPADES));

    cout << "constructor passed!" << endl;
}

void test_deal_one() {
    Pack pack("pack.in");

    Card first_card = pack.deal_one();
    assert(first_card == Card(Card::RANK_NINE, Card::SUIT_SPADES));

    Card second_card = pack.deal_one();
    assert(second_card == Card(Card::RANK_TEN, Card::SUIT_SPADES));

    cout << "deal_one passed!" << endl;
}

void test_reset() {
    Pack pack("pack.in");

    Card first_card = pack.deal_one();
    Card second_card = pack.deal_one();

    pack.reset();

    Card third_card = pack.deal_one();
    assert(first_card == third_card);

    cout << "reset passed!" << endl;
}

void test_shuffle() {
    Pack pack("pack.in");

    pack.shuffle();
    Card first_card = pack.deal_one();
    assert(first_card == Card(Card::RANK_QUEEN, Card::SUIT_SPADES));

    pack.shuffle();
    first_card = pack.deal_one();
    assert(first_card == Card(Card::RANK_NINE, Card::SUIT_HEARTS));

    cout << "shuffle passed!" << endl;
}

void test_empty() {
    Pack pack;
    assert(pack.empty() == true);

    Pack pack2("pack.in");
    assert(pack2.empty() == false);

    cout << "empty passed!" << endl;
}

