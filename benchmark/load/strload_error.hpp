#pragma once

#include <exception>
#include <string>

namespace enigma {

class strload_error
: public std::exception {

    std::string wht;

public:
    explicit strload_error( const std::string& what_arg )
    : wht( what_arg ) {}
    explicit strload_error( const char* what_arg )
    : wht ( what_arg ) {}

    virtual ~strload_error() = default;

    virtual const char* what() const noexcept {
        return wht.c_str();
    }
};

}
