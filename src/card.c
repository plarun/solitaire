#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <memory.h>
#include <stddef.h>

#include "card.h"

const char* card_ranks[N_RANK] = { " A", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9", "10", " J", " Q", " K" };

const char* suit_icons[] = { "\u2660", "\u2663", "\u2665", "\u2666" };

#define to_rank_name(v) card_ranks[v]

// Creates a new card by given rank and suit
card_t* new_card(unsigned short rank, suit_t suit) {
	if (!is_valid_rank(rank)) {
		fprintf(stderr, "Invalid card value: %s\n", to_rank_name(rank));
		exit(EXIT_FAILURE);
	}

	card_t* card = malloc(sizeof(card_t));
	if (card == NULL) {
		fprintf(stderr, "Failed to create a new card.\n");
		exit(EXIT_FAILURE);
	}

	card->suit = suit;
	card->rank = rank;
	card->is_hidden = false;
	return card;
}

// Creates a deck of 52 cards
card_t** new_deck() {
	card_t** deck = malloc(N_CARD * sizeof(card_t*));
	if (deck == NULL) {
		fprintf(stderr, "Cannot create deck.\n");
		exit(EXIT_FAILURE);
	}

	int i = 0;	
	for (suit_t suit = 0; suit < N_SUIT; ++suit)
		for (unsigned short rank = 0; rank < N_RANK; ++rank)
			deck[i++] = new_card(rank, suit);

	return deck;
}

// Validates the rank - it should be the valid index to array card_ranks
bool is_valid_rank(const unsigned short rank) {
	return ((rank >= 0) && (rank < 13));
}

// Gets card color
int card_color(const card_t* card) {
	return ((card->suit == spade || card->suit == clover) ? CARD_COLOR_BLACK : CARD_COLOR_RED);
}

// Checks if the given card's color is black
bool is_black_card(const card_t* card) {
	return (card_color(card) == CARD_COLOR_BLACK);
}

// Checks if the cards have alternate color
bool is_alternate_color(const card_t* card1, const card_t* card2) {
	if (card1 == NULL)
		return false;
	if (card2 == NULL && card1->rank == 12)
		return true;
	return (is_black_card(card1) != is_black_card(card2));
}

// Checks if the rank of card_1 is 1 smaller than rank of card_2
bool is_below_rank(const card_t* card1, const card_t* card2) {
	if (card1 == NULL)
		return false;
	if (card2 == NULL && card1->rank == 12)
		return true;
	return (card2->rank - card1->rank) == 1;
}

// Checks if the rank of card_1 is 1 larger than rank of card_2
bool is_above_rank(const card_t* card1, const card_t* card2) {
	return (card1->rank - card2->rank == 1);
}

bool is_valid_shift(const card_t* rank_low_card, const card_t* rank_high_card) {
	if (rank_low_card == NULL || (rank_low_card->rank == 12 && rank_high_card != NULL))
		return false;
	bool alternate_color = is_alternate_color(rank_low_card, rank_high_card);
	bool below_rank = is_below_rank(rank_low_card, rank_high_card);
	return alternate_color && below_rank;
}

// Prints the given card
void print_card(const card_t* card) {
	if (card == NULL || card->is_hidden) {
		printw("[----]");
		return;
	}
	const char* suit_icon = suit_icons[card->suit];
	int card_color = is_black_card(card) ? CARD_COLOR_BLACK : CARD_COLOR_RED;
	printw("[");
	attron(COLOR_PAIR(card_color));
	printw("%s%s", to_rank_name(card->rank), suit_icon); 
	attroff(COLOR_PAIR(card_color));
	printw(" ]");
}
