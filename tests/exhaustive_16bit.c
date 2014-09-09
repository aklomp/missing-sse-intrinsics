#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <emmintrin.h>

#include "../lib/mm_cmple_epu16.h"
#include "../lib/mm_cmpgt_epu16.h"
#include "../lib/mm_cmplt_epu16.h"
#include "../lib/mm_cmpge_epu16.h"
#include "../lib/mm_blendv_si128.h"
#include "../lib/mm_min_epu16.h"
#include "../lib/mm_max_epu16.h"
#include "../lib/mm_absdiff_epu16.h"
#include "../lib/mm_div255_epu16.h"

// Check if all 8 epu16's are identical:
static bool
epu16_all_same (uint16_t buf[8])
{
	for (int i = 1; i < 8; i++) {
		if (buf[0] != buf[i]) {
			return false;
		}
	}
	return true;
}

static bool
test_epu16_two (char *name, __m128i (*op_epu16)(__m128i a, __m128i b, uint16_t i, uint16_t j, uint16_t *expect, char **op))
{
	char *op;
	uint16_t expect;
	uint16_t buf[8] __attribute__ ((aligned(16)));
	bool pass = true;

	puts(name);
	for (int i = 0; i < 0x10000; i++) {
		for (int j = 0; j < 0x10000; j++) {
			__m128i a = _mm_set1_epi16(i);
			__m128i b = _mm_set1_epi16(j);

			// Run user-defined operation on a and b:
			__m128i c = op_epu16(a, b, i, j, &expect, &op);

			// Save result to array:
			_mm_store_si128((__m128i *)buf, c);

			// Check that all elements in the result are identical:
			if (!epu16_all_same(buf)) {
				printf("FAIL: %d %s %d, not all identical\n", i, op, j);
				pass = false;
				continue;
			}
			// Does the expected result differ?
			if (buf[0] != expect) {
				printf("FAIL: %d %s %d\n", i, op, j);
				pass = false;
				continue;
			}
		}
	}
	return pass;
}

// ---------------------------------------------------------------------------

static __m128i
do_mm_cmplt_epu16 (__m128i a, __m128i b, uint16_t i, uint16_t j, uint16_t *expect, char **op)
{
	*op = "<";
	*expect = (i < j) ? 0xFFFF : 0x0000;
	return _mm_cmplt_epu16(a, b);
}
static bool
test_mm_cmplt_epu16 ()
{
	return test_epu16_two("_mm_cmplt_epu16", do_mm_cmplt_epu16);
}

// ---------------------------------------------------------------------------

static __m128i
do_mm_cmple_epu16 (__m128i a, __m128i b, uint16_t i, uint16_t j, uint16_t *expect, char **op)
{
	*op = "<=";
	*expect = (i <= j) ? 0xFFFF : 0x0000;
	return _mm_cmple_epu16(a, b);
}
static bool
test_mm_cmple_epu16 ()
{
	return test_epu16_two("_mm_cmple_epu16", do_mm_cmple_epu16);
}

// ---------------------------------------------------------------------------

static __m128i
do_mm_cmpge_epu16 (__m128i a, __m128i b, uint16_t i, uint16_t j, uint16_t *expect, char **op)
{
	*op = ">=";
	*expect = (i >= j) ? 0xFFFF : 0x0000;
	return _mm_cmpge_epu16(a, b);
}
static bool
test_mm_cmpge_epu16 ()
{
	return test_epu16_two("_mm_cmpge_epu16", do_mm_cmpge_epu16);
}

// ---------------------------------------------------------------------------

static __m128i
do_mm_cmpgt_epu16 (__m128i a, __m128i b, uint16_t i, uint16_t j, uint16_t *expect, char **op)
{
	*op = ">";
	*expect = (i > j) ? 0xFFFF : 0x0000;
	return _mm_cmpgt_epu16(a, b);
}
static bool
test_mm_cmpgt_epu16 ()
{
	return test_epu16_two("_mm_cmpgt_epu16", do_mm_cmpgt_epu16);
}

// ---------------------------------------------------------------------------

static __m128i
do_mm_min_epu16 (__m128i a, __m128i b, uint16_t i, uint16_t j, uint16_t *expect, char **op)
{
	*op = "min";
	*expect = (i > j) ? j : i;
	return _mm_min_epu16(a, b);
}
static bool
test_mm_min_epu16 ()
{
	return test_epu16_two("_mm_min_epu16", do_mm_min_epu16);
}

// ---------------------------------------------------------------------------

static __m128i
do_mm_max_epu16 (__m128i a, __m128i b, uint16_t i, uint16_t j, uint16_t *expect, char **op)
{
	*op = "max";
	*expect = (i > j) ? i : j;
	return _mm_max_epu16(a, b);
}
static bool
test_mm_max_epu16 ()
{
	return test_epu16_two("_mm_max_epu16", do_mm_max_epu16);
}

// ---------------------------------------------------------------------------

static __m128i
do_mm_absdiff_epu16 (__m128i a, __m128i b, uint16_t i, uint16_t j, uint16_t *expect, char **op)
{
	*op = "absdiff";
	*expect = (i > j) ? (i - j) : (j - i);
	return _mm_absdiff_epu16(a, b);
}
static bool
test_mm_absdiff_epu16 ()
{
	return test_epu16_two("_mm_absdiff_epu16", do_mm_absdiff_epu16);
}

// ---------------------------------------------------------------------------

static bool
test_mm_div255_epu16 ()
{
	bool pass = true;

	// Only works when i < 256*255 = 65280:
	// (result is 8-bit):
	puts("_mm_div255_epu16");
	for (int i = 0; i < 0xFF00; i++) {
		uint16_t c = _mm_extract_epi16(_mm_div255_epu16(_mm_set1_epi16(i)), 1);
		if (c != (i / 255)) {
			printf("FAIL: div255(%d), got %d, expected %d\n", i, c, (i / 255));
			pass = false;
		}
	}
	return pass;
}

// ---------------------------------------------------------------------------

int
main ()
{
	bool pass = false;

	pass |= test_mm_cmplt_epu16();
	pass |= test_mm_cmple_epu16();
	pass |= test_mm_cmpge_epu16();
	pass |= test_mm_cmpgt_epu16();
	pass |= test_mm_min_epu16();
	pass |= test_mm_max_epu16();
	pass |= test_mm_absdiff_epu16();
	pass |= test_mm_div255_epu16();

	return (pass) ? 0 : 1;
}
