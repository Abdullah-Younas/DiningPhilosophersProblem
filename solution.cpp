//Solution

#include <iostream>
#include <string>
#include <random>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

using namespace std;

const int NUM_PHILOSOPHERS = 5;

string philosophers[NUM_PHILOSOPHERS] = {"Aristotle", "Plato", "Socrates", "Descartes", "Kant"};

enum State { THINKING, HUNGRY, EATING };

// get random time in milliseconds
int getRandomTime(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

// MONITOR-BASED SOLUTION
class DiningPhilosophersMonitor {
private:
    State state[NUM_PHILOSOPHERS];
    mutex mtx;
    condition_variable cv[NUM_PHILOSOPHERS];
    
    // Get left neigbor index
    int left(int i) {
        return (i + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS;
    }
    
    // Get rigt neighbor index
    int right(int i) {
        return (i + 1) % NUM_PHILOSOPHERS;
    }
    
    // Test if philosopher i can eat
    void test(int i) {
        if (state[i] == HUNGRY && 
            state[left(i)] != EATING && 
            state[right(i)] != EATING) {
            
            state[i] = EATING;
            cv[i].notify_one(); // Wake up philsopher i
        }
    }
    
public:
    DiningPhilosophersMonitor() {
        for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
            state[i] = THINKING;
        }
    }
    
    // Philospher i picks up forks
    void pickupForks(int i) {
        unique_lock<mutex> lock(mtx);
        
        state[i] = HUNGRY;
        cout << philosophers[i] << " is hungry and trying to pick up forks..." << endl;
        
        test(i); // Try to pick both forks
        
        // If can't eat, wait until neighbors finish eating
        while (state[i] != EATING) {
            cout << philosophers[i] << " can't eat right now..." << endl;
            cv[i].wait(lock);
        }
        
        cout << philosophers[i] << " picked up forks " << i << " and " 
             << (i + 1) % NUM_PHILOSOPHERS << endl;
    }
    
    // Philosopher i puts down forks
    void putdownForks(int i) {
        unique_lock<mutex> lock(mtx);
        
        state[i] = THINKING;
        cout << philosophers[i] << " put down forks " << i << " and " 
             << (i + 1) % NUM_PHILOSOPHERS << endl;
        
        // Test if left and right neighbors can now eat
        test(left(i));
        test(right(i));
    }
};

// Global monitor
DiningPhilosophersMonitor monitor;

//simulate thinking
void think(int philosopherId) {
    int thinkTime = getRandomTime(1000, 3000); // 1-3 seconds
    cout << philosophers[philosopherId] << " is thinking for " 
         << thinkTime << "ms" << endl;
    this_thread::sleep_for(chrono::milliseconds(thinkTime));
}

//simulate eating
void eat(int philosopherId) {
    int eatTime = getRandomTime(1000, 2000); // 1-2 seconds
    cout << philosophers[philosopherId] << " is eating for " 
         << eatTime << "ms" << endl;
    this_thread::sleep_for(chrono::milliseconds(eatTime));
}

// Philosopher lifecycle
void philosopherLife(int id) {
    for (int i = 0; i < 3; i++) { // Each philosopher eats 3 times
        think(id);
        
        monitor.pickupForks(id);
        
        eat(id);
        
        monitor.putdownForks(id);
    }
    
    cout << philosophers[id] << " is done eating!" << endl;
}

int main() {
    srand(time(0));
    
    cout << "== MONITOR-BASED SOLUTION ==" << endl;
    
    cout << "Number of philosophers: " << NUM_PHILOSOPHERS << endl;
    cout << "Number of forks: " << NUM_PHILOSOPHERS << endl << endl;
    
    cout << "Philosophers sitting at the table:" << endl;
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        cout << "Position " << i << ": " << philosophers[i] 
             << " (left fork: " << i 
             << ", right fork: " << (i + 1) % NUM_PHILOSOPHERS << ")" << endl;
    }
    cout << endl;
    
    cout << "=== Starting simulation ===" << endl << endl;
    
    thread philosopherThreads[NUM_PHILOSOPHERS];
    
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopherThreads[i] = thread(philosopherLife, i);
    }
    
    // Wait for all philosophers to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopherThreads[i].join();
    }
    
    cout << endl << "== All philosophers are done! No deadlock! ==" << endl;
    
    return 0;
}