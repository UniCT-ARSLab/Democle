/*
 * ping_pong.cpp
 */


#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string.h>

#include "democle.h"
#include "predicates.h"

using namespace std;

reactor(ping);
reactor(pong);
procedure(go);

class Receiver : public Agent {
public:
    Receiver() : Agent("receiver") {};
    void run() {

        accepts(ping/1);

        var(X);

        +ping(X) >> [X] (Context & c)
        {
            int x = c[X];
            cout << "Message received from " << c.sender() << "," << x << endl;
            x++;
            c + (c.sender(), pong(x));

        };

    };
};

class Sender : public Agent {
public:
    Sender() : Agent("sender") {};
    void run() {

        accepts(pong/1);

        var(X);

        go() >> [](Context & c)
        {
            //c.show_kb();
            cout << "Sending Message" << endl;
            c + ("tcp://localhost:4321/receiver", ping(1));
        };

        +pong(X) >> [X] (Context & c)
        {
            int x = c[X];
            cout << "Message received from " << c.sender() << "," << x << endl;
            sleep(1);
            c + ("tcp://localhost:4321/receiver", ping(x+1));
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

        Sender s_agent;

        s_agent.start();

        s_agent << go();

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
