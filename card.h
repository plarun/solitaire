#ifndef __CARD_H__
#define __CARD_H__

#include <stdbool.h>

#define N_RANK 13
#define N_SUIT 4
#define N_CARD (N_SUIT * N_RANK)
#define CARD_COLOR_BLACK 1
#define CARD_COLOR_RED 2
#define DEFAULT_COLOR -1

extern const char* card_ranks[N_RANK];

typedef enum suit_t { spade, clover, heart, diamond } suit_t;

extern const char* suit_names[];
extern const char* suit_icons[];

// Represents the card in the game
typedef struct card_t {
	suit_t suit;
	unsigned short rank;
	bool is_hidden;
} card_t;

typedef bool (*card_comp_ft)(const card_t* card1, const card_t* card2);

card_t* new_card(unsigned short rank, suit_t suit);

card_t** new_deck();

bool is_valid_rank(const unsigned short rank);

bool is_black_card(const card_t* card);

bool is_valid_shift(const card_t* card1, const card_t* card2);

bool is_below_rank(const card_t* card1, const card_t* card2);

bool is_above_rank(const card_t* card1, const card_t* card2);

bool is_alternate_rank(const card_t* card1, const card_t* card2);

void print_card(const card_t* card);

#endif // __CARD_H__
