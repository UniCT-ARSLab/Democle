/*
 * test_max.cpp
 */


#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "democle.h"
#include "predicates.h"

using namespace std;

// student(X): X e' uno studente
// graduated(X): X' un laureato

belief(student);
belief(graduated);

class TestStudent : public Agent {
public:
    TestStudent() : Agent("test_student") {};
    void run() {

        var(X);

        +graduated(X) / student(X) >> [X](Context & c) {
            string nome = (string)c[X];
            cout << "Evviva!!" << nome << " si e' laureato" << endl;
            // cancellare lo studente
            c - student(X);
        };

        +graduated(X) >> [X](Context & c) {
            string nome = (string)c[X];
            cout << nome << " non e' uno studente" << endl;
            // cancellare il laureato
            c - graduated(X);
        };

    };
};

int main(int argc , char **argv)
{
    TestStudent a;

    a.start();
    a + student("miriana");
    a + student("alex");
    //a.show_plans();

    a + graduated("alex");
    a + graduated("corrado");

    sleep(2);
    a.show_kb();

    return 0;
}
