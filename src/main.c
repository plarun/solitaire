#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ncurses.h>
#include <memory.h>
#include <locale.h>
#include <time.h>

#include "card.h"
#include "pile.h"
#include "ui.h"
#include "util.h"

static void init_curses() {
	initscr();
	keypad(stdscr, TRUE);
	use_default_colors();
	start_color();
	init_pair(CARD_COLOR_BLACK, DEFAULT_COLOR, DEFAULT_COLOR);
	init_pair(CARD_COLOR_RED, COLOR_RED, DEFAULT_COLOR);
}

static void end_curses() {
	endwin();
}

static void render_game_board(game_t* game) {
	stock_t* stock = game->stock;
	waste_t* waste = game->waste;
	foundations_t* foundations = game->foundations;
	tableau_t* tableau = game->tableau;

	render_stock_header(stock);
	render_gap(4);
	render_waste_header(waste);
	render_gap(14);
	render_foundations_header(foundations);
	printw("\n");
	render_stock(stock);
	render_gap(4);
	render_waste(waste);
	render_gap(14);
	render_foundations(foundations);
	printw("\n\n");
	render_tableau_header(tableau);
	printw("\n");
	render_tableau(tableau);
	printw("\n");
	render_last_status(game->last_status);
	printw("\n");
}

static void prompt_input(char buf[]) {
	printw("cli >");
	getstr(buf);
}

static bool check_if_won(game_t* game) {
	for (int i = 0; i < N_SUIT; ++i)
		if (game->foundations->home[i]->cards->size != 13)
			return false;
	return true;
}

static void start_game(game_t* game) {
	char command[CMD_LEN] = "";
	bool run_flag = true;

	while (run_flag) {
		debug_printw_now("start\n");
		render_game_board(game);
		prompt_input(command);
		erase();
		if (strcmp("quit", command) == 0)
			run_flag = false;
		status_t status = try_action(game, command);
		game->last_status = status;
		if (check_if_won(game))
			return;
		debug_show_containers(game);
	}
}

static void end_game() {
	printw("Won the game!\n");
}

int main(int argc, char* argv[]) {
	if (argc == 2 && strcmp(argv[1], "-v") == 0)
		set_verbose(true);
	srand(time(NULL));
	setlocale(LC_ALL, "");
	init_curses();

	card_t** deck = new_deck();
	shuffle_deck(deck);

	game_t* game = new_game();
	
	fill_cards(deck, game->stock, game->tableau);
	free(deck);
	debug_show_containers(game);
	start_game(game);
	end_game();

	refresh();	
	end_curses();
}
