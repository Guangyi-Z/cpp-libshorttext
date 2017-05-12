#ifndef _UTIL_HPP_
#define _UTIL_HPP_

#include <unistd.h> // getcwd()

std::string get_working_path() {
    const int MAXPATHLEN_ = 2048;
    char temp[MAXPATHLEN_];
    return ( getcwd(temp, MAXPATHLEN_) ? std::string( temp ) : std::string("") );
}

#endif // _UTIL_HPP_
