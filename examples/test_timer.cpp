/*
 * test_timer.cpp
 */


#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "democle.h"

using namespace std;

reactor(tick);
belief(state);

class TimerTest : public Agent {
public:
    TimerTest() : Agent("timer_test") {};
    void run() {

        var(X);
        var(T);
        Timer * t = new Timer(250, "tim1");
        attach(t);

        +tick(T) / state(X)  >> [X,T](Context & c)
        {
            string t = c[T];
            int x = c[X];
            cout << "Tick from timer " << t << ":" << x << endl;
            c - state(x);
            x = !x;
            c + state(x);
        };

    };
};


int main(int argc , char **argv)
{
    TimerTest a;

    a + state(0);
    a.start();
    a.show_plans();

    sleep(20);

    return 0;
}
