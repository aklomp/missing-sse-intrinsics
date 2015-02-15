static inline __m128i
_mm_bswap_si128 (__m128i x)
{
	// Reverse order of all bytes in the 128-bit word.

#ifdef __SSSE3__
	return _mm_shuffle_epi8(x,
		_mm_set_epi8(
			 0,  1,  2,  3,
			 4,  5,  6,  7,
			 8,  9, 10, 11,
			12, 13, 14, 15));
#else
	// Swap bytes in each 16-bit word:
	__m128i a = _mm_or_si128(
		_mm_slli_epi16(x, 8),
		_mm_srli_epi16(x, 8));

	// Reverse all 16-bit words in 64-bit halves:
	a = _mm_shufflelo_epi16(a, _MM_SHUFFLE(0, 1, 2, 3));
	a = _mm_shufflehi_epi16(a, _MM_SHUFFLE(0, 1, 2, 3));

	// Reverse 64-bit halves:
	return _mm_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2));
#endif
}
