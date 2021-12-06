#include "Card.h"

#include <cassert>
#include <iostream>

using namespace std;


// THIS TEST CASE WILL ONLY TELL YOU IF YOUR CODE COMPILES.
// YOU NEED TO WRITE YOUR OWN COMPREHENSIVE TEST CASES IN Card_tests.cpp


int main()
{
    Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
    assert(two_spades.get_rank() == Card::RANK_TWO);
    assert(two_spades.get_suit() == Card::SUIT_SPADES);
    assert(two_spades.get_suit(Card::SUIT_CLUBS) == Card::SUIT_SPADES);

    assert(!two_spades.is_face());
    assert(!two_spades.is_right_bower(Card::SUIT_CLUBS));
    assert(!two_spades.is_left_bower(Card::SUIT_CLUBS));
    assert(!two_spades.is_trump(Card::SUIT_CLUBS));

    assert(!(two_spades < two_spades));
    assert(!(two_spades > two_spades));
    assert(two_spades == two_spades);
    assert(!(two_spades != two_spades));

    assert(Suit_next(Card::SUIT_CLUBS) == Card::SUIT_SPADES);

    cout << two_spades << endl;

    assert(!Card_less(two_spades, two_spades, Card::SUIT_CLUBS));
    assert(!Card_less(two_spades, two_spades, two_spades, Card::SUIT_CLUBS));

    cout << "PASS" << endl;

    return 0;
}
