/**
 * @file linkedList.c
 * @brief Defines the functions and attributes of a linked list of Member structs
 *
 * Course: CSC3210
 * Section: 003
 * Assignment: Process Simulator
 * Name: Victor Barbulescu
 */

#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int size = 0;

LL_Node* create_LL_Node(PCB_t data) {
    LL_Node* newNode = (LL_Node*)malloc(sizeof(LL_Node));
    if (!newNode) {
        fprintf(stderr, "ERROR: Memory allocation failed!\n");
        exit(1);
    }

    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void append_to_linked_list(LL_Node** head, PCB_t data) {
    LL_Node* newNode = create_LL_Node(data);

    if (*head == NULL) {
        *head = newNode;
        size++;
        fflush(stdout);
        return;
    }

    LL_Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
    size++;
}

void remove_by_id(LL_Node** head, int id) {
    LL_Node* temp = *head;
    LL_Node* prev = NULL;

    if (temp != NULL && temp->data.PID == id) {
        *head = temp->next;
        free(temp);
        size--;
        return;
    }

    while (temp != NULL && temp->data.PID != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        fprintf(stderr, "ERROR: No such ID found!\n\n");
        return;
    }

    prev->next = temp->next;
    free(temp);
}

void print_process_summary(PCB_t* data) {
    printf("PID: %d, PRIORITY: %d, READY WAIT TIME: %d, I/O WAIT TIME: %d\n", data->PID, data->priority, data->total_ready_time, data->total_io_blocked_time);
}

void print_list(LL_Node* head) {
    if (head == NULL) {
        fprintf(stderr,"ERROR: List is empty!\n\n");
        return;
    }

    LL_Node* temp = head;
    while (temp != NULL) {
        print_process_summary(&temp->data);
        temp = temp->next;
    }
    printf("\n");
}

void free_list(LL_Node* head) {
    LL_Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

