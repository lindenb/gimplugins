#ifndef MYDEBUG_HH
#define MYDEBUG_HH

#include <iostream>


#define DEBUG(a) do { std::cerr << "[" << __FILE__ << ":" << __LINE__ << "]" << a << std::endl;}while(0)

#endif

