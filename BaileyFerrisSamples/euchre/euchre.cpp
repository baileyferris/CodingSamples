/*
 * Bailey Ferris
 * Project 4 - Euchre
 * euchre.cpp
 *
 */
 
#include "Card.h"
#include "Pack.h"
#include "Player.h"
#include <cstdlib>
#include <iostream>

using namespace std;

// GAME STRUCT DECLARATION

struct Game {
    vector<Player*> players;
    Pack pack;
    int t1_points;
    int t2_points;
    int num_hand;
    int win_points;
    string shuffle_mode;
    int deal_index;
    int lead_index;
    int called_trump;
};

//HELPER FUNCTIONS DECLARATIONS

//MODIFIES game, stdout
//EFFECTS  Goes through making trump process
static string make_trump(Game &game, Card &upcard);

//REQUIRES game.pack is not empty
//MODIFIES game.pack
//EFFECTS  shuffles the deck if in shuffle mode
static void shuffle_deck(Game &game);

//REQUIRES Players' hands are empty
//MODIFIES game.players
//EFFECTS  Deals cards to players, starting with player to the left of dealer
//  and working clockwise. Cards are dealt in 3-2-3-2 2-3-2-3 pattern
static void deal(Game &game);

//MODIFIES game, stdout
//EFFECTS  Plays a hand of euchre
//  shuffles cards, deals cards, makes trump, plays tricks, adds points
//  prints results of hand to stdout
static void play_hand(Game &game);

//MODIFIES stdout
//EFFECTS  checks command line arguments for valid input. Prints messages
//  if arguments are invalid and exits
static void errors(int argc, char **argv);

//REQUIRES Players' hands are not empty
//MODIFIES game, stdout
//EFFECTS  plays a trick in a game of euchre. prints player who won the trick
static void play_trick(Game &game, const string &trump);

//EFFECTS  Determines which player won the trick
static Card won_trick(Card &card_led, Card &left_play, Card &cross_play, 
                        Card &right_play, const string &trump);


int main(int argc, char **argv) {
    //print errors if any arguments are invalid
    errors(argc, argv);

    //print command line args
    for(int i = 0; i < argc; ++i) {
        cout << argv[i] << " ";
    }
    cout << endl;

    //initializes Game struct variables
    Game game;
    string pack_in = argv[1];
    game.pack = Pack(pack_in);
    game.t1_points = 0;
    game.t2_points = 0;
    game.num_hand = 0;
    game.win_points = atoi(argv[3]);
    game.shuffle_mode = argv[2];
    game.deal_index = 0;
    game.lead_index = 1;

    //initializes players vector using args
    game.players.push_back(Player_factory(argv[4], argv[5]));
    game.players.push_back(Player_factory(argv[6], argv[7]));
    game.players.push_back(Player_factory(argv[8], argv[9]));
    game.players.push_back(Player_factory(argv[10], argv[11]));

    //until one team has number of points needed to win, play hands
    while(game.t1_points < game.win_points && game.t2_points < game.win_points) {
        play_hand(game);
    }

    //determine winners!
    if(game.t1_points > game.t2_points) {
        cout << *game.players[0] << " and " << *game.players[2] << " win!" << endl;
    }
    if(game.t2_points > game.t1_points) {
        cout << *game.players[1] << " and " << *game.players[3] << " win!" << endl;
    }

    for (int i = 0; i < int(game.players.size()); ++i) {
        delete game.players[i];
        game.players[i] = 0;
    }
}

static void play_hand(Game &game) {
    cout << "Hand " << game.num_hand << endl;
    //pick up cards
    game.pack.reset();
    shuffle_deck(game);
    deal(game);
    Card upcard = game.pack.deal_one();
    cout << upcard << " turned up" << endl;

    string trump = make_trump(game, upcard);
    cout << endl;
    int t1_tricks = 0;
    int t2_tricks = 0;

    //play 5 tricks per hand
    for(int i = 0; i < 5; ++i) {
        play_trick(game, trump);
        //determine which team won the trick
        if(game.lead_index == 0 || game.lead_index == 2) {
            ++t1_tricks;
        }
        if(game.lead_index == 1 || game.lead_index == 3) {
            ++t2_tricks;
        }
    }

    if(t1_tricks > t2_tricks) {
        cout << *game.players[0] << " and " << *game.players[2] 
             << " win the hand" << endl;
        game.t1_points += 1;
        //euchred! if winner of trick did not call trump
        if(game.called_trump == 1 || game.called_trump == 3) {
            game.t1_points += 1;
            cout << "euchred!" << endl;
        }
    }
    if(t2_tricks > t1_tricks) {
        cout << *game.players[1] << " and " << *game.players[3] 
             << " win the hand" << endl;
        game.t2_points += 1;
        //euchred! if winner of trick did not call trump
        if(game.called_trump == 0 || game.called_trump == 2) {
            game.t2_points += 1;
            cout << "euchred!" << endl;
        }
    }

    //march! if a team won all 5 tricks
    if(t1_tricks == 5) {
        cout << "march!" << endl;
        game.t1_points += 1;
    }
    if(t2_tricks == 5) {
        cout << "march!" << endl;
        game.t2_points += 1;
    }

    cout << *game.players[0] << " and " << *game.players[2] << " have " 
         << game.t1_points << " points" << endl;

    cout << *game.players[1] << " and " << *game.players[3] << " have " 
         << game.t2_points << " points" << endl << endl;

    ++game.deal_index;
    //ensure index of dealer is always 0-4
    if(game.deal_index >= 4) {
        game.deal_index = 0;
    }
    game.lead_index = game.deal_index + 1;
    if(game.lead_index >= 4) {
        game.lead_index = 0;
    }
    ++game.num_hand;
}

static void shuffle_deck(Game &game) {
    if(game.shuffle_mode == "shuffle") {
        game.pack.shuffle();
    }
}

static void deal(Game &game) {
    int left_index = game.deal_index + 1;
    if(left_index >= 4) {
        left_index = 0;
    }
    int cross_index = left_index + 1;
    if(cross_index >= 4) {
        cross_index = 0;
    }
    int right_index = cross_index + 1;
    if(right_index >= 4) {
        right_index = 0;
    }

    cout << *game.players[game.deal_index] << " deals" << endl;
    int i = 0;
    for(i = 0; i < 3; ++i) {
        game.players[left_index]->add_card(game.pack.deal_one());
    }
    for(i = 0; i < 2; ++i) {
        game.players[cross_index]->add_card(game.pack.deal_one());
    }
    for(i = 0; i < 3; ++i) {
        game.players[right_index]->add_card(game.pack.deal_one());
    }
    for(i = 0; i < 2; ++i) {
        game.players[game.deal_index]->add_card(game.pack.deal_one());
    }
    for(i = 0; i < 2; ++i) {
        game.players[left_index]->add_card(game.pack.deal_one());
    }
    for(i = 0; i < 3; ++i) {
        game.players[cross_index]->add_card(game.pack.deal_one());
    }
    for(i = 0; i < 2; ++i) {
        game.players[right_index]->add_card(game.pack.deal_one());
    }
    for(i = 0; i < 3; ++i) {
        game.players[game.deal_index]->add_card(game.pack.deal_one());
    }
}

static string make_trump(Game &game, Card &upcard) {
    int left_index = game.deal_index + 1;
    if(left_index >= 4) {
        left_index = 0;
    }
    int cross_index = left_index + 1;
    if(cross_index >= 4) {
        cross_index = 0;
    }
    int right_index = cross_index + 1;
    if(right_index >= 4) {
        right_index = 0;
    }

    int num_round = 1;
    bool order_up = false;
    string order_up_suit = Suit_next(upcard.get_suit());
    //2 rounds of making trump
    while(num_round < 3) {
        //does player to the left of dealer want to order up?
        order_up = game.players[left_index]->make_trump(upcard, 
                                                        *game.players[game.deal_index], 
                                                        num_round, order_up_suit);
        if(order_up) {
            cout << *game.players[left_index] << " orders up " << order_up_suit << endl;
            //during round 1, dealer picks up upcard and discards a card
            if(num_round == 1) {
                game.players[game.deal_index]->add_and_discard(upcard);
            }
            game.called_trump = left_index;
            return order_up_suit;
        }
        cout << *game.players[left_index] << " passes" << endl;

        //does player across from dealer want to order up?
        order_up = game.players[cross_index]->make_trump(upcard, 
                                                         *game.players[game.deal_index], 
                                                         num_round, order_up_suit);
        if(order_up) {
            cout << *game.players[cross_index] << " orders up " 
                 << order_up_suit << endl;
            if(num_round == 1) {
                game.players[game.deal_index]->add_and_discard(upcard);
            }
            game.called_trump = cross_index;
            return order_up_suit;
        }
        cout << *game.players[cross_index] << " passes" << endl;

        //does player to the right of dealer want to order up?
        order_up = game.players[right_index]->make_trump(upcard, 
                                                         *game.players[game.deal_index], 
                                                         num_round, order_up_suit);
        if(order_up) {
            cout << *game.players[right_index] << " orders up " 
                 << order_up_suit << endl;
            if(num_round == 1) {
                game.players[game.deal_index]->add_and_discard(upcard);
            }
            game.called_trump = right_index;
            return order_up_suit;
        }
        cout << *game.players[right_index] << " passes" << endl;

        //does dealer want to order up?
        order_up = game.players[game.deal_index]->make_trump(upcard, 
                                                             *game.players[game.deal_index], 
                                                             num_round, order_up_suit);
        if(order_up) {
            cout << *game.players[game.deal_index] << " orders up " 
                 << order_up_suit << endl;
            if(num_round == 1) {
                game.players[game.deal_index]->add_and_discard(upcard);
            }
            game.called_trump = game.deal_index;
            return order_up_suit;
        }
        cout << *game.players[game.deal_index] << " passes" << endl;
        ++num_round;
    }
    return order_up_suit;
}

static void play_trick(Game &game, const string &trump) {
    int left_index = game.lead_index + 1;
    if(left_index >= 4) {
        left_index = 0;
    }
    int cross_index = left_index + 1;
    if(cross_index >= 4) {
        cross_index = 0;
    }
    int right_index = cross_index + 1;
    if(right_index >= 4) {
        right_index = 0;
    }
    //card laying begins with leader, and moves clockwise
    Card card_led = game.players[game.lead_index]->lead_card(trump);
    cout << card_led << " led by " << *game.players[game.lead_index] << endl;

    Card left_play = game.players[left_index]->play_card(card_led, trump);
    cout << left_play << " played by " << *game.players[left_index] << endl;

    Card cross_play = game.players[cross_index]->play_card(card_led, trump);
    cout << cross_play << " played by " << *game.players[cross_index] << endl;

    Card right_play = game.players[right_index]->play_card(card_led, trump);
    cout << right_play << " played by " << *game.players[right_index] << endl;

    //determine which card won the trick
    Card biggest = won_trick(card_led, left_play, cross_play, right_play, trump);

    //determine which player won the trick
    if(biggest == card_led) {
        cout << *game.players[game.lead_index] << " takes the trick" << endl << endl;
    }
    if(biggest == left_play) {
        cout << *game.players[left_index] << " takes the trick" << endl << endl;
        //player who won trick is the next leader
        game.lead_index = left_index;
    }
    if(biggest == cross_play) {
        cout << *game.players[cross_index] << " takes the trick" << endl << endl;
        game.lead_index = cross_index;
    }
    if(biggest == right_play) {
        cout << *game.players[right_index] << " takes the trick" << endl << endl;
        game.lead_index = right_index;
    }
}

static Card won_trick(Card &card_led, Card &left_play, Card &cross_play, 
                        Card &right_play, const string &trump) {
    Card biggest = card_led;
    if(Card_less(biggest, left_play, card_led, trump)) {
        biggest = left_play;
    }
    if(Card_less(biggest, cross_play, card_led, trump)) {
        biggest = cross_play;
    }
    if(Card_less(biggest, right_play, card_led, trump)) {
        biggest = right_play;
    }
    return biggest;
}

static void errors(int argc, char **argv) {
    if(argc != 12) {
        cout << "Usage: euchre PACK_FILENAME [shuffle|noshuffle] POINTS_TO_WIN "
             << "NAME1 NAME2 NAME3 NAME4" << endl;
        exit(EXIT_FAILURE);
    }
    int points = atoi(argv[3]);
    if(!(points >= 1 && points <= 100)) {
        cout << "Usage: euchre PACK_FILENAME [shuffle|noshuffle] POINTS_TO_WIN "
             << "NAME1 NAME2 NAME3 NAME4" << endl;
        exit(EXIT_FAILURE);
    }
    string mode = argv[2];
    if(mode != "shuffle" && mode != "noshuffle") {
        cout << "Usage: euchre PACK_FILENAME [shuffle|noshuffle] POINTS_TO_WIN "
             << "NAME1 NAME2 NAME3 NAME4" << endl;
        exit(EXIT_FAILURE);
    }
    string type = argv[5];
    if(type != "Simple" && type != "Human") {
        cout << "Usage: euchre PACK_FILENAME [shuffle|noshuffle] POINTS_TO_WIN "
             << "NAME1 NAME2 NAME3 NAME4" << endl;
        exit(EXIT_FAILURE);
    }
    type = argv[7];
    if(type != "Simple" && type != "Human") {
        cout << "Usage: euchre PACK_FILENAME [shuffle|noshuffle] POINTS_TO_WIN "
             << "NAME1 NAME2 NAME3 NAME4" << endl;
        exit(EXIT_FAILURE);
    }
    type = argv[9];
    if(type != "Simple" && type != "Human") {
        cout << "Usage: euchre PACK_FILENAME [shuffle|noshuffle] POINTS_TO_WIN "
             << "NAME1 NAME2 NAME3 NAME4" << endl;
        exit(EXIT_FAILURE);
    }
    type = argv[11];
    if(type != "Simple" && type != "Human") {
        cout << "Usage: euchre PACK_FILENAME [shuffle|noshuffle] POINTS_TO_WIN "
             << "NAME1 NAME2 NAME3 NAME4" << endl;
        exit(EXIT_FAILURE);
    }
}



