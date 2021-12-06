/*
 * Bailey Ferris
 * Project 4 - Euchre
 * Pack.cpp
 *
 */

#include "Pack.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

Pack::Pack() : next(PACK_SIZE) {}

Pack::Pack(const string &pack_filename) {
    ifstream fin;
    fin.open(pack_filename.c_str());
    if (!fin.is_open()) {
        cout << "Error opening " << pack_filename << endl;
        exit(EXIT_FAILURE);
    }
    string rank_in, of, suit_in;
    while (fin >> rank_in >> of >> suit_in) {
        cards.push_back(Card(rank_in, suit_in));
    }
    fin.close();
    next = 0;
}

Card Pack::deal_one() {
    Card deal = cards.at(next);
    ++next;
    return deal;
}

void Pack::reset() {
    next = 0;
}

void Pack::shuffle() {
    for (int i = 0; i < 3; ++i) {
        vector<Card> front_cards;
        vector<Card> back_cards;
        for (int j = 0; j < PACK_SIZE; ++j) {
            if(j < 17) {
                front_cards.push_back(cards.at(j));
            }
            else if (j >= 17) {
                back_cards.push_back(cards.at(j));
            }
        }
        int k;
        for (k = 0; k < 7; ++k) {
            cards.at(k) = back_cards.at(k);
        }
        for (int t = 0; k < PACK_SIZE; ++t) {
            cards.at(k) = front_cards.at(t);
            ++k;
        }
    }
    reset();
}

bool Pack::empty() const {
    return(next >= PACK_SIZE);
}

