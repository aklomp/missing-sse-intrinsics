static inline __m128i
_mm_cmpge_epu16 (__m128i x, __m128i y)
{
	// Returns 0xFFFF where x >= y:
	return _mm_cmple_epu16(y, x);
}
