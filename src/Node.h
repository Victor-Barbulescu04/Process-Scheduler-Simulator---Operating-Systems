/**
 * @file Node.h
 * @brief Declares and defines Node struct and static createNode function
 *
 * Course: CSC3210
 * Section: 003
 * Assignment: Process Simulator
 * Name: Victor Barbulescu
 */

#ifndef NODE_H
#define NODE_H

// Header imports
#include "PCB.h"
// Library imports
#include <stdio.h>
#include <stdlib.h>

/**
* @struct Node
* @brief Defines nodes within the linkedList
*/
typedef struct node{
	PCB_t* data;
	struct node* next;
}Node;

/**
* @brief Creates a new node with some initialized data
* @param data: PCB data
* @return a new node initialized with data
*/
static Node* createNode(PCB_t* data) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (!newNode) {
		fprintf(stderr, "ERROR: Memory allocation failed!\n");
		exit(1);
	}
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}

#endif //NODE_H
