/*
 * Bailey Ferris
 * Project 4 - Euchre
 * Card_tests.cpp
 *
 */

#include "Card.h"
#include <iostream>
#include <cassert>
using namespace std;

// TEST FUNCTION PROTOTYPES

//tests Card.cpp get_rank() function
void test_get_rank();

//tests Card.cpp get_suit() function
void test_get_suit();

//tests Card.cpp Card() constructor
void test_Card();

//tests Card.cpp is_face() function
void test_is_face();

//tests Card.cpp is_right_bower() function
void test_is_right_bower();

//tests Card.cpp is_left_bower() function
void test_is_left_bower();

//tests Card.cpp is_trump() function
void test_is_trump();

//tests Card.cpp overloaded < operator
void test_operator_less();

//tests Card.cpp overloaded > operator
void test_operator_more();

//tests Card.cpp overloaded == operator
void test_operator_equals();

//tests Card.cpp overloaded != operator
void test_operator_not_equals();

//tests Card.cpp Suit_next() function
void test_Suit_next();

//tests Card.cpp overloaded << operator
void test_print_card();

//tests Card.cpp Card_less() function
void test_Card_less();

int main() {

    test_get_rank();
    test_get_suit();
    test_Card();
    test_is_face();
    test_is_right_bower();
    test_is_left_bower();
    test_is_trump();
    test_operator_less();
    test_operator_more();
    test_operator_equals();
    test_operator_not_equals();
    test_Suit_next();
    test_print_card();
    test_Card_less();

  cout << "Card_tests passed" << endl;
  return 0;
}

// TEST FUNCTION IMPLEMENTATIONS

void test_get_rank() {
    Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    assert(two_spades.get_rank() == Card::RANK_TWO);
    assert(jack_hearts.get_rank() == Card::RANK_JACK);

    cout << "get_rank passed!" << endl;
}

void test_get_suit() {
    Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
    assert(two_spades.get_suit() == Card::SUIT_SPADES);
    assert(two_spades.get_suit(Card::SUIT_CLUBS) == Card::SUIT_SPADES);

    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    assert(jack_spades.get_suit() == Card::SUIT_SPADES);
    assert(jack_spades.get_suit(Card::SUIT_CLUBS) == Card::SUIT_CLUBS);

    cout << "get_suit passed!" << endl;
}

void test_Card() {
    Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
    assert(two_spades.get_rank() == Card::RANK_TWO);
    assert(two_spades.get_suit() == Card::SUIT_SPADES);

    cout << "Card passed!" << endl;
}

void test_is_face() {
    Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
    assert(two_spades.is_face() == false);

    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    assert(jack_spades.is_face() == true);

    Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
    assert(ace_hearts.is_face() == true);

    cout << "is_face passed!" << endl;
}

void test_is_right_bower() {
    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    assert(jack_spades.is_right_bower(Card::SUIT_SPADES) == true);
    assert(jack_spades.is_right_bower(Card::SUIT_CLUBS) == false);

    Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
    assert(two_spades.is_right_bower(Card::SUIT_SPADES) == false);

    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    assert(jack_hearts.is_right_bower(Card::SUIT_CLUBS) == false);
    assert(jack_hearts.is_right_bower(Card::SUIT_HEARTS) == true);

    cout << "is_right_bower passed!" << endl;
}

void test_is_left_bower() {
    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    assert(jack_spades.is_left_bower(Card::SUIT_SPADES) == false);
    assert(jack_spades.is_left_bower(Card::SUIT_CLUBS) == true);

    Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
    assert(two_spades.is_left_bower(Card::SUIT_CLUBS) == false);

    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    assert(jack_hearts.is_left_bower(Card::SUIT_CLUBS) == false);
    assert(jack_hearts.is_left_bower(Card::SUIT_HEARTS) == false);

    Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    assert(jack_diamonds.is_left_bower(Card::SUIT_HEARTS) == true);

    cout << "is_left_bower passed!" << endl;
}

void test_is_trump() {
    Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
    assert(nine_spades.is_trump(Card::SUIT_SPADES) == true);
    assert(nine_spades.is_trump(Card::SUIT_CLUBS) == false);

    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    assert(jack_spades.is_trump(Card::SUIT_SPADES) == true);
    assert(jack_spades.is_trump(Card::SUIT_CLUBS) == true);
    assert(jack_spades.is_trump(Card::SUIT_HEARTS) == false);

    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    assert(jack_hearts.is_trump(Card::SUIT_HEARTS) == true);
    assert(jack_hearts.is_trump(Card::SUIT_DIAMONDS) == true);

    cout << "is_trump passed!" << endl;
}

void test_operator_less() {
    Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    assert(two_spades < jack_spades);

    Card two_diamonds = Card(Card::RANK_TWO, Card::SUIT_DIAMONDS);
    assert(two_diamonds < jack_spades);

    Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    assert(jack_spades < jack_diamonds);

    cout << "< passed!" << endl;
}

void test_operator_more() {
    Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    assert(jack_spades > two_spades);

    Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    assert(jack_diamonds > jack_spades);

    Card two_diamonds = Card(Card::RANK_TWO, Card::SUIT_DIAMONDS);
    assert(jack_spades > two_diamonds);

    cout << "> passed!" << endl;
}

void test_operator_equals() {
    Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
    Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);

    assert(two_spades == two_spades);
    assert(jack_diamonds == jack_diamonds);
    assert(ten_spades == ten_spades);

    cout << "== passed!" << endl;
}

void test_operator_not_equals() {
    Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
    Card two_clubs = Card(Card::RANK_TWO, Card::SUIT_CLUBS);
    Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    assert(two_spades != jack_diamonds);
    assert(two_clubs != two_spades);

    cout << "!= passed!" << endl;
}

void test_Suit_next() {
    assert(Suit_next(Card::SUIT_SPADES) == Card::SUIT_CLUBS);
    assert(Suit_next(Card::SUIT_CLUBS) == Card::SUIT_SPADES);
    assert(Suit_next(Card::SUIT_HEARTS) == Card::SUIT_DIAMONDS);
    assert(Suit_next(Card::SUIT_DIAMONDS) == Card::SUIT_HEARTS);

    cout << "Suit_next passed!" << endl;
}

void test_print_card() {
    Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
    cout << two_spades << endl;
}

void test_Card_less() {
    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
    Card two_diamonds = Card(Card::RANK_TWO, Card::SUIT_DIAMONDS);
    Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card three_diamonds = Card(Card::RANK_THREE, Card::SUIT_DIAMONDS);
    Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
    Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
    Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);
    Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
    Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);

    assert(Card_less(jack_spades, jack_clubs, Card::SUIT_CLUBS) == true);
    assert(Card_less(two_diamonds, jack_spades, three_diamonds, Card::SUIT_HEARTS) == false);
    assert(Card_less(ace_hearts, jack_hearts, Card::SUIT_HEARTS) == true);
    assert(Card_less(jack_hearts, ace_hearts, Card::SUIT_HEARTS) == false);
    assert(Card_less(nine_spades, ten_spades, king_clubs, Card::SUIT_HEARTS) == true);
    assert(Card_less(nine_diamonds, ace_clubs, jack_spades, Card::SUIT_HEARTS) == true);

    cout << "Card_less passed!" << endl;
}
