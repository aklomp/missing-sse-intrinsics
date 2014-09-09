// Probably too big to want to inline:
static __m128i
_mm_div_epu8 (__m128i x, uint8_t d)
{
	// This is the same routine as above, but exact; it includes a
	// correction step where we back-substitute the quotient and correct
	// the off-by-one errors of the faster routine.
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
	__m128i xlo = _mm_unpacklo_epi8(x, _mm_setzero_si128());
	__m128i xhi = _mm_unpackhi_epi8(x, _mm_setzero_si128());

	// Multiply with the "inverse sensitivity" and divide:
	__m128i alo = _mm_srli_epi16(_mm_mullo_epi16(xlo, inv), n);
	__m128i ahi = _mm_srli_epi16(_mm_mullo_epi16(xhi, inv), n);

	// alo and ahi contain the quotients. The result is sometimes too large
	// by one, so we introduce a correction step. Decrement the quotients:
	__m128i blo = _mm_subs_epu16(alo, _mm_set1_epi16(1));
	__m128i bhi = _mm_subs_epu16(ahi, _mm_set1_epi16(1));

	// Multiply with the divisor to get something back in the neighbourhood
	// of the original numbers:
	__m128i mdd = _mm_set1_epi16(d);
	__m128i plo = _mm_mullo_epi16(blo, mdd);
	__m128i phi = _mm_mullo_epi16(bhi, mdd);

	// If (orig - new) >= d, the existing quotient is a better fit:
	// We can use native epi16 ops because these are all 8-bit numbers:
	__m128i masklo = _mm_cmpeq_epi16(_mm_min_epi16(_mm_subs_epu16(xlo, plo), mdd), mdd);
	__m128i maskhi = _mm_cmpeq_epi16(_mm_min_epi16(_mm_subs_epu16(xhi, phi), mdd), mdd);

	// Decrement the original divisors according to the inverse masks:
	alo = _mm_subs_epu16(alo, _mm_andnot_si128(masklo, _mm_set1_epi16(1)));
	ahi = _mm_subs_epu16(ahi, _mm_andnot_si128(maskhi, _mm_set1_epi16(1)));

	// Repack:
	return _mm_packus_epi16(alo, ahi);
}
