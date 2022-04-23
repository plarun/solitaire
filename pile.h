#ifndef __PILE_H__
#define __PILE_H__

#include <stdbool.h>

#include "card.h"
#include "dlist.h"

card_t* front_card(dlist_t* list);

card_t* back_card(dlist_t* list);


// Represents the stock that contains rest of the cards
typedef struct stock_t {
	dlist_t* cards;
} stock_t;

stock_t* new_stock();

dnode_t* stock_next(stock_t* stock);


// Represents the waste where the card from the stock can be used
typedef struct waste_t {
	dlist_t* cards;
} waste_t;

waste_t* new_waste();

dnode_t* waste_use_card(waste_t* waste);


// Represents one of the four foundations
typedef struct foundation_t {
	dlist_t* cards;
	suit_t suit;
} foundation_t;

foundation_t* new_foundation();

// Represents the foundations for each suit
typedef struct foundations_t {
	foundation_t* home[4];
} foundations_t;

foundations_t* new_foundations();


// Represents seven piles of cards
typedef struct Tableau {
	dlist_t* piles[7];
} tableau_t;

tableau_t* new_tableau();

typedef enum Action {
	ACTION_STOCK_TO_WASTE,
	ACTION_WASTE_TO_TABLEAU,
	ACTION_WASTE_TO_FOUNDATION,
	ACTION_TABLEAU_TO_FOUNDATION,
	ACTION_MOVE_IN_TABLEAU,
	ACTION_MOVE_N_IN_TABLEAU
} action_t;

typedef enum Status {
	STATUS_OK,
	STATUS_IN_PROGRESS,
	STATUS_INVALID_CMD,
	STATUS_INVALID,
	STATUS_SOURCE_EMPTY,
	STATUS_SOURCE_INVALID,
	STATUS_DESTINATION_INVALID
} status_t;

typedef struct Move {
	char source, destination;
	int ncards;
	action_t action;
	status_t status;
} move_t;

// Represents game
typedef struct Game {
	stock_t* stock;
	waste_t* waste;
	foundations_t* foundations;
	tableau_t* tableau;
	status_t last_status;
} game_t;

game_t* new_game();

void shuffle_deck(card_t* deck[]);

void fill_cards(card_t* deck[], stock_t* stock, tableau_t* tableau);

status_t try_action(game_t* game, const char* command);

const char* get_status_desc(status_t status);

const char* get_action_desc(action_t action);

#endif // __PILE_H__
