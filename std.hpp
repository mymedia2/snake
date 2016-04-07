#ifndef STD_HPP
#define STD_HPP

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <deque>
#include <exception>
#include <iomanip>
#include <iosfwd>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <thread>
#include <valarray>

#if __cplusplus < 201402L

namespace std {

template <class T, class ... Args>
inline unique_ptr<T> make_unique(Args&& ... args);

}	// namespace std

template <class T, class ... Args>
inline std::unique_ptr<T> std::make_unique(Args&& ... args)
{
	return unique_ptr<T>(new T(args ...));
}

#endif	// __cplusplus

#endif	// STD_HPP
