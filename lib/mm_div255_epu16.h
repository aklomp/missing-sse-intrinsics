static inline __m128i
_mm_div255_epu16 (__m128i x)
{
	// Divide 8 16-bit uints by 255:
	// x := ((x + 1) + (x >> 8)) >> 8:
	return _mm_srli_epi16(_mm_adds_epu16(
		_mm_adds_epu16(x, _mm_set1_epi16(1)),
		_mm_srli_epi16(x, 8)), 8);
}
