#ifndef __SSE4_1__
static inline __m128i
_mm_blendv_epi8 (__m128i x, __m128i y, __m128i mask)
{
	// Replace byte in x with byte in y when the MSB of the corresponding
	// byte in mask is set:
	return _mm_blendv_si128(x, y, _mm_cmplt_epi8(mask, _mm_setzero_si128()));
}
#endif
