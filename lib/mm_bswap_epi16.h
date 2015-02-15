static inline __m128i
_mm_bswap_epi16 (__m128i x)
{
	// Swap upper and higher byte in each 16-bit word:
	return _mm_or_si128(
		_mm_slli_epi16(x, 8),
		_mm_srli_epi16(x, 8));
}
