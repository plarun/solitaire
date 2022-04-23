#ifndef __UTIL_H__
#define __UTIL_H__

#include "dlist.h"
#include "pile.h"

void set_verbose(bool vflag);

void exit_if_null(void* ptr, const char* err_msg);

void debug_printw_now(const char* msg);

void debug_print_move(move_t* move);

void debug_print_list(dlist_t* list, const char* name);

void debug_show_containers(const game_t* game);

#endif // __UTIL_H__
