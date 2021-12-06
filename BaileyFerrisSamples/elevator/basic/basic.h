/* Bailey Ferris
 * CSCI 503
 * Lab 4
 *
 * Part 1: Concurrent Elevator Simulation - Basic version
 *
 * header file
 *
 *
 */

#ifndef BASIC_H
#define BASIC_H

struct person {
    int id; /* 0, 1, 2, 3, … */
    int from_floor, to_floor; /* i.e., from where to where */
};

struct elevator {
    int id; /* 0, 1, 2, 3, … */
    int current_floor; /* Current location of the elevator */
    pthread_mutex_t lock;
    struct person *people;/* All the people currently inside the elevator */
};

typedef struct gv_t { /* All the global information related to the elevator simulation */
    int num_elevators;
    int num_floors;
    int time_interval;
    int beginning_time;
    int elevator_speed;
    int simulation_time;
    int random_seed;
    int num_people_started; /* statistics */
    int num_people_finished; /* statistics */
    pthread_mutex_t *lock;
    pthread_cond_t cv; /* Will be used to block the elevator if there is no request */
    struct Node* waiting;
}* GV;

typedef struct lv_t { // local variables (to pass gvs to threads)
    int id;
    GV  gv;
}* LV;

struct Node { 
    struct person data; 
    struct Node* next; // Pointer to next node in DLL 
    struct Node* prev; // Pointer to previous node in DLL 
};

// Input: void* v, variables passed in by pthread_create
// Output: prints info about people arriving at floors
// Effects: produces a person, assigning them a from_floor and a to_floor
//          and adding them to waitlist. increments num_started
void* Producer(void* v);

// Input: void* v, variables passed in by pthread_create
// Effects: picks up and drops off people, one at a time
void* Elevator(void* v);

/* LINKED LIST FUNCTION DEFINITIONS */

// adds new element to the end of the linked list pointed to by head_ref
void push(struct Node** head_ref, struct person new_data);

// deletes specific node del from linked list (but does not free the memory)
void deleteNode(struct Node** head_ref, struct Node* del);

// removes the last node from the linked list (but does not free the memory)
void removeLastNode(struct Node** head_ref, struct Node* head);

// This function returns size of linked list (number of nodes)
int findSize(struct Node *node);

// returns true if there are no nodes in the list, otherwise false
int isEmpty(struct Node** head_ref);

// returns the last node in the linked list (does not remove the element)
struct Node* getLastNode(struct Node* head);

#endif // BASIC_H
