#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>

#include "ui.h"

void render_gap(const unsigned int n) {
	for (int i = 0; i < n; ++i)
		printw(" ");
}

void render_status(const char* msg) {
	printw("Move status: %s\n", msg);
}

void render_stock_header(stock_t* stock) {
	printw("(s %02d)", stock->cards->size);
}

void render_waste_header(waste_t* waste) {
	printw("(w %02d)", waste->cards->size);
}

void render_foundations_header(foundations_t* foundations) {
	for (int i = 0; i < N_SUIT; ++i) {
		printw("(h %02d)", foundations->home[i]->cards->size);
		if (i < N_SUIT - 1)
			render_gap(4);
	}
}

void render_tableau_header(tableau_t* tableau) {
	for (int i = 97; i <= 103; ++i) {
		printw("(%c %02d)", i, tableau->piles[i - 97]->size);
		render_gap(4);
	}
}

void render_stock(stock_t* stock) {
	print_card(back_card(stock->cards));
}

void render_waste(waste_t* waste) {
	print_card(back_card(waste->cards));
}

void render_foundations(foundations_t* foundations) {
	for (int i = 0; i < N_SUIT; ++i) {
		print_card(back_card(foundations->home[i]->cards));
		if (i < N_SUIT - 1)
			render_gap(4);
	}
}

void render_tableau(tableau_t* tableau) {
	int depth[N_TABLEAU_COL];
	for (int i = 0; i < N_TABLEAU_COL; ++i)
		depth[i] = tableau->piles[i]->size;

	for (int r = 0; r < N_TABLEAU_ROW; ++r) {
		for (int c = 0; c < N_TABLEAU_COL; ++c) {
			if (depth[c] > 0) {
				dnode_t* node = dlist_get(tableau->piles[c], r+1);
				if (node == NULL)
					continue;
				print_card(node->card);
				render_gap(4);
				depth[c]--;
			} else {
				render_gap(10);
			}
		}
		printw("\n");
	}
}

void render_last_status(status_t status) {
	printw("Status: %s\n", get_status_desc(status));
}
