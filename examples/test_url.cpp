/*
 * test_url.cpp
 */


#include <iostream>
#include "url.h"

using namespace std;


int main(int argc , char **argv)
{
    url u(argv[1]);
    cout << u.protocol << '\t' << u.host << "\t" << u.port << "\t" << u.path << endl;
    return 0;
}
