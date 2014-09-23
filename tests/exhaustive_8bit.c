#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <emmintrin.h>

#include "../lib/mm_cmpgt_epu8.h"
#include "../lib/mm_cmplt_epu8.h"
#include "../lib/mm_cmple_epu8.h"
#include "../lib/mm_cmpge_epu8.h"
#include "../lib/mm_absdiff_epu8.h"
#include "../lib/mm_div255_epu16.h"
#include "../lib/mm_scale_epu8.h"
#include "../lib/mm_divfast_epu8.h"
#include "../lib/mm_div_epu8.h"

// Check if all 16 epu8's are identical:
static bool
epu8_all_same (uint8_t buf[16])
{
	for (int i = 1; i < 16; i++) {
		if (buf[0] != buf[i]) {
			return false;
		}
	}
	return true;
}

static bool
test_epu8_two (char *name, __m128i (*op_epu8)(__m128i a, __m128i b, uint8_t i, uint8_t j, uint8_t *expect, char **op))
{
	char *op;
	uint8_t expect;
	uint8_t buf[16] __attribute__ ((aligned(16)));
	bool pass = true;

	puts(name);

	for (int i = 0; i < 0x100; i++) {
		for (int j = 0; j < 0x100; j++) {
			__m128i a = _mm_set1_epi8(i);
			__m128i b = _mm_set1_epi8(j);

			// Run user-defined operation on a and b:
			__m128i c = op_epu8(a, b, i, j, &expect, &op);

			// Save result to array:
			_mm_store_si128((__m128i *)buf, c);

			// Check that all elements in the result are identical:
			if (!epu8_all_same(buf)) {
				printf("FAIL: %d %s %d, not all identical\n", i, op, j);
				pass = false;
				continue;
			}
			// Does the expected result differ?
			if (buf[0] != expect) {
				printf("FAIL: %d %s %d, expected %d, got %d\n", i, op, j, expect, buf[0]);
				pass = false;
				continue;
			}
		}
	}
	return pass;
}

// ---------------------------------------------------------------------------

static __m128i
do_mm_cmplt_epu8 (__m128i a, __m128i b, uint8_t i, uint8_t j, uint8_t *expect, char **op)
{
	*op = "<";
	*expect = (i < j) ? 0xFF : 0x00;
	return _mm_cmplt_epu8(a, b);
}
static bool
test_mm_cmplt_epu8 ()
{
	return test_epu8_two("_mm_cmplt_epu8", do_mm_cmplt_epu8);
}

// ---------------------------------------------------------------------------

static __m128i
do_mm_cmple_epu8 (__m128i a, __m128i b, uint8_t i, uint8_t j, uint8_t *expect, char **op)
{
	*op = "<=";
	*expect = (i <= j) ? 0xFF : 0x00;
	return _mm_cmple_epu8(a, b);
}
static bool
test_mm_cmple_epu8 ()
{
	return test_epu8_two("_mm_cmple_epu8", do_mm_cmple_epu8);
}

// ---------------------------------------------------------------------------

static __m128i
do_mm_cmpge_epu8 (__m128i a, __m128i b, uint8_t i, uint8_t j, uint8_t *expect, char **op)
{
	*op = ">=";
	*expect = (i >= j) ? 0xFF : 0x00;
	return _mm_cmpge_epu8(a, b);
}
static bool
test_mm_cmpge_epu8 ()
{
	return test_epu8_two("_mm_cmpge_epu8", do_mm_cmpge_epu8);
}

// ---------------------------------------------------------------------------

static __m128i
do_mm_cmpgt_epu8 (__m128i a, __m128i b, uint8_t i, uint8_t j, uint8_t *expect, char **op)
{
	*op = ">";
	*expect = (i > j) ? 0xFF : 0x00;
	return _mm_cmpgt_epu8(a, b);

}
static bool
test_mm_cmpgt_epu8 ()
{
	return test_epu8_two("_mm_cmpgt_epu8", do_mm_cmpgt_epu8);
}

// ---------------------------------------------------------------------------

static __m128i
do_mm_absdiff_epu8 (__m128i a, __m128i b, uint8_t i, uint8_t j, uint8_t *expect, char **op)
{
	*op = "absdiff";
	*expect = (i > j) ? (i - j) : (j - i);
	return _mm_absdiff_epu8(a, b);
}
static bool
test_mm_absdiff_epu8 ()
{
	return test_epu8_two("_mm_absdiff_epu8", do_mm_absdiff_epu8);
}

// ---------------------------------------------------------------------------

static __m128i
do_mm_scale_epu8 (__m128i a, __m128i b, uint8_t i, uint8_t j, uint8_t *expect, char **op)
{
	*op = "scale";
	*expect = (i * j) / 255;
	return _mm_scale_epu8(a, b);

}
static bool
test_mm_scale_epu8 ()
{
	return test_epu8_two("_mm_scale_epu8", do_mm_scale_epu8);
}

// ---------------------------------------------------------------------------

static bool
test_mm_divfast_epu8 ()
{
	bool pass = true;

	puts("_mm_divfast_epu8");
	for (int i = 0; i < 0x100; i++) {
		for (int j = 1; j < 0x100; j++) {
			__m128i a = _mm_set1_epi8(i);
			uint8_t c = _mm_extract_epi16(_mm_divfast_epu8(a, j), 1) & 0xFF;
			uint8_t s = i / j;
			if (c != s) {
				printf("FAIL: div(%d, %d), got %d, expected %d\n", i, j, c, s);
				pass = false;
			}
		}
	}
	return pass;
}

// ---------------------------------------------------------------------------

static bool
test_mm_div_epu8 ()
{
	bool pass = true;

	puts("_mm_div_epu8");
	for (int i = 0; i < 0x100; i++) {
		for (int j = 1; j < 0x100; j++) {
			__m128i a = _mm_set1_epi8(i);
			uint8_t c = _mm_extract_epi16(_mm_div_epu8(a, j), 1) & 0xFF;
			uint8_t s = i / j;
			if (c != s) {
				printf("FAIL: divp(%d, %d), got %d, expected %d\n", i, j, c, s);
				pass = false;
			}
		}
	}
	return pass;
}

// ---------------------------------------------------------------------------

int
main ()
{
	bool pass = false;

	pass |= test_mm_cmplt_epu8();
	pass |= test_mm_cmple_epu8();
	pass |= test_mm_cmpge_epu8();
	pass |= test_mm_cmpgt_epu8();
	pass |= test_mm_absdiff_epu8();
	pass |= test_mm_scale_epu8();
	pass |= test_mm_div_epu8();

	// This function is inexact by design:
	(void) test_mm_divfast_epu8();

	return (pass) ? 0 : 1;
}
