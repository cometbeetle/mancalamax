/*
 * project:  Mancalamax
 * file:     LinkedList.h
 * author:   Ethan Mentzer
 * modified: 2025-03-08
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H


/**
 * Linked list node implementation for integers.
 */
typedef struct Node* Node;

extern Node new_Node(int value);
extern void Node_free(Node node);
extern int Node_value(Node node);
extern Node Node_next(Node node);

/**
 * Linked list implementation for integer Nodes.
 */
typedef struct LinkedList* LinkedList;

extern LinkedList new_LinkedList();
extern void LinkedList_free(LinkedList list);
extern size_t LinkedList_size(LinkedList list);
extern void LinkedList_append(LinkedList list, int value);
extern void LinkedList_prepend(LinkedList list, int value);
extern int LinkedList_get(LinkedList list, int index);
extern Node LinkedList_head(LinkedList list);
extern Node LinkedList_tail(LinkedList list);


#endif //LINKEDLIST_H
