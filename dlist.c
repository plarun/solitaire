#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "dlist.h"
#include "util.h"

dnode_t* new_dnode(void* card) {
	dnode_t* node = malloc(sizeof(dnode_t));
	exit_if_null(node, "Unable to create dnode_t object.");
	node->card = card;
	node->prev = node->next = NULL;
	return node;
}

dlist_t* new_dlist() {
	dlist_t* list = malloc(sizeof(dlist_t));
	exit_if_null(list, "Unable to create dlist_t object.");
	list->head = list->tail = NULL;
	list->size = 0;
	return list;
}

bool dlist_push_front(dlist_t* list, void* card) {
	if (list == NULL || card == NULL)
		return false;
	dnode_t* node = new_dnode(card);
	if (list->size == 0) {
		list->head = list->tail = node;
	} else {
		list->head->prev = node;
		node->next = list->head;
		list->head = node;
	}
	++list->size;
	return true;
}

bool dlist_push_back(dlist_t* list, void* card) {
	if (list == NULL || card == NULL)
		return false;
	dnode_t* node = new_dnode(card);
	if (list->size == 0) {
		list->head = list->tail = node;
	} else {
		list->tail->next = node;
		node->prev = list->tail;
		list->tail = node;
	}
	++list->size;
	return true;
}

dnode_t* dlist_front(dlist_t* list) {
	if (list == NULL)
		return NULL;
	return list->head;
}

dnode_t* dlist_back(dlist_t* list) {
	if (list == NULL)
		return NULL;
	return list->tail;
}

dnode_t* dlist_get(dlist_t* list, const int pos) {
	if (pos < 1 || list == NULL || list->size == 0 || pos > list->size)
		return NULL;
	if (pos == 1)
		return dlist_front(list);
	else if (pos == list->size)
		return dlist_back(list);

	dnode_t* curr = list->head;
	for (int i = 1; i < pos; ++i, curr = curr->next);
	return curr;
}

dnode_t* dlist_pop_front(dlist_t* list) {
	if (list == NULL || list->size == 0)
		return NULL;
	dnode_t* curr_head = list->head;
	if (list->size == 1) {
		list->head = list->tail = NULL;
	} else {
		dnode_t* new_head = curr_head->next;
		list->head = new_head;
		new_head->prev = curr_head->next = NULL;
		if (list->size == 2)
			list->tail = new_head;
	}
	--list->size;
	return curr_head;
}

dnode_t* dlist_pop_back(dlist_t* list) {
	if (list == NULL || list->size == 0)
		return NULL;
	dnode_t* curr_tail = list->tail;
	if (list->size == 1) {
		list->head = list->tail = NULL;
	} else {
		dnode_t* new_tail = curr_tail->prev;
		list->tail = new_tail;
		new_tail->next = curr_tail->prev = NULL;
		if (list->size == 2)
			list->head = new_tail;
	}
	--list->size;
	return curr_tail;
}

dnode_t* dlist_unlink_nodes(dlist_t* list, const int n) {
	if (list == NULL || list->size == 0 || list->size < n)
		return NULL;
	dnode_t* unode = list->tail;
	for (int i = 1; i < n; ++i)
		unode = unode->prev;
	if (list->size == n) {
		list->head = list->tail = NULL;
	} else {
		list->tail = unode->prev;
		list->tail->next = NULL;
		unode->prev = NULL;
	}
	list->size -= n;
	return unode;
}

bool dlist_link_node(dlist_t* list, dnode_t* lnode) {
	if (list == NULL || lnode == NULL)
		return false;
	if (list->size == 0) {
		list->head = lnode;
	} else {
		list->tail->next = lnode;
		lnode->prev = list->tail;
	}
	dnode_t* curr = lnode;
	while (curr != NULL) {
		list->tail = curr;
		curr = curr->next;
		++list->size;
	}
	return true;
}
