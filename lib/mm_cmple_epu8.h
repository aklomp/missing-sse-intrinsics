// Return 0xFF where x <= y, else 0x00.
static inline __m128i
_mm_cmple_epu8 (__m128i x, __m128i y)
{
	return _mm_cmpeq_epi8(_mm_min_epu8(x, y), x);
}
