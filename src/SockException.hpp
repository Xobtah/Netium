//
// Created by xobtah on 21/12/16.
//

#ifndef RTYPE_SOCKEXCEPTION_HPP
#define RTYPE_SOCKEXCEPTION_HPP

#include <exception>

namespace ium
{
	class SockException : public std::exception
	{
	public:
		SockException(std::string what) throw() : _what(what) {}
		~SockException() {}

		virtual const char  *what() const throw() { return _what.c_str(); }

	private:
		std::string _what;
	};
}

#endif //RTYPE_SOCKEXCEPTION_HPP
