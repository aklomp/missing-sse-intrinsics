// Probably too big to want to inline:
static __m128i
_mm_divfast_epu8 (__m128i x, uint8_t d)
{
	// Find shift factor:
	// This is actually much faster than using __builtin_clz():
	uint8_t n
		= (d >= 128) ? 15
		: (d >= 64) ? 14
		: (d >= 32) ? 13
		: (d >= 16) ? 12
		: (d >= 8) ? 11
		: (d >= 4) ? 10
		: (d >= 2) ? 9
		: 8;

	// Set 8 words of "inverse sensitivity":
	// Multiplying by this amount and right-shifting will give a
	// very good approximation of the result:
	__m128i inv = _mm_set1_epi16((1 << n) / d + 1);

	// Unpack input into two 16-bit uints:
	__m128i lo = _mm_unpacklo_epi8(x, _mm_setzero_si128());
	__m128i hi = _mm_unpackhi_epi8(x, _mm_setzero_si128());

	// Multiply with the "inverse sensitivity" and divide:
	lo = _mm_srli_epi16(_mm_mullo_epi16(lo, inv), n);
	hi = _mm_srli_epi16(_mm_mullo_epi16(hi, inv), n);

	// Repack:
	return _mm_packus_epi16(lo, hi);
}
