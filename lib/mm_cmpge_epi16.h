static inline __m128i
_mm_cmpge_epi16 (__m128i x, __m128i y)
{
	// Returns 0xFFFF where x >= y:
	return _mm_or_si128(_mm_cmpeq_epi16(x, y), _mm_cmpgt_epi16(x, y));
}
