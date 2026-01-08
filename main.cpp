#include <iostream>
#include <string>
using namespace std;

int main(){
    int age;
    string philosophers[5] = {"Tom", "Max", "Jerry", "Cillian", "Zoro"};

    for (string philosophers : philosophers){
        cout << philosophers << endl;
    };

    return 0;
}