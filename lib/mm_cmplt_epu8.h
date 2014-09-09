static inline __m128i
_mm_cmplt_epu8 (__m128i x, __m128i y)
{
	// Returns 0xFF where x < y:
	return _mm_cmpgt_epu8(y, x);
}
