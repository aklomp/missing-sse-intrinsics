static inline __m128i
_mm_not_si128 (__m128i x)
{
	// Returns ~x, the bitwise complement of x:
	return _mm_xor_si128(x, _mm_cmpeq_epi32(_mm_setzero_si128(), _mm_setzero_si128()));
}
