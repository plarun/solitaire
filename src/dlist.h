#ifndef __DLIST_H__
#define __DLIST_H__

#include <stdbool.h>

typedef struct Dnode {
	void* card;
	struct Dnode* next;
	struct Dnode* prev;
} dnode_t;

dnode_t* new_dnode(void* card);

typedef struct Dlist {
	dnode_t* head;
	dnode_t* tail;
	int size;
} dlist_t;

dlist_t* new_dlist();

bool dlist_push_front(dlist_t* list, void* card);

bool dlist_push_back(dlist_t* list, void* card);

bool dlist_remove(dlist_t* list, dnode_t* node);

dnode_t* dlist_get(dlist_t* list, const int pos);

dnode_t* dlist_front(dlist_t* list);

dnode_t* dlist_back(dlist_t* list);

dnode_t* dlist_pop_front(dlist_t* list);

dnode_t* dlist_pop_back(dlist_t* list);

dnode_t* dlist_unlink_nodes(dlist_t* list, const int n);

bool dlist_link_node(dlist_t* list, dnode_t* node);

#endif // __DLIST_H__
