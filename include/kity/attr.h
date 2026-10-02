#ifndef KITY_ATTR_H_
#define KITY_ATTR_H_ 1

#include "common.h"

KITY_BEGIN_DECLS

#if defined(_MSC_VER)
#ifdef KITY_LIB_STATIC
#define KITY_API
#elif defined(KITY_LIB_SHARED)
#define KITY_API __declspec(dllexport)
#else
#define KITY_API __declspec(dllimport)
#endif
#define KITY_INLINE static __forceinline
#else
#ifdef KITY_LIB_SHARED
#define KITY_API __attribute__((__visibility__("default")))
#else
#define KITY_API
#endif /* KITY_LIB_SHARED */
#define KITY_INLINE static inline __attribute__((__always_inline__))
#endif /* Compilers */

#if defined(__GNUC__) || defined(__clang__)
#define KITY_HOT __attribute__((__hot__))
#else
#define KITY_HOT
#endif /* Compilers */

#ifdef __cplusplus
#define KITY_RESTRICT __restrict
#else
#define KITY_RESTRICT restrict
#endif /* __cplusplus */

KITY_END_DECLS

#endif /* KITY_ATTR_H_ */
