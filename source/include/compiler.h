#ifndef __Compiler_H
#define __Compiler_H

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif

#if defined(_MSC_VER)

#if defined(_CPPUNWIND)
#define COMPILER_EXCEPTIONS 1
#endif
#if defined(_CPPRTTI)
#define COMPILER_RTTI 1
#endif

#define PREFAST_SUPPRESS_WARNING(W) __pragma(warning(suppress: W))

#define DEPRECATED __declspec(deprecated)
#define ALIGN(bytes) __declspec(align(bytes))

#define RESTRICT_REFERENCE
#define PRINTF_PARAMS(...)
#define SCANF_PARAMS(...)

#define IF(condition, hint) if (condition)
#define IF_UNLIKELY(condition) if (condition)
#define IF_LIKELY(condition) if (condition)

#define NO_INLINE __declspec(noinline)
#define NO_INLINE_WEAK __declspec(noinline) inline
#define FORCE_INLINE __forceinline

#define __PACKED

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)

#elif defined(__GNUC__)

#if defined(__EXCEPTIONS)
#define COMPILER_EXCEPTIONS 1
#endif
#if defined(__GXX_RTTI)
#define COMPILER_RTTI 1
#endif

#define __FUNC__ __PRETTY_FUNCTION__
#define FUNC_HAS_SIGNATURE 1

#define PREFAST_SUPPRESS_WARNING(W)

#define DEPRECATED __attribute__ ((deprecated))
#define ALIGN(bytes) __attribute__ ((aligned(bytes)))
#define RESTRICT_REFERENCE __restrict__
#define PRINTF_PARAMS(...) __attribute__ ((format(printf, __VA_ARGS__)))
#define SCANF_PARAMS(...) __attribute__ ((format(scanf, __VA_ARGS__)))

#define IF(condition, hint) if (__builtin_expect(!!(condition), hint))
#define IF_UNLIKELY(condition) if (__builtin_expect(!!(condition), 0))
#define IF_LIKELY(condition) if (__builtin_expect(!!(condition), 1))

#define NO_INLINE __attribute__ ((noinline))
#define NO_INLINE_WEAK __attribute__  ((noinline)) __attribute__((weak))
#define FORCE_INLINE __attribute__ ((always_inline)) inline

#define __PACKED __attribute__ ((packed))

#define DLL_EXPORT __attribute__ ((visibility("default")))
#define DLL_IMPORT __attribute__ ((visibility("default")))

#elif defined(__clang__)

#if __has_feature(cxx_exceptions)
#define COMPILER_EXCEPTIONS 1
#endif
#if __has_feature(cxx_rtti)
#define COMPILER_RTTI 1
#endif

#define __FUNC__ __PRETTY_FUNCTION__
#define FUNC_HAS_SIGNATURE 1

#define PREFAST_SUPPRESS_WARNING(W)

#define DEPRECATED __attribute__ ((deprecated))
#define ALIGN(bytes) __attribute__ ((aligned(bytes)))
#define RESTRICT_REFERENCE __restrict__
#define PRINTF_PARAMS(...) __attribute__ ((format(printf, __VA_ARGS__)))
#define SCANF_PARAMS(...)  __attribute__ ((format(scanf, __VA_ARGS__)))

#define IF(condition, hint) if (__builtin_expect(!!(condition), hint))
#define IF_UNLIKELY(condition) if (__builtin_expect(!!(condition), 0))
#define IF_LIKELY(condition) if (__builtin_expect(!!(condition), 1))

#define NO_INLINE __attribute__ ((noinline))
#define NO_INLINE_WEAK __attribute__ ((noinline)) __attribute__((weak))
#define FORCE_INLINE __attribute__ ((always_inline)) inline

#define __PACKED __attribute__ ((packed))

#define DLL_EXPORT __attribute__ ((visibility("default")))
#define DLL_IMPORT __attribute__ ((visibility("default")))

#else

#define __FUNC__ __FUNCTION__
#define FUNC_HAS_SIGNATURE 0

#define PREFAST_SUPPRESS_WARNING(W)

#define DEPRECATED
#define ALIGN(bytes)

#define RESTRICT_REFERENCE
#define PRINTF_PARAMS(...)
#define SCANF_PARAMS(...)

#define IF(condition, hint)
#define IF_UNLIKELY(condition)
#define IF_LIKELY(condition)

#define NO_INLINE
#define NO_INLINE_WEAK
#define FORCE_INLINE

#define __PACKED

#define DLL_EXPORT
#define DLL_IMPORT

#define nullptr 0

#endif

#endif // !__Compiler_H