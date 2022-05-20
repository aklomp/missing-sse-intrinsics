// Return 0xFFFF where x <= y, else 0x0000.
static inline __m128i
_mm_cmple_epu16 (__m128i x, __m128i y)
{
	return _mm_cmpeq_epi16(_mm_subs_epu16(x, y), _mm_setzero_si128());
}
