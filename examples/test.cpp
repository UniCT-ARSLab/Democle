/*
 * test.cpp
 */


#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "democle.h"

using namespace std;

belief(coin);
belief(amount);
belief(amount2);
goal(coin50);
procedure(p1);
procedure(p2);


class MyAgent : public Agent {
public:
    MyAgent() : Agent("myagent") { };
    void run() {

        var(X);
        var(Y);

        coin50() << (coin(50) & amount(50));

        p1() >> [](Context & c) {
            cout << "p1 in" << endl;
            c << p2();
            cout << "p1 out" << endl;
        };

        p2() >> [](Context & c) {
            cout << "p2" << endl;
        };

        +coin(X) / (amount(X) & amount2(X)) >> [X](Context & c)
        {
            int x = c[X];
            cout << "hello1 coin + amount " << x << endl;
            cout << "hello2 coin + amount " << x << endl;
            //c + coin(50, "one");
        };

        +coin(X) >> [X](Context & c) {
            int x = c[X];
            cout << "Other plan " << x << endl;
        };

        +coin(X, Y) >> [X,Y](Context & c)
        {
            int x = c[X];
            string y = c[Y];
            cout << "hello[2] " << x << "," << y << endl;
        };


    };
};

int main(int argc , char **argv)
{
    MyAgent e;

    e.show_plans();
    e.start();

    // e + amount(30);
    // e + amount(40);
    // e + amount2(30);

    // e + coin(30);
    // e + coin(40);
    // e + coin();

    e << p1();

    sleep(1);

    //e.show_kb();
    // kb.assert_belief(c1);
    // kb.assert_belief(c2);
    // kb.assert_belief(c3);
    // kb.assert_belief(c4);
    // kb.show();

    return 0;
}
