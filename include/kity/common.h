#ifndef KITY_COMMON_H_
#define KITY_COMMON_H_ 1

#ifdef __cplusplus
#define KITY_BEGIN_DECLS extern "C" {
#define KITY_END_DECLS }
#else
#define KITY_BEGIN_DECLS
#define KITY_END_DECLS
#endif /* __cplusplus */

#define KITY_STRING(x) #x

#define KITY_ASSERT_SIZE(s1, s2) \
_Static_assert(sizeof(s1) <= sizeof(s2), "Size mismatch between " KITY_STRING(s1) " and "KITY_STRING(s2)".")

#define KITY_ASSERT_ALIGNMENT(s1, s2) \
_Static_assert(_Alignof(s1) == _Alignof(s2), "Alignment mismatch between " KITY_STRING(s1) " and "KITY_STRING(s2)".")

#define KITY_ASSERT_SIZE_N_ALIGNMENT(s1, s2) \
KITY_ASSERT_SIZE(s1, s2); KITY_ASSERT_ALIGNMENT(s1, s2)

#endif /* KITY_COMMON_H_ */
