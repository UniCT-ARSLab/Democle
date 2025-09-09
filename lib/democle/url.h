/*
 * url.h
 */

#ifndef __URL_HH
#define __URL_HH

#include <string>

struct url {
    url(const std::string& url_s) {
      is_good = this->parse(url_s);
    };
    std::string protocol, host, port, path, query;
    bool is_good;
private:
    bool parse(const std::string& url_s);
};

#endif

