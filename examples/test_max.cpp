/*
 * test_max.cpp
 */


#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "democle.h"
#include "predicates.h"

using namespace std;

belief(number);
procedure(calc_max);

class CalcMax : public Agent {
public:
    CalcMax() : Agent("calc_max") {};
    void run() {

        var(X);
        var(M);

        calc_max() / number(X)  >> [X](Context & c)
        {
            int x = c[X];
            cout << "Starting from " << x << endl;
            c << calc_max(X);
        };

        calc_max(M) / (number(X) & gt(X, M)) >> [X](Context & c)
        {
            int x = c[X];
            cout << "Continuing with " << x << endl;
            c << calc_max(x);
        };

        calc_max(M) >> [M](Context & c)
        {
            int m = c[M];
            cout << "Max is " << m << endl;
        };


        srand(time(NULL));
        for (int i = 0; i < 300;i++) {
            this + number((int)(rand() * 3000.0 / RAND_MAX));
        }
        show_kb();
        this << calc_max();

    };
};

int main(int argc , char **argv)
{
    CalcMax a;

    a.start();
    a.show_plans();

    sleep(10);

    return 0;
}
