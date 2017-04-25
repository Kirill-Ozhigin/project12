// vector.h

#ifndef __vector_H
#define __vector_H 1

#include <vector>
#include <stdio.h>


template < class Ty, 
	class Alloc = std::allocator<Ty> >
using vector = std::vector<Ty, Alloc>;
//typedef std::vector<Ty, Alloc> vector;

#endif // !__vector_H
