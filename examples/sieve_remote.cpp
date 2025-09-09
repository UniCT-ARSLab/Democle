/*
 * sieve.cpp
 */


#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string.h>

#include "democle.h"
#include "predicates.h"

using namespace std;

belief(number);
reactor(test);
procedure(sieve);
procedure(show_primes);
procedure(go);

class Receiver : public Agent {
public:
    Receiver() : Agent("receiver") {};
    void run() {

        accepts(test/0);

        go() >> [](Context & c)
        {
            c + ("receiver", test());
        };

        +test() >> [] (Context & c)
        {
            cout << "Message received from " << c.sender() << endl;
        };

    };
};

class SieveAgent : public Agent {
public:
    SieveAgent() : Agent("sieve") {};
    void run() {

        var(X);
        var(Y);

        sieve() / (number(X) & number(Y) & neq(X,Y) & multiple(X, Y)) >> [X](Context & c)
        {
            int x = c[X];
            c - number(x);
            c << sieve();
        };

        sieve() >> [](Context & c)
        {
            //c.show_kb();
            c + ("tcp://localhost:4321/receiver", test());
            c << show_primes();
        };

        show_primes() / number(X) >> [X](Context & c)
        {
            int x = c[X];
            cout << "Prime number " << x << endl;
            c - number(x);
            c << show_primes();
        };

        show_primes() >> [](Context & c)
        {
            cout << "end" << endl;
            c.show_kb();
        };

    };
};

int main(int argc , char **argv)
{

    if (argc == 1) {
        fprintf(stderr, "usage: %s --sender|--receiver\n", argv[0]);
    }

    if (!strcmp(argv[1],"--sender")) {

        DEMOCLE::register_protocol("tcp", "localhost", 4322);

        SieveAgent s_agent;

        s_agent.start();

        //s_agent.show_plans();

        for (int i = 99; i >= 2;i--) {
            s_agent + number(i);
        }
        s_agent << sieve();

    }
    else {

        DEMOCLE::register_protocol("tcp", "localhost", 4321);

        Receiver r_agent;
        r_agent.start();

        //r_agent << go();
    }

    sleep(60);

    return 0;
}
