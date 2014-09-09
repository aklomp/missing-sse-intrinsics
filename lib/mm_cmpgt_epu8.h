static inline __m128i
_mm_cmpgt_epu8 (__m128i x, __m128i y)
{
	// Returns 0xFF where x > y:
	return _mm_andnot_si128(
		_mm_cmpeq_epi8(x, y),
		_mm_cmpeq_epi8(_mm_max_epu8(x, y), x)
	);
}
