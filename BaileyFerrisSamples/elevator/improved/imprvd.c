/* Bailey Ferris
 * CSCI 503
 * Lab 4
 *
 * Part 2: Concurrent Elevator Simulation - Improved version
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
#include "imprvd.h"

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
    gv->waiting_up = NULL;
    gv->waiting_down = NULL;
    gv->num_waiting = 0;
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

    printf("\nSimulation result: %d people have started, %d people have finished during %d seconds\n", 
            gv->num_people_started, gv->num_people_finished, gv->simulation_time);

    free(gv);
    free(lvs);
    free(attrs);
    free(threads);

    return 0;
}

void* Producer(void* v) {
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
        p.assigned = 0;

        p.from_floor = rand() % gv->num_floors + 1;
        p.to_floor = rand() % gv->num_floors + 1;

        while(p.to_floor == p.from_floor){
            p.to_floor = rand() % gv->num_floors + 1;
        }

        pthread_mutex_lock(gv->lock);
        
        printf("\n[%d] Person %d arrives on floor %d, waiting to go to floor %d", (int)time(NULL) - gv->beginning_time, p.id, p.from_floor, p.to_floor);
        gv->num_people_started++;

        if(gv->num_waiting == 0){
            pthread_cond_signal(&gv->cv);
        }
        if(p.from_floor < p.to_floor){
            push(&gv->waiting_up, p);
        }else{
            push(&gv->waiting_down, p);
        }
        gv->num_waiting++;
        pthread_mutex_unlock(gv->lock);
    }
    return NULL;
}

void* Elevator(void* v){
    GV  gv;
    LV  lv;

    lv = (LV) v;
    gv = lv->gv;

    struct elevator e;
    e.id = lv->id;
    e.current_floor = 1;
    e.people = NULL;
    e.direction = 0;
    e.dest = 1;
    e.next_stop = 1;

    while (1) {

        if((int)time(NULL) - gv->beginning_time >= gv->simulation_time){
            return NULL;
        }

        if(!isEmpty(&gv->waiting_up) || !isEmpty(&gv->waiting_down)){

            pthread_mutex_lock(gv->lock);

            // block thread if no one is waiting to be picked up
            while(gv->num_waiting == 0){
                pthread_cond_wait(&gv->cv, gv->lock);
            }


            // if the elevator is currently empty/idle, assign the most recent request as its destination
            if(isEmpty(&e.people)){

                int ret = idle_assign(&e, gv);

                if(ret){ // if all requests have been assigned, start loop over and try again
                    pthread_mutex_unlock(gv->lock);
                    continue;
                }
            }

            pthread_mutex_unlock(gv->lock);

            // stop at each floor w/ requests or people who need to be dropped 
            // there on the way to that floor & pick up everyone there going in the same direction
            if(e.direction == 1){
                while(!isEmpty(&e.people) || e.current_floor < e.dest){
                    sleep(gv->elevator_speed);
                    e.current_floor++;

                    if(e.current_floor > gv->num_floors){
                        e.current_floor--;
                        continue;
                    }

                    pthread_mutex_lock(gv->lock);

                    struct Node* drop = searchNodedrop(e.people, e.current_floor);
                    struct Node* pickup = searchNodepickup(gv->waiting_up, e.current_floor);
                    if(drop != NULL){
                        printf("\n[%d] Elevator %d arrives at floor %d", (int)time(NULL) - gv->beginning_time, e.id, e.current_floor);
                    }else if(pickup != NULL){
                        printf("\n[%d] Elevator %d arrives at floor %d", (int)time(NULL) - gv->beginning_time, e.id, e.current_floor);
                    }

                    // search e.people for people to drop at that floor --> drop
                    int stopped = 0;
                    stopped = drop_people(&e, gv);


                    // search waiting_up for people on that floor --> pickup
                    // pickup people going up
                    if(pickup != NULL){
                        stopped = 1;
                        push(&e.people, pickup->data);
                        deleteNode(&gv->waiting_up, pickup);
                        gv->num_waiting--;
                        printf("\n[%d] Elevator %d picks up Person %d", (int)time(NULL) - gv->beginning_time, e.id, pickup->data.id);
                    }
                    pickup = searchNodepickup(gv->waiting_up, e.current_floor);  
                    while(pickup != NULL){
                        printf(", %d", pickup->data.id);
                        push(&e.people, pickup->data);
                        deleteNode(&gv->waiting_up, pickup);
                        gv->num_waiting--;
                        pickup = searchNodepickup(gv->waiting_up, e.current_floor);  
                    }

                    // update destination and next stop, if picked up or dropped on this floor
                    if(stopped){
                        struct Node* check = getLastNode(e.people);
                        if(check != NULL){
                            e.dest = check->data.to_floor;
                            e.next_stop = check->data.to_floor;
                            check = check->prev;
                        }
                        while(check != NULL){
                            if(e.dest < check->data.to_floor){
                                e.dest = check->data.to_floor;
                            }
                            if(e.next_stop > check->data.to_floor){
                                e.next_stop = check->data.to_floor;
                            }
                            check = check->prev;
                        }
                    }

                    // if picked up new person(s), announce next floor headed to
                    if(stopped && e.current_floor != e.next_stop){
                        printf("\n[%d] Elevator %d starts moving from %d to %d", (int)time(NULL) - gv->beginning_time, e.id, e.current_floor, e.next_stop);
                    }

                    pthread_mutex_unlock(gv->lock);

                    if(isEmpty(&e.people) && e.current_floor == e.dest){
                        e.direction = 0; // if emptied the elevator and doesn't have a request, make elevator idle
                    }
                }

                if(isEmpty(&e.people) && e.current_floor == e.dest){
                    int stopped = 0;

                    //search floor for people going in other direction
                    pthread_mutex_lock(gv->lock);
                    struct Node* pickup = searchNodepickup(gv->waiting_down, e.current_floor);
                    if(pickup != NULL){
                        if(e.direction != 0){
                            // if elevator was not already sitting idle at the floor, announce arrival
                            printf("\n[%d] Elevator %d arrives at floor %d", (int)time(NULL) - gv->beginning_time, e.id, e.current_floor);
                        }                        
                        stopped = 1;
                        push(&e.people, pickup->data);
                        deleteNode(&gv->waiting_down, pickup);
                        gv->num_waiting--;
                        printf("\n[%d] Elevator %d picks up Person %d", (int)time(NULL) - gv->beginning_time, e.id, pickup->data.id);
                    }
                    pickup = searchNodepickup(gv->waiting_down, e.current_floor); 
                    while(pickup != NULL){
                        printf(", %d", pickup->data.id);
                        push(&e.people, pickup->data);
                        deleteNode(&gv->waiting_down, pickup);
                        gv->num_waiting--;
                        pickup = searchNodepickup(gv->waiting_down, e.current_floor);  
                    }

                    // update destination and next stop, if picked up or dropped on this floor
                    if(stopped){
                        struct Node* check = getLastNode(e.people);
                        if(check != NULL){
                            e.direction = -1;
                            e.dest = check->data.to_floor;
                            e.next_stop = check->data.to_floor;
                            check = check->prev;
                        }
                        while(check != NULL){
                            if(e.dest > check->data.to_floor){
                                e.dest = check->data.to_floor;
                            }
                            if(e.next_stop < check->data.to_floor){
                                e.next_stop = check->data.to_floor;
                            }
                            check = check->prev;
                        }
                    }

                    // if picked up/dropped person(s), announce next floor headed to
                    if(stopped && e.current_floor != e.next_stop){
                        printf("\n[%d] Elevator %d starts moving from %d to %d", (int)time(NULL) - gv->beginning_time, e.id, e.current_floor, e.next_stop);
                    }

                    pthread_mutex_unlock(gv->lock);
                }
            }

            if(e.direction == -1){
                while(!isEmpty(&e.people) || e.current_floor > e.dest){
                    sleep(gv->elevator_speed);
                    e.current_floor--;

                    if(e.current_floor < 1){
                        e.current_floor++;
                        continue;
                    }

                    pthread_mutex_lock(gv->lock);

                    int stopped = 0;
                    struct Node* drop = searchNodedrop(e.people, e.current_floor);
                    struct Node* pickup = searchNodepickup(gv->waiting_down, e.current_floor);
                    if(drop != NULL){
                        printf("\n[%d] Elevator %d arrives at floor %d", (int)time(NULL) - gv->beginning_time, e.id, e.current_floor);
                    }else if(pickup != NULL){
                        printf("\n[%d] Elevator %d arrives at floor %d", (int)time(NULL) - gv->beginning_time, e.id, e.current_floor);
                    }

                    stopped = drop_people(&e, gv);

                    // pick up people going down
                    if(pickup != NULL){
                        push(&e.people, pickup->data);
                        deleteNode(&gv->waiting_down, pickup);
                        gv->num_waiting--;
                        printf("\n[%d] Elevator %d picks up Person %d", (int)time(NULL) - gv->beginning_time, e.id, pickup->data.id);
                        stopped = 1;
                    }
                    pickup = searchNodepickup(gv->waiting_down, e.current_floor);
                    while(pickup != NULL){
                        printf(", %d", pickup->data.id);
                        push(&e.people, pickup->data);
                        deleteNode(&gv->waiting_down, pickup);
                        gv->num_waiting--;
                        pickup = searchNodepickup(gv->waiting_down, e.current_floor);
                    }

                    // update destination and next stop, if picked up or dropped on this floor
                    if(stopped){
                        struct Node* check = getLastNode(e.people);
                        if(check != NULL){
                            e.dest = check->data.to_floor;
                            e.next_stop = check->data.to_floor;
                            check = check->prev;
                        }
                        while(check != NULL){
                            if(e.dest > check->data.to_floor){
                                e.dest = check->data.to_floor;
                            }
                            if(e.next_stop < check->data.to_floor){
                                e.next_stop = check->data.to_floor;
                            }
                            check = check->prev;   
                        }
                    }

                    // if picked up/dropped person(s), announce next floor headed to
                    if(stopped && e.current_floor != e.next_stop){
                        printf("\n[%d] Elevator %d starts moving from %d to %d", (int)time(NULL) - gv->beginning_time, e.id, e.current_floor, e.next_stop);
                    }

                    pthread_mutex_unlock(gv->lock);

                    // if drops all people and doesn't have a specific floor to head to, make idle
                    if(isEmpty(&e.people) && e.current_floor == e.dest){
                        e.direction = 0;
                    }
                }

                // if the elevator is empty, possibly pick people up & change direction
                if(isEmpty(&e.people) && e.current_floor == e.dest){
                    int stopped = 0;
                    //search floor for people going in other direction
                    pthread_mutex_lock(gv->lock);
                    struct Node* pickup = searchNodepickup(gv->waiting_up, e.current_floor);
                    if(pickup != NULL){
                        // if isn't sitting idle at a floor, announce arrival
                        if(e.direction != 0){
                            printf("\n[%d] Elevator %d arrives at floor %d", (int)time(NULL) - gv->beginning_time, e.id, e.current_floor);
                        }
                        stopped = 1;
                        push(&e.people, pickup->data);
                        deleteNode(&gv->waiting_up, pickup);
                        gv->num_waiting--;
                        printf("\n[%d] Elevator %d picks up Person %d", (int)time(NULL) - gv->beginning_time, e.id, pickup->data.id);
                    }
                    pickup = searchNodepickup(gv->waiting_up, e.current_floor); 
                    while(pickup != NULL){
                        printf(", %d", pickup->data.id);
                        push(&e.people, pickup->data);
                        deleteNode(&gv->waiting_up, pickup);
                        gv->num_waiting--;
                        pickup = searchNodepickup(gv->waiting_up, e.current_floor);  
                    }

                    // update destination and next stop, if picked up or dropped on this floor
                    if(stopped){
                        struct Node* check = getLastNode(e.people);
                        if(check != NULL){
                            e.direction = 1; // set to opposite direction (up, was going down before)
                            e.dest = check->data.to_floor;
                            e.next_stop = check->data.to_floor;
                            check = check->prev;
                        }
                        while(check != NULL){
                            // dest is the highest floor the current passengers are headed to
                            if(e.dest < check->data.to_floor){
                                e.dest = check->data.to_floor;
                            }
                            if(e.next_stop > check->data.to_floor){
                                e.next_stop = check->data.to_floor;
                            }
                            check = check->prev;
                        }
                    }

                    // if picked up/dropped person(s), announce next floor headed to
                    if(stopped && e.current_floor != e.next_stop){
                        printf("\n[%d] Elevator %d starts moving from %d to %d", (int)time(NULL) - gv->beginning_time, e.id, e.current_floor, e.next_stop);
                    }

                    pthread_mutex_unlock(gv->lock);
                }
            }
        }
    }
    return NULL;
}

int idle_assign(struct elevator* e, GV gv){
    struct Node* next_up = getLastNode(gv->waiting_up);
    struct Node* next_down = getLastNode(gv->waiting_down);
    while(next_up != NULL && next_up->data.assigned == 1){
        next_up = next_up->prev;
    }
    while(next_down != NULL && next_down->data.assigned == 1){
        next_down = next_down->prev;
    }

    if(next_up == NULL && next_down == NULL){
        return 1;
    }else if(next_down != NULL && next_up == NULL){
        next_down->data.assigned = 1;
        e->dest = next_down->data.from_floor;
        e->next_stop = next_down->data.from_floor;

        if(next_down->data.from_floor > e->current_floor){
            e->direction = 1;
        }else if(next_down->data.from_floor < e->current_floor){
            e->direction = -1;
        }else{ // the person is on the current floor, so pick them up (and anyone else on the floor going down)
            struct Node* pickup = searchNodepickup(gv->waiting_down, e->current_floor);
            if(pickup != NULL){
                printf("\n[%d] Elevator %d arrives at floor %d", (int)time(NULL) - gv->beginning_time, e->id, e->current_floor);
                if(e->next_stop < pickup->data.to_floor){
                    e->next_stop = pickup->data.to_floor;
                }
                if(e->dest > pickup->data.to_floor){
                    e->dest = pickup->data.to_floor;
                }
                push(&e->people, pickup->data);
                deleteNode(&gv->waiting_down, pickup);
                gv->num_waiting--;
                printf("\n[%d] Elevator %d picks up Person %d", (int)time(NULL) - gv->beginning_time, e->id, pickup->data.id);
            }
            pickup = searchNodepickup(gv->waiting_down, e->current_floor);
            while(pickup != NULL){
                if(e->next_stop < pickup->data.to_floor){
                    e->next_stop = pickup->data.to_floor;
                }
                if(e->dest > pickup->data.to_floor){
                    e->dest = pickup->data.to_floor;
                }
                printf(", %d", pickup->data.id);
                push(&e->people, pickup->data);
                deleteNode(&gv->waiting_down, pickup);
                gv->num_waiting--;
                pickup = searchNodepickup(gv->waiting_down, e->current_floor);
            }
            e->direction = -1;
        }
    }else if(next_up != NULL && next_down == NULL){
        next_up->data.assigned = 1;
        e->dest = next_up->data.from_floor;
        e->next_stop = next_up->data.from_floor;

        if(next_up->data.from_floor > e->current_floor){
            e->direction = 1;
        }else if(next_up->data.from_floor < e->current_floor){
            e->direction = -1;
        }else{ // the person is on the current floor, so pick them up (and anyone else on the floor going up)
            struct Node* pickup = searchNodepickup(gv->waiting_up, e->current_floor);
            if(pickup != NULL){
                printf("\n[%d] Elevator %d arrives at floor %d", (int)time(NULL) - gv->beginning_time, e->id, e->current_floor);
                if(e->next_stop > pickup->data.to_floor){
                    e->next_stop = pickup->data.to_floor;
                }
                if(e->dest < pickup->data.to_floor){
                    e->dest = pickup->data.to_floor;
                }
                push(&e->people, pickup->data);
                deleteNode(&gv->waiting_up, pickup);
                gv->num_waiting--;
                printf("\n[%d] Elevator %d picks up Person %d", (int)time(NULL) - gv->beginning_time, e->id, pickup->data.id);
            }
            pickup = searchNodepickup(gv->waiting_up, e->current_floor);
            while(pickup != NULL){
                if(e->next_stop > pickup->data.to_floor){
                    e->next_stop = pickup->data.to_floor;
                }
                if(e->dest < pickup->data.to_floor){
                    e->dest = pickup->data.to_floor;
                }
                printf(", %d", pickup->data.id);
                push(&e->people, pickup->data);
                deleteNode(&gv->waiting_up, pickup);
                gv->num_waiting--;
                pickup = searchNodepickup(gv->waiting_up, e->current_floor);
            }
            e->direction = 1;
        }
    }else if(next_up->data.id < next_down->data.id){
        next_up->data.assigned = 1;
        e->dest = next_up->data.from_floor;
        e->next_stop = next_up->data.from_floor;

        if(next_up->data.from_floor > e->current_floor){
            e->direction = 1;
        }else if(next_up->data.from_floor < e->current_floor){
            e->direction = -1;
        }else{
            struct Node* pickup = searchNodepickup(gv->waiting_up, e->current_floor);
            if(pickup != NULL){
                printf("\n[%d] Elevator %d arrives at floor %d", (int)time(NULL) - gv->beginning_time, e->id, e->current_floor);
                if(e->next_stop > pickup->data.to_floor){
                    e->next_stop = pickup->data.to_floor;
                }
                if(e->dest < pickup->data.to_floor){
                    e->dest = pickup->data.to_floor;
                }
                push(&e->people, pickup->data);
                deleteNode(&gv->waiting_up, pickup);
                gv->num_waiting--;
                printf("\n[%d] Elevator %d picks up Person %d", (int)time(NULL) - gv->beginning_time, e->id, pickup->data.id);
            }
            pickup = searchNodepickup(gv->waiting_up, e->current_floor);
            while(pickup != NULL){
                if(e->next_stop > pickup->data.to_floor){
                    e->next_stop = pickup->data.to_floor;
                }
                if(e->dest < pickup->data.to_floor){
                    e->dest = pickup->data.to_floor;
                }
                printf(", %d", pickup->data.id);
                push(&e->people, pickup->data);
                deleteNode(&gv->waiting_up, pickup);
                gv->num_waiting--;
                pickup = searchNodepickup(gv->waiting_up, e->current_floor);  
            }
            e->direction = 1;
        }
    }else if(next_up->data.id > next_down->data.id){
        next_down->data.assigned = 1;
        e->dest = next_down->data.from_floor;
        e->next_stop = next_down->data.from_floor;

        if(next_down->data.from_floor > e->current_floor){
            e->direction = 1;
        }else if(next_down->data.from_floor < e->current_floor){
            e->direction = -1;
        }else{
            struct Node* pickup = searchNodepickup(gv->waiting_down, e->current_floor);
            if(pickup != NULL){
                printf("\n[%d] Elevator %d arrives at floor %d", (int)time(NULL) - gv->beginning_time, e->id, e->current_floor);
                if(e->next_stop < pickup->data.to_floor){
                    e->next_stop = pickup->data.to_floor;
                }
                if(e->dest > pickup->data.to_floor){
                    e->dest = pickup->data.to_floor;
                }
                push(&e->people, pickup->data);
                deleteNode(&gv->waiting_down, pickup);
                gv->num_waiting--;
                printf("\n[%d] Elevator %d picks up Person %d", (int)time(NULL) - gv->beginning_time, e->id, pickup->data.id);
            }
            pickup = searchNodepickup(gv->waiting_down, e->current_floor);
            while(pickup != NULL){
                if(e->next_stop < pickup->data.to_floor){
                    e->next_stop = pickup->data.to_floor;
                }
                if(e->dest > pickup->data.to_floor){
                    e->dest = pickup->data.to_floor;
                }
                printf(", %d", pickup->data.id);
                push(&e->people, pickup->data);
                deleteNode(&gv->waiting_down, pickup);
                gv->num_waiting--;
                pickup = searchNodepickup(gv->waiting_down, e->current_floor);
            }
            e->direction = -1;
        }
    }

    if(e->current_floor != e->next_stop){
        printf("\n[%d] Elevator %d starts moving from %d to %d", (int)time(NULL) - gv->beginning_time, e->id, e->current_floor, e->next_stop);
    }
    return 0;
}

int drop_people(struct elevator* e, GV gv){
    // drop people
    int stopped = 0;
    struct Node* drop = searchNodedrop(e->people, e->current_floor);
    if(drop != NULL){
        stopped = 1;
        printf("\n[%d] Elevator %d drops Person %d", (int)time(NULL) - gv->beginning_time, e->id, drop->data.id);
        gv->num_people_finished++;
        deleteNode(&e->people, drop);
        free(drop);
    }
    drop = searchNodedrop(e->people, e->current_floor);
    while(drop != NULL){
        printf(", %d", drop->data.id);
        gv->num_people_finished++;
        deleteNode(&e->people, drop);
        free(drop);
        drop = searchNodedrop(e->people, e->current_floor);
    }
    return stopped;
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

struct Node* searchNodepickup(struct Node* head, int floor){
    if (head == NULL){
        return NULL; 
    }

    if(head->data.from_floor == floor){
        return head;
    }

    struct Node* search = head;
    while(search != NULL){
        if(search->data.from_floor == floor){
            return search;
        }
        search = search->next;
    }
    return NULL;
}

struct Node* searchNodedrop(struct Node* head, int floor){
    if (head == NULL){
        return NULL; 
    }

    if(head->data.to_floor == floor){
        return head;
    }

    struct Node* search = head;
    while(search != NULL){
        if(search->data.to_floor == floor){
            return search;
        }
        search = search->next;
    }
    return NULL;
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
    if(*head_ref == NULL){
        return 1;
    }else{
        return 0;
    }
}