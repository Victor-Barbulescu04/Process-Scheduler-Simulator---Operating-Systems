/**
* @file linkedList.h
 * @brief Declares the functions and attributes of a linked list of Member structs
 *
 * Course: CSC3210
 * Section: 003
 * Assignment: Process Simulator
 * Name: Victor Barbulescu
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "PCB.h"
#include <stdbool.h>


/**
* @struct ll_node
* @brief Defines nodes within the linkedList
*/
typedef struct ll_node{
    PCB_t data;
    struct ll_node* next;
}LL_Node;

extern int size;

/**
* @brief Creates a new node with some initialized data
* @param data: the PCB data
* @return a new node initialized with data
*/
LL_Node* create_LL_Node(PCB_t data);

/**
* @brief Creates a new node with some initialized data
* @param head: head of the linkedList
* @param data: the PCB data
* @return a new node initialized with data
*/
void append_to_linked_list(LL_Node** head, PCB_t data);

/**
* @brief removes a node based on the provided ID
* @param head pointer to the head of the list
* @param id: PID to remove
*/
void remove_by_id(LL_Node** head, int id);

/**
* @brief prints the contents of the list
* @param head pointer to the head of the list
*/
void print_list(LL_Node* head);

/**
* @brief destroys the list and clears its memory
* @param head pointer to the head of the list
*/
void free_list(LL_Node* head);

#endif //LINKEDLIST_H
