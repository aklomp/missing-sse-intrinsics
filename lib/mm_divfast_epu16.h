static __m128i
_mm_divfast_epu16 (__m128i x, uint16_t d)
{
	uint8_t n
		= (d >= 32768) ? 31
		: (d >= 16384) ? 30
		: (d >= 8192) ? 29
		: (d >= 4096) ? 28
		: (d >= 2048) ? 27
		: (d >= 1024) ? 26
		: (d >= 512) ? 25
		: (d >= 256) ? 24
		: (d >= 128) ? 23
		: (d >= 64) ? 22
		: (d >= 32) ? 21
		: (d >= 16) ? 20
		: (d >= 8) ? 19
		: (d >= 4) ? 18
		: (d >= 2) ? 17
		: 16;

	__m128i inv = _mm_set1_epi32((1 << n) / d + 1);

	// Unpack input into two 32-bit uints:
	__m128i xlo = _mm_unpacklo_epi16(x, _mm_setzero_si128());
	__m128i xhi = _mm_unpackhi_epi16(x, _mm_setzero_si128());

	// Multiply xlo[0] and xlo[2] with the "inverse sensitivity" and divide:
	xlo = _mm_srli_epi32(_mm_mullo_epu32(xlo, inv), n);
	xhi = _mm_srli_epi32(_mm_mullo_epu32(xhi, inv), n);

	// Create a ghetto 16-bit two's complement:
	__m128i xa = _mm_add_epi32(_mm_set1_epi32(-65536), xlo);
	__m128i xb = _mm_add_epi32(_mm_set1_epi32(-65536), xhi);

	// Splice these in where x > 32767:
	xa = _mm_blendv_si128(xlo, xa, _mm_cmpgt_epi32(xlo, _mm_set1_epi32(32767)));
	xb = _mm_blendv_si128(xhi, xb, _mm_cmpgt_epi32(xhi, _mm_set1_epi32(32767)));

	// Repack using the regular saturated pack:
	return _mm_packs_epi32(xa, xb);
}
