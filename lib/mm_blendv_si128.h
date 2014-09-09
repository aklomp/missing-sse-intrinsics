static inline __m128i
_mm_blendv_si128 (__m128i x, __m128i y, __m128i mask)
{
	// Replace bit in x with bit in y when matching bit in mask is set:
	return _mm_or_si128(_mm_andnot_si128(mask, x), _mm_and_si128(mask, y));
}
