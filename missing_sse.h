#pragma once

#include <stdint.h>

#if defined(__SSSE3__)
#include <tmmintrin.h>
#elif defined(__SSE2__)
#include <emmintrin.h>
#else
#error "No SSE support"
#endif

// Return 0xFF where x <= y, else 0x00.
static inline __m128i
_mm_cmple_epu8 (__m128i x, __m128i y)
{
	return _mm_cmpeq_epi8(_mm_min_epu8(x, y), x);
}

// Return 0xFF where x >= y, else 0x00.
static inline __m128i
_mm_cmpge_epu8 (__m128i x, __m128i y)
{
	return _mm_cmple_epu8(y, x);
}

// Return 0xFF where x > y, else 0x00.
static inline __m128i
_mm_cmpgt_epu8 (__m128i x, __m128i y)
{
	return _mm_andnot_si128(
		_mm_cmpeq_epi8(x, y),
		_mm_cmpeq_epi8(_mm_max_epu8(x, y), x)
	);
}

// Return 0xFF where x < y, else 0x00.
static inline __m128i
_mm_cmplt_epu8 (__m128i x, __m128i y)
{
	return _mm_cmpgt_epu8(y, x);
}

// Return 0xFFFF where x <= y, else 0x0000.
static inline __m128i
_mm_cmple_epu16 (__m128i x, __m128i y)
{
	return _mm_cmpeq_epi16(_mm_subs_epu16(x, y), _mm_setzero_si128());
}

// Return 0xFFFF where x >= y, else 0x0000.
static inline __m128i
_mm_cmpge_epu16 (__m128i x, __m128i y)
{
	return _mm_cmple_epu16(y, x);
}

// Return 0xFFFF where x > y, else 0x0000.
static inline __m128i
_mm_cmpgt_epu16 (__m128i x, __m128i y)
{
	return _mm_andnot_si128(_mm_cmpeq_epi16(x, y), _mm_cmple_epu16(y, x));
}

// Return 0xFFFF where x < y, else 0x0000.
static inline __m128i
_mm_cmplt_epu16 (__m128i x, __m128i y)
{
	return _mm_cmpgt_epu16(y, x);
}

// Return 0xFFFF where x >= y, else 0x0000.
static inline __m128i
_mm_cmpge_epi16 (__m128i x, __m128i y)
{
	return _mm_or_si128(_mm_cmpeq_epi16(x, y), _mm_cmpgt_epi16(x, y));
}

// Return ~x, the bitwise complement of x.
static inline __m128i
_mm_not_si128 (__m128i x)
{
	return _mm_xor_si128(x, _mm_cmpeq_epi32(_mm_setzero_si128(), _mm_setzero_si128()));
}

// Return a vector where every byte has the value 1.
static inline __m128i
_mm_setone_epi8 (void)
{
	__m128i x = _mm_cmpeq_epi32(_mm_setzero_si128(), _mm_setzero_si128());
	return _mm_xor_si128(_mm_add_epi8(x, x), x);
}

// Return a vector where every word has the value 1.
static inline __m128i
_mm_setone_epi16 (void)
{
	__m128i x = _mm_cmpeq_epi32(_mm_setzero_si128(), _mm_setzero_si128());
	return _mm_xor_si128(_mm_add_epi16(x, x), x);
}

// Replace the bit in x with the bit in y when the matching bit in the mask is
// set.
static inline __m128i
_mm_blendv_si128 (__m128i x, __m128i y, __m128i mask)
{
	return _mm_or_si128(_mm_andnot_si128(mask, x), _mm_and_si128(mask, y));
}

// Replace the byte in x with the byte in y when the MSB of the corresponding
// byte in the mask is set.
#ifndef __SSE4_1__
static inline __m128i
_mm_blendv_epi8 (__m128i x, __m128i y, __m128i mask)
{
	return _mm_blendv_si128(x, y, _mm_cmplt_epi8(mask, _mm_setzero_si128()));
}

// Return x where x <= y, else y.
static inline __m128i
_mm_min_epu16 (__m128i x, __m128i y)
{
	return _mm_sub_epi16(x, _mm_subs_epu16(x, y));
}

// Return x where x >= y, else y.
static inline __m128i
_mm_max_epu16 (__m128i x, __m128i y)
{
	return _mm_add_epi16(x, _mm_subs_epu16(y, x));
}
#endif

// Calculate the absolute difference: abs(x - y).
static inline __m128i
_mm_absdiff_epu8 (__m128i x, __m128i y)
{
	return _mm_or_si128(_mm_subs_epu8(x, y), _mm_subs_epu8(y, x));
}

// Calculate the absolute difference: abs(x - y).
static inline __m128i
_mm_absdiff_epu16 (__m128i x, __m128i y)
{
	return _mm_or_si128(_mm_subs_epu16(x, y), _mm_subs_epu16(y, x));
}

// Divide 8 16-bit uints by 255:
//   x := ((x + 1) + (x >> 8)) >> 8
static inline __m128i
_mm_div255_epu16 (__m128i x)
{
	return _mm_srli_epi16(_mm_adds_epu16(
		_mm_adds_epu16(x, _mm_set1_epi16(1)),
		_mm_srli_epi16(x, 8)), 8);
}

// Returns an "alpha blend" of x scaled by y/255;
//   x := x * (y / 255)
// Reorder: x := (x * y) / 255
static inline __m128i
_mm_scale_epu8 (__m128i x, __m128i y)
{
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

static inline __m128i
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

static inline __m128i
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

// Swap the upper and lower byte in each 16-bit word.
static inline __m128i
_mm_bswap_epi16 (__m128i x)
{
	return _mm_or_si128(
		_mm_slli_epi16(x, 8),
		_mm_srli_epi16(x, 8));
}

// Reverse the order of all bytes in each 32-bit word.
static inline __m128i
_mm_bswap_epi32 (__m128i x)
{
#ifdef __SSSE3__
	return _mm_shuffle_epi8(x,
		_mm_set_epi8(
			12, 13, 14, 15,
			 8,  9, 10, 11,
			 4,  5,  6,  7,
			 0,  1,  2,  3));
#else
	// First swap bytes in each 16-bit word.
	__m128i a = _mm_bswap_epi16(x);

	// Then swap all 16-bit words.
	a = _mm_shufflelo_epi16(a, _MM_SHUFFLE(2, 3, 0, 1));
	a = _mm_shufflehi_epi16(a, _MM_SHUFFLE(2, 3, 0, 1));

	return a;
#endif
}

// Reverse the order of all bytes in each 64-bit word.
static inline __m128i
_mm_bswap_epi64 (__m128i x)
{
#ifdef __SSSE3__
	return _mm_shuffle_epi8(x,
		_mm_set_epi8(
			 8,  9, 10, 11,
			12, 13, 14, 15,
			 0,  1,  2,  3,
			 4,  5,  6,  7));
#else
	// Swap bytes in each 16-bit word.
	__m128i a = _mm_bswap_epi16(x);

	// Reverse all 16-bit words in 64-bit halves.
	a = _mm_shufflelo_epi16(a, _MM_SHUFFLE(0, 1, 2, 3));
	a = _mm_shufflehi_epi16(a, _MM_SHUFFLE(0, 1, 2, 3));

	return a;
#endif
}

// Reverse the order of all bytes in the 128-bit word.
static inline __m128i
_mm_bswap_si128 (__m128i x)
{
#ifdef __SSSE3__
	return _mm_shuffle_epi8(x,
		_mm_set_epi8(
			 0,  1,  2,  3,
			 4,  5,  6,  7,
			 8,  9, 10, 11,
			12, 13, 14, 15));
#else
	// Swap bytes in each 16-bit word.
	__m128i a = _mm_bswap_epi16(x);

	// Reverse all 16-bit words in 64-bit halves.
	a = _mm_shufflelo_epi16(a, _MM_SHUFFLE(0, 1, 2, 3));
	a = _mm_shufflehi_epi16(a, _MM_SHUFFLE(0, 1, 2, 3));

	// Reverse 64-bit halves:
	return _mm_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2));
#endif
}
