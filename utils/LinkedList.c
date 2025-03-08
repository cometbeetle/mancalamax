/*
 * project:  Mancalamax
 * file:     LinkedList.c
 * author:   Ethan Mentzer
 * modified: 2025-03-08
 */

#include <stdlib.h>
#include <limits.h>

#include "LinkedList.h"


/**
 * Linked list node implementation for integers.
 */
struct Node {
    int value;
    Node next;
};

Node new_Node(const int value) {
    struct Node* const newNode = (Node)malloc(sizeof(struct Node));
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

void Node_free(Node node) {
    if (node == NULL) return;
    free(node);
}

int Node_value(Node node) {
    return node->value;
}

Node Node_next(Node node) {
    return node->next;
}

// ------------------------------------------------------------------------------------------


/**
 * Linked list implementation for integer Nodes.
 */
struct LinkedList {
    Node head;
    Node tail;
    size_t size;
};

LinkedList new_LinkedList() {
    struct LinkedList* const newList = (LinkedList)malloc(sizeof(struct LinkedList));
    newList->head = NULL;
    newList->tail = NULL;
    newList->size = 0;
    return newList;
}

void LinkedList_free(LinkedList list) {
    if (list == NULL) return;

    Node toFree = list->head;

    while (toFree != NULL) {
        struct Node* const next = toFree->next;
        free(toFree);
        toFree = next;
    }

    free(list);
}

/**
 * Returns the number of nodes in the specified list.
 */
size_t LinkedList_size(LinkedList list) {
    if (list == NULL) return -1;
    return list->size;
}

/**
 * Append an element to the specified list.
 *
 * This operation is O(1).
 */
void LinkedList_append(LinkedList list, const int value) {
    if (list == NULL) return;

    if (list->head == NULL) {
        list->head = new_Node(value);
        list->tail = list->head;
    } else {
        list->tail->next = new_Node(value);
        list->tail = list->tail->next;
    }

    list->size++;
}

/**
 * Prepend an element to the specified list.
 *
 * This operation is O(1).
 */
void LinkedList_prepend(LinkedList list, const int value) {
    if (list == NULL) return;

    if (list->head == NULL) {
        list->head = new_Node(value);
        list->tail = list->head;
    } else {
        struct Node* const newNode = new_Node(value);
        newNode->next = list->head;
        list->head = newNode;
    }

    list->size++;
}

/**
 * Get the value at a given index in a list.
 *
 * Note that this operation is O(n), where n is the number of items in the list.
 */
int LinkedList_get(LinkedList list, const int index) {
    if (list == NULL) return INT_MIN;
    if (index >= list->size || index < 0) return INT_MIN;

    Node node = list->head;
    int iter = 0;

    while (iter < index) {
        node = node->next;
        iter++;
    }

    return node->value;
}

/**
 * Get the current head Node of the list.
 */
Node LinkedList_head(LinkedList list) {
    return list->head;
}

/**
 * Get the current tail node of the list.
 */
Node LinkedList_tail(LinkedList list) {
    return list->tail;
}

