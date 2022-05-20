static inline __m128i
_mm_setone_epi8 (void)
{
	// Returns a vector where every byte has the value 1:
	__m128i x = _mm_cmpeq_epi32(_mm_setzero_si128(), _mm_setzero_si128());
	return _mm_xor_si128(_mm_add_epi8(x, x), x);
}
