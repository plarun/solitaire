#ifndef __UI_H__
#define __UI_H__

#include <stdbool.h>

#include "card.h"
#include "pile.h"

#define N_TABLEAU_ROW 20
#define N_TABLEAU_COL 7
#define CMD_LEN 20

void render_gap(const unsigned int n);

void render_status(const char* msg);

void render_stock_header(stock_t* stock);

void render_waste_header(waste_t* waste);

void render_foundations_header(foundations_t* foundation);

void render_tableau_header(tableau_t* tableau);

void render_stock(stock_t* stock);

void render_waste(waste_t* waste);

void render_foundations(foundations_t* foundations);

void render_tableau(tableau_t* tableau);

void render_last_status(status_t status);

#endif // __UI_H__
