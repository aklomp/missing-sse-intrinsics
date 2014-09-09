// Probably too big to want to inline:
static __m128i
_mm_scale_epu8 (__m128i x, __m128i y)
{
	// Returns an "alpha blend" of x scaled by y/255;
	//   x := x * (y / 255)
	// Reorder: x := (x * y) / 255

	// Unpack x and y into 16-bit uints:
	__m128i xlo = _mm_unpacklo_epi8(x, _mm_setzero_si128());
	__m128i ylo = _mm_unpacklo_epi8(y, _mm_setzero_si128());
	__m128i xhi = _mm_unpackhi_epi8(x, _mm_setzero_si128());
	__m128i yhi = _mm_unpackhi_epi8(y, _mm_setzero_si128());

	// Multiply x with y, keeping the low 16 bits:
	xlo = _mm_mullo_epi16(xlo, ylo);
	xhi = _mm_mullo_epi16(xhi, yhi);

	// Divide by 255:
	xlo = _mm_div255_epu16(xlo);
	xhi = _mm_div255_epu16(xhi);

	// Repack the 16-bit uints to clamped 8-bit values:
	return _mm_packus_epi16(xlo, xhi);
}
