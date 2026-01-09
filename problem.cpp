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

// Philosopher names
string philosophers[NUM_PHILOSOPHERS] = {"Rowan", "Jackie", "Matthew", "Cillian", "Tom"};

mutex forkMutexes[NUM_PHILOSOPHERS]; // One mutex per fork
mutex coutMutex; // Protect console output from race conditions

int getRandomTime(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

// Get left fork index
int leftFork(int philosopherId) {
    return philosopherId;
}

// Get right fork index  
int rightFork(int philosopherId) {
    return (philosopherId + 1) % NUM_PHILOSOPHERS;
}

void pickupForks(int id) {
    int left = leftFork(id);
    int right = rightFork(id);
    
    {
        lock_guard<mutex> lock(coutMutex);
        cout << philosophers[id] << " is hungry and reaching for fork " << left << endl;
    }
    
    forkMutexes[left].lock();
    
    {
        lock_guard<mutex> lock(coutMutex);
        cout << philosophers[id] << " picked up left fork " << left << endl;
    }
    
    // Small delay to increase chance of deadlock
    this_thread::sleep_for(chrono::milliseconds(100));
    
    {
        lock_guard<mutex> lock(coutMutex);
        cout << philosophers[id] << " is reaching for fork " << right << endl;
    }
    
    forkMutexes[right].lock();
    
    {
        lock_guard<mutex> lock(coutMutex);
        cout << philosophers[id] << " picked up right fork " << right << endl;
    }
}

void putdownForks(int id) {
    int left = leftFork(id);
    int right = rightFork(id);
    
    forkMutexes[left].unlock();
    forkMutexes[right].unlock();
    
    {
        lock_guard<mutex> lock(coutMutex);
        cout << philosophers[id] << " put down both forks" << endl;
    }
}

void philosopherLife(int id) {
    for (int i = 0; i < 3; i++) {
        int thinkTime = getRandomTime(500, 1000);
        
        {
            lock_guard<mutex> lock(coutMutex);
            cout << philosophers[id] << " is thinking for " << thinkTime << "ms" << endl;
        }
        
        this_thread::sleep_for(chrono::milliseconds(thinkTime));
        
        // Try to eat
        pickupForks(id);
        
        int eatTime = getRandomTime(500, 1000);
        
        {
            lock_guard<mutex> lock(coutMutex);
            cout << philosophers[id] << " is eating for " << eatTime << "ms" << endl;
        }
        
        this_thread::sleep_for(chrono::milliseconds(eatTime));
        
        putdownForks(id);
    }
    
    {
        lock_guard<mutex> lock(coutMutex);
        cout << philosophers[id] << " is dne eating!" << endl;
    }
}

int main() {
    srand(time(0));
    
    cout << "Number of philosophers: " << NUM_PHILOSOPHERS << endl << endl;
    
    cout << "== Starting simulation in 3 seconds ==" << endl;
    this_thread::sleep_for(chrono::seconds(3));
    cout << endl;
    
    thread philosopherThreads[NUM_PHILOSOPHERS];
    
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopherThreads[i] = thread(philosopherLife, i);
    }
    
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopherThreads[i].join();
    }
    
    cout << endl << "== All philosphers are done ==" << endl;
    
    return 0;
}