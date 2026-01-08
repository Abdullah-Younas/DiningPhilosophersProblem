#include <iostream>
#include <string>
#include <random>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(){
    srand(time(0)); // Seed random number generator
    int age;
    string philosophers[5] = {"Tom", "Max", "Jerry", "Cillian", "Zoro"};
    int forks[5] = {1, 2, 3, 4, 5};

    for (string philosophers : philosophers){
        cout << philosophers << endl;
    };

    cout << "How many forks are there?" << endl;
    for (int fork : forks){
        cout << "Fork " << fork << endl;
    };

    cout << "Let's choose a random fork: Fork: " << (rand() % 5) + 1 << endl;

    return 0;
}