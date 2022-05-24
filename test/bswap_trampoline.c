#if __SSSE3__
#include <tmmintrin.h>
#elif __SSE2__
#include <emmintrin.h>
#endif

// Include inline function definitions:
#include "../missing_sse.h"

// Machinery to create trampoline functions for different
// word widths and instruction sets (SSE2 and SSSE3):
#define BSWAP(a)	_BSWAP(a,FUNC_POSTFIX)
#define _BSWAP(a,b)	__BSWAP(a,b)
#define __BSWAP(a,b)				\
__m128i _mm_bswap_##a##_##b (__m128i x)		\
{						\
	return _mm_bswap_##a(x);		\
}

BSWAP(epi16)
BSWAP(epi32)
BSWAP(epi64)
BSWAP(si128)
