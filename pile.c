#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>

#include "pile.h"
#include "dlist.h"
#include "util.h"

card_t* front_card(dlist_t* list) {
	dnode_t* node = dlist_front(list);
	return (node == NULL) ? NULL : node->card;
}

card_t* back_card(dlist_t* list) {
	dnode_t* node = dlist_back(list);
	return (node == NULL) ? NULL : node->card;
}

stock_t* new_stock() {
	stock_t* stock = malloc(sizeof(stock_t));
	exit_if_null(stock, "Unable to create stock_t object.");	
	stock->cards = new_dlist();
	return stock;
}

waste_t* new_waste() {
	waste_t* waste = malloc(sizeof(waste_t));
	exit_if_null(waste, "Unable to create waste_t object.");
	waste->cards = new_dlist();
	return waste;
}

tableau_t* new_tableau() {
	tableau_t* tableau = malloc(sizeof(tableau_t));
	exit_if_null(tableau, "Unable to create tableau_t object.");
	for (int i = 0; i < 7; ++i)
		tableau->piles[i] = new_dlist();
	return tableau;
}

foundation_t* new_foundation() {
	foundation_t* foundation = malloc(sizeof(foundation_t));
	exit_if_null(foundation, "Unable to create foundation_t object.");
	foundation->cards = new_dlist();
	return foundation;
}

foundations_t* new_foundations() {
	foundations_t* foundations = malloc(sizeof(foundations_t));
	exit_if_null(foundations, "Unable to create foundations_t object.");
	for (int i = 0; i < N_SUIT; ++i) {
		foundations->home[i] = new_foundation();
		foundations->home[i]->suit = i;
	}
	return foundations;
}

game_t* new_game() {
	game_t* game = malloc(sizeof(game_t));
	exit_if_null(game, "Unable to create game_t object.");
	game->stock = new_stock();
	game->waste = new_waste();
	game->foundations = new_foundations();
	game->tableau = new_tableau();
	game->last_status = STATUS_OK;
	return game;
}

void shuffle_deck(card_t** deck) {
	card_t* tmpcard = NULL;
	for (short i = 0; i < N_CARD; ++i) {
		int j = i + rand() / (RAND_MAX / (N_CARD - i) + 1);
		tmpcard = deck[j];
		deck[j] = deck[i];
		deck[i] = tmpcard;
	}
}

void fill_cards(card_t* shuffled_deck[], stock_t* stock, tableau_t* tableau) {
	int icard = 0;
	for (int i = 0; i < 7; ++i)
		for (int j = i; j >= 0; --j) {
			if (j != 0)
				shuffled_deck[icard]->is_hidden = true;
			dlist_push_back(tableau->piles[i], shuffled_deck[icard++]);
		}

	while (icard < N_CARD)
		dlist_push_back(stock->cards, shuffled_deck[icard++]);
}

const char* status_desc[] = {
	"Ok",
	"In Progress",
	"Invalid command",
	"Invalid action",
	"Source pile is empty",
	"Invalid source",
	"Invalid destination"
};

const char* action_desc[] = {
	"ACTION_STOCK_TO_WASTE",
	"ACTION_WASTE_TO_TABLEAU",
	"ACTION_WASTE_TO_FOUNDATION",
	"ACTION_TABLEAU_TO_FOUNDATION",
	"ACTION_MOVE_IN_TABLEAU",
};

const char* get_status_desc(status_t status) {
	return status_desc[status];
}

const char* get_action_desc(action_t action) {
	return action_desc[action];
}

move_t* new_move() {
	move_t* move = malloc(sizeof(move_t));
	exit_if_null(move, "Unable to create move_t object.");
	move->source = '\0';
	move->destination = '\0';
	move->ncards = 0;
	return move;
}

static move_t* parse_command(const char* command) {
	move_t* move = new_move();
	move->status = STATUS_IN_PROGRESS;
	
	if (sscanf(command, "%[s]", &(move->source)) == 1) {
		move->action = ACTION_STOCK_TO_WASTE;
		move->destination = 'w';
	} else if (sscanf(command, "%[w] %[a-g]", &(move->source), &(move->destination)) == 2) {
		move->ncards = 1;
		move->action = ACTION_WASTE_TO_TABLEAU;
	} else if (sscanf(command, "%[w] %[h]", &(move->source), &(move->destination)) == 2) {
		move->ncards = 1;
		move->action = ACTION_WASTE_TO_FOUNDATION;
	} else if (sscanf(command, "%[a-g] %[a-g]", &(move->source), &(move->destination)) == 2) {
		move->ncards = 1;
		move->action = ACTION_MOVE_IN_TABLEAU;
	} else if (sscanf(command, "%[a-g]%d %[a-g]", &(move->source), &(move->ncards), &(move->destination)) == 3) {
		move->action = ACTION_MOVE_IN_TABLEAU;
	} else if (sscanf(command, "%[a-g] %[h]", &(move->source), &(move->destination)) == 2) {
		move->ncards = 1;
		move->action = ACTION_TABLEAU_TO_FOUNDATION;
	} else {
		move->status = STATUS_INVALID_CMD;
	}
	return move;		
}

/*
 * Direction of cards
 * deck to tableau: push_back
 * deck to stock: push_back
 * stock to waste: pop_back, push_back
 * redeal(waste to stock): pop_front, push_back
 * move(waste to tableau): pop_back, push_back
 * move(tableau to tableau): pop_back, push_back
 * move(tableau to foundations): pop_back, push_back
 * move(waste to foundations): pop_back, push_back
 * */

static void reveal_last_card(dlist_t* cards) {
	if (cards->size > 0) {
		card_t* card = back_card(cards);
		card->is_hidden = false;
	}
}

static void redeal(stock_t* stock, waste_t* waste) {
	while (waste->cards->size != 0) {
		dnode_t* node = dlist_pop_back(waste->cards);
		dlist_push_back(stock->cards, node->card);
	}
}

static status_t move_card(dlist_t* src, dlist_t* dst, card_comp_ft can_move) {
	card_t* src_card = (back_card(src));
	card_t* dst_card = (back_card(dst));
	if (can_move != NULL && !can_move(src_card, dst_card))
		return STATUS_INVALID;
	debug_printw_now("after valid shift check in move_card\n");
	dnode_t* node = dlist_pop_back(src);
	dlist_push_back(dst, node->card);
	return STATUS_OK;
}

static status_t stock_to_waste(stock_t* stock, waste_t* waste, move_t* move) {
	if (move->source != 's')
		return STATUS_SOURCE_INVALID;
	if (move->destination != 'w')
		return STATUS_DESTINATION_INVALID;
	if (stock->cards->size == 0 && waste->cards->size > 0)
		redeal(stock, waste);
	if (stock->cards->size == 0 && waste->cards->size == 0)
		return STATUS_SOURCE_EMPTY;
	dnode_t* node = dlist_pop_back(stock->cards);
	dlist_push_back(waste->cards, node->card);
	return STATUS_OK;
}

static status_t waste_to_tableau(waste_t* waste, tableau_t* tableau, move_t* move) {
	if (move->source != 'w')
		return STATUS_SOURCE_INVALID;
	if (move->destination < 97 || move->destination > 103)
		return STATUS_DESTINATION_INVALID;
	int p = (int) move->destination - 97;
	return move_card(waste->cards, tableau->piles[p], is_valid_shift);
}

static status_t move_to_foundation(dlist_t* cards, foundation_t* foundation) {
	card_t* card = back_card(cards);
	int fsize = foundation->cards->size;
	if (fsize == 0) {
		if (card->rank == 0)    // ace rank
			return move_card(cards, foundation->cards, NULL);
		return STATUS_INVALID;
	}
	return move_card(cards, foundation->cards, is_above_rank);
}

static status_t waste_to_foundations(waste_t* waste, foundations_t* foundations, move_t* move) {
	if (move->source != 'w')
		return STATUS_SOURCE_INVALID;
	if (move->destination != 'h')
		return STATUS_DESTINATION_INVALID;
	card_t* card = back_card(waste->cards);
	foundation_t* foundation = foundations->home[card->suit];
	return move_to_foundation(waste->cards, foundation);
}

static status_t tableau_to_foundations(tableau_t* tableau, foundations_t* foundations, move_t* move) {
	if (move->source < 97 || move->source > 103)
		return STATUS_SOURCE_INVALID;
	if (move->destination != 'h')
		return STATUS_DESTINATION_INVALID;
	dlist_t* pile = tableau->piles[move->source - 97];
	card_t* card = back_card(pile);
	foundation_t* foundation = foundations->home[card->suit];
	status_t status = move_to_foundation(pile, foundation);
	if (status == STATUS_OK)
		reveal_last_card(pile);
	return status;
}

/*
 * src pile should not be empty
 * src and dst should be diff
 * if dst pile is empty then src_card or first card of src_cards should be rank_king
 * if src pile has only one card - handle it properly
 * if ncards=1 then move_card to dst then reveal the back card at src pile
 * if ncards>1 then unlink last n nodes from src then link to dst pile
 * */
static status_t tableau_pile_to_pile(tableau_t* tableau, move_t* move) {
	if (move->source < 97 || move->source > 103)
		return STATUS_SOURCE_INVALID;
	if (move->destination < 97 || move->destination > 103)
		return STATUS_DESTINATION_INVALID;
	if (move->source == move->destination)
		return STATUS_INVALID;

	dlist_t* src_pile = tableau->piles[move->source - 97];
	dlist_t* dst_pile = tableau->piles[move->destination - 97];
	
	if (src_pile->size < move->ncards)
		return STATUS_SOURCE_EMPTY;
	
	int pos = src_pile->size - move->ncards + 1;
	dnode_t* src_node = dlist_get(src_pile, pos);
	if (src_node == NULL)
		return STATUS_INVALID;

	card_t* src_card = src_node->card;
	card_t* dst_card = back_card(dst_pile);

	if ((dst_pile->size == 0 && src_card->rank != 12) || !is_valid_shift(src_card, dst_card))
		return STATUS_INVALID;
	
	src_node = dlist_unlink_nodes(src_pile, move->ncards);
	dlist_link_node(dst_pile, src_node);
	reveal_last_card(src_pile);
	return STATUS_OK;
}

status_t try_action(game_t* game, const char* command) {
	move_t* move = parse_command(command);
	if (move->status == STATUS_INVALID_CMD) {
		move->action = 0;
	}
	debug_print_move(move);

	switch (move->action) {
		case ACTION_STOCK_TO_WASTE:
			return stock_to_waste(game->stock, game->waste, move);
		case ACTION_WASTE_TO_TABLEAU:
			return waste_to_tableau(game->waste, game->tableau, move);
		case ACTION_WASTE_TO_FOUNDATION:
			return waste_to_foundations(game->waste, game->foundations, move);
		case ACTION_MOVE_IN_TABLEAU:
			return tableau_pile_to_pile(game->tableau, move);
		case ACTION_TABLEAU_TO_FOUNDATION:
			return tableau_to_foundations(game->tableau, game->foundations, move);
		default:
			break;
	}
	
	return move->status;
}
