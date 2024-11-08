// List.c - implementation of the List ADT using a doubly linked list

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "List.h"

// concrete data structure
struct node {
	int value;
	struct node *next;
	struct node *prev;
};

struct list {
	struct node *head; // pointer to the first node
	struct node *tail; // pointer to the last node
	int size;          // number of elements
};

/**
 * Creates a new empty list
 */
List ListNew(void) {
	List l = malloc(sizeof(*l));
	if (l == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}

	l->head = NULL;
	l->tail = NULL;
	l->size = 0;
	return l;
}

/**
 * Frees all memory allocated to the list
 */
void ListFree(List l) {
	struct node *curr = l->head;
	while (curr != NULL) {
		struct node *temp = curr;
		curr = curr->next;
		free(temp);
	}
	free(l);
}

// !!! DO NOT MODIFY THE CODE ABOVE !!!
////////////////////////////////////////////////////////////////////////

/**
 * Adds an element to the start of the list
 */
void ListAddStart(List l, int elem) {
	struct node *n = malloc(sizeof(struct node));
	n->value = elem;

	if (l->size == 0) {
		n->next = NULL;
	} else {
		n->next = l->head;
		n->next->prev = n;
	}
	n->prev = NULL;

	l->head = n;
	if (l->size == 0) {
		l->tail = n;
	}

	l->size++;
}

/**
 * Adds an element to the end of the list
 */
void ListAddEnd(List l, int elem) {
	struct node *n = malloc(sizeof(struct node));
	n->value = elem;

	n->next = NULL;
	if (l->size == 0) {
		n->prev = NULL;
	} else {
		n->prev = l->tail;
		n->prev->next = n;
	}

	l->tail = n;
	if (l->size == 0) {
		l->head = n;
	}

	l->size++;
}

/**
 * Deletes an element from the start of the list and returns it
 * Assumes that the list is not empty
 * This function is complete, there is no need to modify it
 */
int ListDeleteStart(List l) {
	assert(l->size > 0);

	struct node *nodeToDelete = l->head;
	l->head = l->head->next;

	// if there was a second node, then its prev should now be null
	if (nodeToDelete->next != NULL) {
		nodeToDelete->next->prev = NULL;

	// otherwise, we are deleting the only node from the list, so tail
	// should now be null
	} else {
		l->tail = NULL;
	}

	int valToReturn = nodeToDelete->value;
	free(nodeToDelete);
	l->size--;

	return valToReturn;
}

/**
 * Deletes an element from the end of the list and returns it
 * Assumes that the list is not empty
 */
int ListDeleteEnd(List l) {
	assert(l->size > 0);

	struct node *nodeToDelete = l->tail;
	l->tail = l->tail->prev;

	// if there was a second last node, then its next should now be null
	if (nodeToDelete->prev != NULL) {
		nodeToDelete->prev->next = NULL;

	// otherwise, we are deleting the only node from the list, so head should now be null
	} else {
		l->head = NULL;
	}

	int valToReturn = nodeToDelete->value;
	free(nodeToDelete);
	l->size--;

	return valToReturn;
}

////////////////////////////////////////////////////////////////////////
// !!! DO NOT MODIFY THE CODE BELOW !!!

/**
 * Returns the first element in the list
 * Assumes that the list is not empty
 */
int ListFirst(List l) {
	assert(l->size > 0);

	return l->head->value;
}

/**
 * Returns the last element in the list
 * Assumes that the list is not empty
 */
int ListLast(List l) {
	assert(l->size > 0);

	return l->tail->value;
}

/**
 * Returns the number of elements in the list
 */
int ListSize(List l) {
	return l->size;
}

/**
 * Returns true if the given element is in the list, and false otherwise
 */
bool ListContains(List l, int elem) {
	for (struct node *curr = l->head; curr != NULL; curr = curr->next) {
		if (curr->value == elem) {
			return true;
		}
	}
	return false;
}

/**
 * Prints the given list to stdout
 */
void ListShow(List l) {
	printf("[");
	for (struct node *curr = l->head; curr != NULL; curr = curr->next) {
		printf("%d", curr->value);
		if (curr->next != NULL) {
			printf(", ");
		}
	}
	printf("]");
}
