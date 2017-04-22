#ifndef PROTOCOLEXCEPTION_HPP_
#define PROTOCOLEXCEPTION_HPP_

#include <exception>

namespace ium
{
    class ProtocolException : public std::exception
    {
    public:
        ProtocolException(std::string what) throw() : _what(what) {}
        ~ProtocolException() {}

        virtual const char  *what() const throw() { return _what.c_str(); }

    private:
        std::string _what;
    };
}

#endif