// Replace the bit in x with the bit in y when the matching bit in the mask is
// set.
static inline __m128i
_mm_blendv_si128 (__m128i x, __m128i y, __m128i mask)
{
	return _mm_or_si128(_mm_andnot_si128(mask, x), _mm_and_si128(mask, y));
}
