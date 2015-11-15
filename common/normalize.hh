#ifndef NORMALIZE_HH
#define NORMALIZE_HH

#include <algorithm>

template<typename T>
T normalize(T v,T minv,T maxv)
	{
	if( v < std::min(minv,maxv)) return std::min(minv,maxv);
	if( v > std::max(minv,maxv)) return std::max(minv,maxv);
	return v;
	}

#endif

