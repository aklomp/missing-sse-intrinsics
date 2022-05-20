// Return 0xFFFF where x >= y, else 0x0000.
static inline __m128i
_mm_cmpge_epu16 (__m128i x, __m128i y)
{
	return _mm_cmple_epu16(y, x);
}
