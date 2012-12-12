#ifndef UTIL_H
#define UTIL_H

#include	<sstream>

template<typename T>
T StringTo(const std::string& string){
    T valor;

    std::stringstream stream(string);
    stream >> valor;
    if (stream.fail()) {
        std::runtime_error e(string);
        throw e;
    }
    return valor;
}

#endif