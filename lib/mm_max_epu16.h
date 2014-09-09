#ifndef __SSE4_1__
static inline __m128i
_mm_max_epu16 (__m128i x, __m128i y)
{
	// Returns x where x >= y, else y:
	return _mm_blendv_si128(x, y, _mm_cmple_epu16(x, y));
}
#endif
