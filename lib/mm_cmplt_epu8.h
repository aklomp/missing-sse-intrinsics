// Return 0xFF where x < y, else 0x00.
static inline __m128i
_mm_cmplt_epu8 (__m128i x, __m128i y)
{
	return _mm_cmpgt_epu8(y, x);
}
