/* Bailey Ferris
 * CSCI 503
 * Lab 4
 *
 * Part 2: Concurrent Elevator Simulation - Improved version
 *
 * header file
 *
 *
 */

#ifndef IMPRVD_H
#define IMPRVD_H

struct person {
    int id; /* 0, 1, 2, 3, … */
    int from_floor, to_floor; /* i.e., from where to where */
    int assigned;
};

struct elevator {
    int id; /* 0, 1, 2, 3, … */
    int current_floor; /* Current location of the elevator */
    int dest;
    int next_stop;
    pthread_mutex_t lock;
    struct Node *people;/* All the people currently inside the elevator */
    int direction; /* 0 = none; -1 = down; 1 = up */
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
    int num_waiting;
    pthread_mutex_t *lock;
    pthread_cond_t cv; /* Will be used to block the elevator if there is no request */
    struct Node* waiting_up;
    struct Node* waiting_down;
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
//          and adding them to appropriate waitlists. increments num_started
void* Producer(void* v);

// Input: void* v, variables passed in by pthread_create
// Effects: picks up and drops off people, functions like a real elevator
//          w/ multiple people picked up / dropped off
void* Elevator(void* v);

// input: elevator e, global variables gv
// output: prints information about the elevators movements, and pickups
// effects: send elevator to a floor with a waiting person, and/or picks up people
int idle_assign(struct elevator* e, GV gv);

// input: elevator e, global variables gv
// output: prints info about people dropped off
// effects: removes people from e.people, increments num_finished
int drop_people(struct elevator* e, GV gv);

/* LINKED LIST IMPLEMENTATIONS */

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

// search for nodes(people) to pickup
// searches the waiting up or down linked lists (passed in to head) for people
// waiting on the current floor
struct Node* searchNodepickup(struct Node* head, int floor);

// search for nodes(people) to drop off
// searches the people linked list (passed in to head) for people
// with to_floor = the current floor
struct Node* searchNodedrop(struct Node* head, int floor);

#endif // IMPRVD_H