// map.h

#ifndef __map_H
#define __map_H 1

#include <stdio.h>
#include <map>

template < class Kty,
	class Ty,
	class Pr = std::less<Kty>,
	class Alloc = std::allocator< std::pair<const Kty, Ty> > >
using map = std::map<Kty, Ty, Pr, Alloc>;
//typedef std::map<Kty, Ty, Pr, Alloc> map;

#endif // !__map_H
