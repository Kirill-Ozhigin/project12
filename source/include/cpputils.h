// cpputils.h
// C++ Utilities

#ifndef __cppUtilities_H
#define __cppUtilities_H 1



// Shortcut to |const_cast<T&>()| that automatically derives the correct
// type (class) for the const_cast template's argument from its own argument.
// Can be used to temporarily cancel the |const| modifier on the left-hand side
// of assignment expressions, like this:
// @code
//      const Typename That;
//      ...
//      unconst(That) = SomeValue;
// @endcode
template <typename T>
inline T& unconst(const T& that)
{
	return const_cast<T&>(that);
}


// Shortcut to |const_cast<T*>()| that automatically derives the correct
// type (class) for the const_cast template's argument from its own argument.
// Can be used to temporarily cancel the |const| modifier on the left-hand side
// of assignment expressions, like this:
// @code
//      const Typename* pThat;
//      ...
//      unconst(pThat) = SomeValue;
// @endcode
template <typename T>
inline T* unconst(const T* that)
{
	return const_cast<T*>(that);
}



#endif // !__cpputils_H
