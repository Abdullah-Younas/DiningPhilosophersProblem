//Problem
#include <iostream>
#include <string>
#include <random>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <mutex>

using namespace std;

const int NUM_PHILOSOPHERS = 5;

string philosophers[NUM_PHILOSOPHERS] = {"Aristotle", "Plato", "Socrates", "Descartes", "Kant"};

mutex forks[NUM_PHILOSOPHERS]; // One mutex per fork

// get random time in milliseconds
int getRandomTime(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

// Get left fork index
int leftFork(int i) {
    return i;
}

// Get right fork index
int rightFork(int i) {
    return (i + 1) % NUM_PHILOSOPHERS;
}

// Pick up forks - THIS CAUSES DEADLOCK!
void pickupForks(int i) {
    int left = leftFork(i);
    int right = rightFork(i);
    
    cout << philosophers[i] << " is trying to pick up forks..." << endl;
    
    // Step 1: Pick up LEFT fork
    forks[left].lock();
    cout << philosophers[i] << " picked up LEFT fork " << left << endl;
    
    // Small delay - this makes ALL philosophers pick up left fork first!
    this_thread::sleep_for(chrono::milliseconds(50));
    
    // Step 2: Try to pick up RIGHT fork
    // DEADLOCK HAPPENS HERE! Everyone waiting for right fork!
    cout << philosophers[i] << " is waiting for RIGHT fork " << right << "..." << endl;
    forks[right].lock();
    
    cout << philosophers[i] << " picked up RIGHT fork " << right << endl;
}

// Put down forks
void putdownForks(int i) {
    int left = leftFork(i);
    int right = rightFork(i);
    
    forks[left].unlock();
    forks[right].unlock();
    
    cout << philosophers[i] << " put down both forks" << endl;
}

// simulate thinking
void think(int philosopherId) {
    int thinkTime = getRandomTime(500, 1000);
    cout << philosophers[philosopherId] << " is thinking for " 
         << thinkTime << "ms" << endl;
    this_thread::sleep_for(chrono::milliseconds(thinkTime));
}

// simulate eating
void eat(int philosopherId) {
    int eatTime = getRandomTime(500, 1000);
    cout << philosophers[philosopherId] << " is eating for " 
         << eatTime << "ms" << endl;
    this_thread::sleep_for(chrono::milliseconds(eatTime));
}

// Philosopher lifecycle
void philosopherLife(int id) {
    for (int i = 0; i < 3; i++) { // Each philosopher tries to eat 3 times
        think(id);
        
        pickupForks(id);  // DEADLOCK WILL HAPPEN HERE!
        
        eat(id);
        
        putdownForks(id);
    }
    
    cout << philosophers[id] << " is done eating!" << endl;
}

int main() {
    srand(time(0));
    
    cout << "== DEADLOCK VERSION (BAD SOLUTION) ==" << endl;
    
    cout << "Number of philosophers: " << NUM_PHILOSOPHERS << endl;
    cout << "Number of forks: " << NUM_PHILOSOPHERS << endl << endl;
    
    cout << "Philosophers sitting at the table:" << endl;
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        cout << "Position " << i << ": " << philosophers[i] 
             << " (left fork: " << i 
             << ", right fork: " << (i + 1) % NUM_PHILOSOPHERS << ")" << endl;
    }
    cout << endl;
    
    cout << "== Starting simulation ==" << endl << endl;
    
    thread philosopherThreads[NUM_PHILOSOPHERS];
    
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopherThreads[i] = thread(philosopherLife, i);
    }
    
    // Wait for all philosophers to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopherThreads[i].join();
    }
    
    cout << endl << "== All philosophers are done ==" << endl;
    
    return 0;
}