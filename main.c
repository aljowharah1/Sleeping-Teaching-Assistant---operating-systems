#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM_SEATS 3
#define SLEEP_MAX 5
#define OFFICE_HOURS_DURATION 1// Office hours duration in minutes

// Semaphores and mutex
sem_t ta_sleep;      // For waking up the TA
sem_t student_ready; // Signals that a student is ready to be helped
sem_t ta_ready;      // Signals that TA is ready to help
pthread_mutex_t chair_mutex;

// Shared data protected by mutex
int waiting_students = 0;
int chairs[NUM_SEATS] = {0};
int next_seat = 0;
int next_teach = 0;
int ta_sleeping = 0;  // Flag to track if TA is sleeping
int num_students_remaining; // Track remaining students who need help
int office_hours_active = 1; // Flag to indicate if office hours are active

void rand_sleep(void) {
    sleep(rand() % SLEEP_MAX + 1);
}

void print_waiting_students() {
    printf("Waiting students: [1] %d [2] %d [3] %d\n", chairs[0], chairs[1], chairs[2]);
}

void* student_activity(void* arg) {
    int id = *(int*)arg;
    
    while (1) {
        if (!office_hours_active) {
            printf("[Student %d] Office hours are over, I can't get help anymore.\n", id);
            break; // Exit the loop if office hours are over
        }

        printf("[Student %d] Programming...\n", id);
        rand_sleep(); // Simulate programming
        
        printf("[Student %d] Needs help, going to TA's office\n", id);
        
        pthread_mutex_lock(&chair_mutex);
        if (waiting_students < NUM_SEATS) {
            // Take a seat in the hallway
            chairs[next_seat] = id;
            waiting_students++;
            num_students_remaining++; // Increment the number of students remaining to be helped
            printf("[Student %d] Took seat %d.\n", id, next_seat);
            print_waiting_students();
            
            int my_seat = next_seat;
            next_seat = (next_seat + 1) % NUM_SEATS;

            // If TA is sleeping, wake them up
            if (ta_sleeping) {
                printf("[Student %d] Waking up the TA!\n", id);
                ta_sleeping = 0;
                sem_post(&ta_sleep); // Wake up the TA
            }
            
            pthread_mutex_unlock(&chair_mutex);
            
            // Wait until TA is ready to help
            sem_wait(&ta_ready);
            
            printf("[Student %d] Getting help from TA\n", id);
            sem_post(&student_ready);
            
            // Simulate getting help
            rand_sleep();
            printf("[Student %d] Finished getting help\n", id);
            
        } else {
            printf("[Student %d] No seats available, will try again later\n", id);
            pthread_mutex_unlock(&chair_mutex);
            rand_sleep(); // Wait before trying again
            continue;
        }
        
        // Simulate time between visits
        rand_sleep();
    }
    return NULL;
}

void* ta_activity(void* arg) {
    while (1) {
        pthread_mutex_lock(&chair_mutex);
        
        // Check if office hours have ended
        if (!office_hours_active && waiting_students == 0) {
            printf("[TA] Office hours are over. No more students to help.\n");
            pthread_mutex_unlock(&chair_mutex);
            break; // Exit if office hours are over and there are no students left
        }

        if (waiting_students == 0) {
            ta_sleeping = 1;  // Set TA as sleeping
            printf("\n[TA] All chairs empty, TA is falling asleep...\n");
            print_waiting_students();
            pthread_mutex_unlock(&chair_mutex);
            sem_wait(&ta_sleep); // Sleep until a student arrives
        } else {
            int student_id = chairs[next_teach];
            chairs[next_teach] = 0;
            waiting_students--;
            num_students_remaining--; // One student has been helped
            
            printf("[TA] Helping student %d.\n", student_id);
            print_waiting_students();
            
            next_teach = (next_teach + 1) % NUM_SEATS;
            pthread_mutex_unlock(&chair_mutex);
            
            // Signal ready to help and wait for student to be ready
            sem_post(&ta_ready);
            sem_wait(&student_ready);
            
            // Simulate teaching
            rand_sleep();
            
            // After helping, check if all chairs are now empty
            pthread_mutex_lock(&chair_mutex);
            if (waiting_students == 0) {
                printf("\n[TA] All chairs empty, TA is falling asleep...\n");
                print_waiting_students();
            }
            pthread_mutex_unlock(&chair_mutex);
        }
    }
    return NULL;
}

void* office_hours_activity(void* arg) {
    // Simulate office hours duration using sleep
    printf("[Office Hours] Office hours are starting. TA will help students for %d minutes.\n", OFFICE_HOURS_DURATION);
    
    // Sleep for the office hours duration (in seconds)
    sleep(OFFICE_HOURS_DURATION * 60);
    
    office_hours_active = 0; // End office hours after the time period is over
    printf("[Office Hours] Office hours are over. No more students will be accepted.\n");
    
    return NULL;
}

int main(int argc, char **argv) {
    int num_students;
    printf("Enter number of students: ");
    scanf("%d", &num_students);
    
    if (num_students <= 0) {
        printf("no students need help!\n");
        return 1;
    }
    
    // Initialize random seed
    srand(time(NULL));
    
    // Initialize semaphores and mutex
    sem_init(&ta_sleep, 0, 0);
    sem_init(&student_ready, 0, 0);
    sem_init(&ta_ready, 0, 0);
    pthread_mutex_init(&chair_mutex, NULL);
    
    // Set the number of remaining students
    num_students_remaining = num_students;
    
    // Create threads
    pthread_t ta_thread;
    pthread_t office_hours_thread;
    pthread_t* student_threads = malloc(sizeof(pthread_t) * num_students);
    int* student_ids = malloc(sizeof(int) * num_students);
    
    // Create TA thread
    pthread_create(&ta_thread, NULL, ta_activity, NULL);
    
    // Create office hours thread (tracks the office hours duration)
    pthread_create(&office_hours_thread, NULL, office_hours_activity, NULL);
    
    // Create student threads
    for (int i = 0; i < num_students; i++) {
        student_ids[i] = i + 1;
        pthread_create(&student_threads[i], NULL, student_activity, &student_ids[i]);
    }
    
    // Wait for office hours thread to finish (i.e., wait until office hours end)
    pthread_join(office_hours_thread, NULL);
    
    // Wait for TA thread to finish
    pthread_join(ta_thread, NULL);
    
    // Cleanup
    free(student_threads);
    free(student_ids);
    pthread_mutex_destroy(&chair_mutex);
    sem_destroy(&ta_sleep);
    sem_destroy(&student_ready);
    sem_destroy(&ta_ready);
    
    return 0;
}
