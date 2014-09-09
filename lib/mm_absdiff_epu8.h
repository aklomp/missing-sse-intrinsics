static inline __m128i
_mm_absdiff_epu8 (__m128i x, __m128i y)
{
	// Calculate absolute difference: abs(x - y):
	return _mm_or_si128(_mm_subs_epu8(x, y), _mm_subs_epu8(y, x));
}
