/*
 * url.cpp
 */

#include "url.h"

#include <string>
#include <algorithm>
#include <cctype>
#include <functional>

using namespace std;

bool url::parse(const string& url_s)
{
    protocol = "";
    host = "";
    port = "";
    path = "";
    query = "";
    const string prot_end("://");
    string::const_iterator prot_i = search(url_s.begin(), url_s.end(),
                                           prot_end.begin(), prot_end.end());
    if( prot_i == url_s.end() )
        return false;
    protocol.reserve(distance(url_s.begin(), prot_i));
    transform(url_s.begin(), prot_i,
              back_inserter(protocol),
              ptr_fun<int,int>(tolower)); // protocol is icase
    advance(prot_i, prot_end.length());
    string::const_iterator port_i = find(prot_i, url_s.end(), ':');
    if ( port_i == url_s.end() ) {
        string::const_iterator path_i = find(prot_i, url_s.end(), '/');
        host.reserve(distance(prot_i, path_i));
        transform(prot_i, path_i,
                  back_inserter(host),
                  ptr_fun<int,int>(tolower)); // host is icase
        advance(path_i, 1);
        path.assign(path_i, url_s.end());
    }
    else {
        host.reserve(distance(prot_i, port_i));
        transform(prot_i, port_i,
                  back_inserter(host),
                  ptr_fun<int,int>(tolower)); // host is icase
        advance(port_i, 1);
        string::const_iterator path_i = find(port_i, url_s.end(), '/');
        port.assign(port_i, path_i);
        advance(path_i, 1);
        path.assign(path_i, url_s.end());
    }
    return true;
}
