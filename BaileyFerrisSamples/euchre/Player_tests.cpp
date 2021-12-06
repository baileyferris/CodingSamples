/*
 * Bailey Ferris
 * Project 4 - Euchre
 * Player_tests.cpp
 *
 */

#include "Player.h"
#include <iostream>
#include <cassert>
using namespace std;

// TEST FUNCTION PROTOTYPES

// tests Player.cpp get_name() function
void test_get_name();

// tests Player.cpp make_trump() function
void test_make_trump();

// tests Player.cpp add_and_discard() function
void test_add_and_discard();

// tests Player.cpp lead_card() function
void test_lead_card();

// tests Player.cpp play_card() function
void test_play_card();

// tests Player.cpp overloaded << operator
void test_output_operator();

int main() {

    test_get_name();
    test_make_trump();
    test_add_and_discard();
    test_lead_card();
    test_play_card();
    test_output_operator();

  cout << "Player_tests passed" << endl;
  return 0;
}

// TEST FUNCTION IMPLEMENTATIONS

void test_get_name() {
    Player * simple = Player_factory("Robot", "Simple");
    assert(simple->get_name() == "Robot");

    cout << "get_name() passed!" << endl;

    delete simple;
}

void test_make_trump() {
    Player * bob1 = Player_factory("Bob", "Simple");
    Player * bob2 = Player_factory("Bob", "Simple");

    Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
    Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
    Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);

    bob1->add_card(ten_spades);
    bob1->add_card(nine_spades);
    bob1->add_card(ace_hearts);
    bob1->add_card(jack_hearts);
    bob1->add_card(ten_hearts);

    bob2->add_card(ten_spades);
    bob2->add_card(ten_spades);
    bob2->add_card(ten_spades);
    bob2->add_card(ten_spades);
    bob2->add_card(ten_spades);

    Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
    string trump = Card::SUIT_DIAMONDS;

    bool orderup = bob1->make_trump(queen_diamonds, *bob1, 1, trump);

    assert(orderup == false);
    assert(trump == Card::SUIT_DIAMONDS);

    orderup = bob1->make_trump(queen_diamonds, *bob1, 2, trump);
    assert(orderup == true);
    assert(trump == Card::SUIT_HEARTS);

    orderup = bob2->make_trump(queen_diamonds, *bob1, 2, trump);
    assert(orderup == false);

    cout << "make_trump() passed!" << endl;

}

void test_add_and_discard() {
    Player * bob = Player_factory("Bob", "Simple");

    Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
    Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
    Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
    Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
    Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);

    bob->add_card(ten_spades);
    bob->add_card(nine_spades);
    bob->add_card(ace_hearts);
    bob->add_card(jack_hearts);
    bob->add_card(ten_hearts);

    bob->add_and_discard(nine_hearts);
    bob->play_card(ten_clubs, Card::SUIT_HEARTS);
    assert(bob->play_card(ten_clubs, Card::SUIT_HEARTS) == nine_hearts);

    cout << "add_and_discard() passed!" << endl;
}

void test_lead_card() {
    Player * bob = Player_factory("Bob", "Simple");
    Player * alice = Player_factory("Alice", "Simple");

    Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
    Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
    Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);

    bob->add_card(ten_spades);
    bob->add_card(nine_spades);
    bob->add_card(ace_hearts);
    bob->add_card(jack_hearts);
    bob->add_card(ten_hearts);

    string trump = Card::SUIT_HEARTS;
    assert(bob->lead_card(trump) == ten_spades);
    assert(bob->lead_card(trump) == nine_spades);
    assert(bob->lead_card(trump) == jack_hearts);

    alice->add_card(ace_hearts);
    alice->add_card(jack_hearts);
    alice->add_card(ten_hearts);
    assert(alice->lead_card(trump) == jack_hearts);

    cout << "lead_card() passed!" << endl;
    delete bob;
}

void test_play_card() {
    Player * bob = Player_factory("Bob", "Simple");

    Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
    Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
    Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
    Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);
    Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

    bob->add_card(ten_spades);
    bob->add_card(nine_spades);
    bob->add_card(ace_hearts);
    bob->add_card(jack_hearts);
    bob->add_card(ten_hearts);

    string trump = Card::SUIT_HEARTS;
    assert(bob->play_card(king_clubs, trump) == nine_spades);
    assert(bob->play_card(king_spades, trump) == ten_spades);

    cout << "play_card() passed!" << endl;
}

void test_output_operator() {
    Player * bob = Player_factory("Bob", "Simple");
    cout << *bob << endl;
}

