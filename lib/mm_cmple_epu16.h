static inline __m128i
_mm_cmple_epu16 (__m128i x, __m128i y)
{
	// Returns 0xFFFF where x <= y:
	return _mm_cmpeq_epi16(_mm_subs_epu16(x, y), _mm_setzero_si128());
}
