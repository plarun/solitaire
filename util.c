#include <stdlib.h>
#include <ncurses.h>

#include "util.h"
#include "pile.h"
#include "card.h"

bool is_verbose = false;

void set_verbose(bool vflag) {
	is_verbose = vflag;
}

void exit_if_null(void* ptr, const char* err_msg) {
	if (ptr == NULL) {
		fprintf(stderr, "%s\n", err_msg);
		exit(EXIT_FAILURE);
	}
}

void debug_printw_now(const char* msg) {
	if (!is_verbose) return;
        printw("%s", msg);
        refresh();
}

void debug_print_move(move_t* move) {
	if (!is_verbose) return;
	printw("Move: s=%c d=%c nc=%d action=%s\n", move->source, move->destination, move->ncards, get_action_desc(move->action));
	refresh();
}

void debug_print_list(dlist_t* list, const char* name) {
	if (!is_verbose) return;
        printw("%s: ", name);
        for (dnode_t* curr = list->head; curr != NULL; curr = curr->next) {
		print_card(curr->card);
		refresh();
	}
        printw("\n");
	refresh();
}

void debug_show_containers(const game_t* game) {
	if (!is_verbose) return;
	debug_print_list(game->stock->cards, "Stock");
	debug_print_list(game->waste->cards, "Waste");
	for (int i = 0; i < 7; ++i) {
		debug_print_list(game->tableau->piles[i], "Tableau");
	}
	refresh();
}

