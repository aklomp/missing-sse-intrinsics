static inline __m128i
_mm_bswap_epi32 (__m128i x)
{
	// Reverse order of bytes in each 32-bit word.

#ifdef __SSSE3__
	return _mm_shuffle_epi8(x,
		_mm_set_epi8(
			12, 13, 14, 15,
			 8,  9, 10, 11,
			 4,  5,  6,  7,
			 0,  1,  2,  3));
#else
	// First swap bytes in each 16-bit word:
	__m128i a = _mm_or_si128(
		_mm_slli_epi16(x, 8),
		_mm_srli_epi16(x, 8));

	// Then swap all 16-bit words:
	a = _mm_shufflelo_epi16(a, _MM_SHUFFLE(2, 3, 0, 1));
	a = _mm_shufflehi_epi16(a, _MM_SHUFFLE(2, 3, 0, 1));

	return a;
#endif
}
