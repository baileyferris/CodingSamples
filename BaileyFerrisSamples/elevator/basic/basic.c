/* Bailey Ferris
 * CSCI 503
 * Lab 4
 *
 * Part 1: Concurrent Elevator Simulation - Basic version
 *
 * c file
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "basic.h"

int main(int argc, char *argv[]){
    int i;
    GV gv;
    LV lvs;
    gv = (GV) malloc(sizeof(*gv));


    //set global variable values
    if(argc == 7){
        gv->num_elevators = atoi(argv[1]);
        if(gv->num_elevators <= 0){
            printf("Number of elevators must be > 0\n");
            exit(1);
        }
        gv->num_floors = atoi(argv[2]);
        if(gv->num_floors <= 0){
            printf("Number of floors must be > 0\n");
            exit(1);
        }
        gv->time_interval = atoi(argv[3]);
        if(gv->time_interval <= 0){
            printf("Time interval must be > 0\n");
            exit(1);
        }
        gv->elevator_speed = atoi(argv[4]);
        if(gv->elevator_speed <= 0){
            printf("Elevator speed must be > 0\n");
            exit(1);
        }
        gv->simulation_time = atoi(argv[5]);
        if(gv->simulation_time <= 0){
            printf("Simulation time must be > 0\n");
            exit(1);
        }
        gv->random_seed = atoi(argv[6]);
    }else{
        printf("INCORRECT NUMBER OF ARGUMENTS\n");
        exit(1);
    }

    gv->num_people_started = 0;
    gv->num_people_finished = 0;
    gv->waiting = NULL;
    gv->lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));

    pthread_t *threads;
    pthread_attr_t *attrs;
   // void *retval;

    threads = (pthread_t*) malloc(sizeof(pthread_t)*(gv->num_elevators + 1));
    attrs = (pthread_attr_t*) malloc(sizeof(pthread_attr_t)*(gv->num_elevators +1));
    lvs = (LV) malloc(sizeof(*lvs)*(gv->num_elevators + 1));

    if (pthread_mutex_init(gv->lock, NULL)){ 
        perror("pthread_mutex_init()"); 
        exit(1); 
    }

    if (pthread_cond_init(&gv->cv, NULL)){ 
        perror("pthread_cond_init()"); 
        exit(1); 
    }

    //create person thread
    lvs[0].gv = gv;
    gv->beginning_time = (int)time(NULL);
    if(pthread_attr_init(attrs)) perror("attr_init()");
    if(pthread_attr_setscope(attrs, PTHREAD_SCOPE_SYSTEM)) perror("attr_setscope()");
    if(pthread_create(threads, attrs, Producer, lvs)){
        perror("pthread_create()");
        exit(1);
    }

    // Create elevator threads
    int j;
    for(j = 1; j <= gv->num_elevators; ++j){
        lvs[j].gv = gv;
        lvs[j].id = j-1;
        if(pthread_attr_init(attrs+j)) perror("attr_init()");
        if(pthread_attr_setscope(attrs+j, PTHREAD_SCOPE_SYSTEM)) perror("attr_setscope()");
        if(pthread_create(threads+j, attrs+j, Elevator, lvs+j)){
            perror("pthread_create()");
            exit(1);
        }
    }

    while((int)time(NULL) - gv->beginning_time < gv->simulation_time){
        // just wait for simulation time to be up
    }



    // cancel threads to end simulation
    for(i = 0; i < gv->num_elevators+1; i++){
        pthread_cancel(threads[i]);
    }

    printf("Simulation result: %d people have started, %d people have finished during %d seconds\n", 
            gv->num_people_started, gv->num_people_finished, gv->simulation_time);

    free(gv);
    free(lvs);
    free(attrs);
    free(threads);

    return 0;
}

void* Producer(void* v) {
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    GV  gv;
    LV  lv;

    lv = (LV) v;
    gv = lv->gv;


    srand(gv->random_seed);

    while (1) {

        sleep(gv->time_interval);

        if((int)time(NULL) - gv->beginning_time >= gv->simulation_time){
            return NULL;
        }

        //make person
        struct person p;
        
        p.id = gv->num_people_started;

        p.from_floor = rand() % gv->num_floors + 1;
        p.to_floor = rand() % gv->num_floors + 1;

        while(p.to_floor == p.from_floor){
            p.to_floor = rand() % gv->num_floors + 1;
        }

        
        printf("[%d] Person %d arrives on floor %d, waiting to go to floor %d\n", (int)time(NULL) - gv->beginning_time, p.id, p.from_floor, p.to_floor);
        gv->num_people_started++;

        pthread_mutex_lock(gv->lock);
        if(isEmpty(&gv->waiting)){
            pthread_cond_signal(&gv->cv);
        }
        push(&gv->waiting, p);
        pthread_mutex_unlock(gv->lock);
    }
    return NULL;
}

void* Elevator(void* v) {
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    GV  gv;
    LV  lv;

    lv = (LV) v;
    gv = lv->gv;

    struct elevator e;
    e.id = lv->id;
    e.current_floor = 1;
    e.people = (struct person*)malloc(sizeof(struct person));

    while (1) {

        if((int)time(NULL) - gv->beginning_time >= gv->simulation_time){
            return NULL;
        }


        if(!isEmpty(&gv->waiting)){

            pthread_mutex_lock(gv->lock);

            while(isEmpty(&gv->waiting)){
                pthread_cond_wait(&gv->cv, gv->lock);
            }

            struct Node* next_person = getLastNode(gv->waiting);
            removeLastNode(&gv->waiting, gv->waiting);

            pthread_mutex_unlock(gv->lock);

            e.people[0] = next_person->data;

            if(e.current_floor != next_person->data.from_floor){
                printf("[%d] Elevator %d starts moving from %d to %d\n", (int)time(NULL) - gv->beginning_time, e.id, e.current_floor, next_person->data.from_floor);
                if(e.current_floor > next_person->data.from_floor){
                    while(e.current_floor > next_person->data.from_floor){
                        sleep(gv->elevator_speed);
                        e.current_floor--;
                    }
                }else if(e.current_floor < next_person->data.from_floor){
                    while(e.current_floor < next_person->data.from_floor){
                        sleep(gv->elevator_speed);
                        e.current_floor++;
                    }
                }
            }

            printf("[%d] Elevator %d arrives at floor %d\n", (int)time(NULL) - gv->beginning_time, e.id, next_person->data.from_floor);
            printf("[%d] Elevator %d picks up Person %d\n", (int)time(NULL) - gv->beginning_time, e.id, next_person->data.id);
            
            printf("[%d] Elevator %d starts moving from %d to %d\n", (int)time(NULL) - gv->beginning_time, e.id, next_person->data.from_floor, next_person->data.to_floor);
            if(e.current_floor > next_person->data.to_floor){
                while(e.current_floor > next_person->data.to_floor){
                    sleep(gv->elevator_speed);
                    e.current_floor--;
                }
            }else if(e.current_floor < next_person->data.to_floor){
                while(e.current_floor < next_person->data.to_floor){
                    sleep(gv->elevator_speed);
                    e.current_floor++;
                }
            }

            printf("[%d] Elevator %d arrives at floor %d\n", (int)time(NULL) - gv->beginning_time, e.id, next_person->data.to_floor);
            printf("[%d] Elevator %d drops Person %d\n", (int)time(NULL) - gv->beginning_time, e.id, next_person->data.id);
            free(next_person);
            gv->num_people_finished++;
        }

    }
    return NULL;
}



// linked list implementations

void push(struct Node** head_ref, struct person new_data){ 
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node)); 
  
    new_node->data = new_data; 
  
    //Make next of new node as head and previous as NULL
    new_node->next = (*head_ref); 
    new_node->prev = NULL; 
  
    //change prev of head node to new node
    if ((*head_ref) != NULL) {
        (*head_ref)->prev = new_node; 
    }
  
    //move the head to point to the new node
    (*head_ref) = new_node;
}

void deleteNode(struct Node** head_ref, struct Node* del){ 
    // base case
    if (*head_ref == NULL || del == NULL) 
        return; 
  
    // If node to be deleted is head node
    if (*head_ref == del) 
        *head_ref = del->next; 
  
    // Change next only if node to be deleted is NOT the last node
    if (del->next != NULL) 
        del->next->prev = del->prev; 
  
    // Change prev only if node to be deleted is NOT the first node
    if (del->prev != NULL) 
        del->prev->next = del->next; 
  
    return; 
}

void removeLastNode(struct Node** head_ref, struct Node* head){ 
    if (head == NULL) 
        return; 
  
    if (head->next == NULL) { 
        deleteNode(head_ref, head); 
        return; 
    } 
  
    // Find the second last node 
    struct Node* second_last = head; 
    while (second_last->next->next != NULL) {
        second_last = second_last->next;
    }
  
    // Delete last node 
    deleteNode(head_ref, second_last->next); 
  
    // Change next of second last 
    second_last->next = NULL; 
  
    return; 
}

struct Node* getLastNode(struct Node* head){ 
    if (head == NULL) 
        return NULL; 
  
    if (head->next == NULL) { 
        return head; 
    } 
  
    // Find the second last node 
    struct Node* last = head; 
    while (last->next != NULL) {
        last = last->next;
    }
  
    return last; 
}

int findSize(struct Node *node) { 
   int res = 0; 
   while (node != NULL){ 
       res++; 
       node = node->next; 
   } 
   return res; 
}

int isEmpty(struct Node** head_ref){
    if (*head_ref == NULL){
        return 1;
    }else{
        return 0;
    }
}