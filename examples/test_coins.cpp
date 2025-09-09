/*
 * test_coins.cpp
 */


#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "democle.h"
#include "predicates.h"

using namespace std;

// fifty(X) monete da 50 cent
// twenty(X) monete da 20 cent
// ten(X) montete da 10 cent
belief(fifty);
belief(twenty);
belief(ten);

procedure(change);

class Coins : public Agent {
public:
    Coins() : Agent("coins") {};
    void run() {

        var(M);
        var(C);

        change(M) / (fifty(C) & gt(C, 0) & gt(M, 0) & geq(M, 0.5)) >> [C,M](Context & c) {
            int coin = c[C];
            float amount = c[M];
            cout << "50 cent coin erogated" << endl;
            c - fifty(coin);
            coin = coin - 1;
            c + fifty(coin);
            amount = amount - .5;
            c << change(amount);
        };

        change(M) / (twenty(C) & gt(C, 0) & gt(M, 0) & geq(M, 0.2)) >> [C,M](Context & c) {
            int coin = c[C];
            float amount = c[M];
            cout << "20 cent coin erogated" << endl;
            c - twenty(coin);
            coin = coin - 1;
            c + twenty(coin);
            amount = amount - .2;
            c << change(amount);
        };

        change(M) / (ten(C) & gt(C, 0) & gt(M, 0) & geq(M, 0.1)) >> [C,M](Context & c) {
            int coin = c[C];
            float amount = c[M];
            cout << "10 cent coin erogated" << endl;
            c - ten(coin);
            coin = coin - 1;
            c + ten(coin);
            amount = amount - .1;
            c << change(amount);
        };

        change(M) >> [M](Context & c) {
            float amount = c[M];
            cout << "final amount: " << amount << endl;
        };

        this + fifty(10);
        this + twenty(5);
        this + ten(20);

    };
};

int main(int argc , char **argv)
{
    Coins a;

    a.start();

    a << change(2.0);

    sleep(2);
    a.show_kb();

    return 0;
}
