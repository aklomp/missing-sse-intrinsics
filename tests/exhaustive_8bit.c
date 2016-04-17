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

// Inputs to test function:
struct testcase {
	const uint8_t	i;	// First input value
	const uint8_t	j;	// Second input value
	uint8_t		expect;	// Expected output value
};

// Struct to define a simple testcase:
struct test {
	const char *name;
	const char *op;
	__m128i    (*run) (struct testcase *);
};

// Check if all 16 epu8's are identical:
static bool
epu8_all_same (uint8_t buf[16])
{
	for (int i = 1; i < 16; i++)
		if (buf[0] != buf[i])
			return false;

	return true;
}

static bool
test_epu8_two (struct test *test)
{
	uint8_t buf[16] __attribute__ ((aligned(16)));
	bool pass = true;

	puts(test->name);

	for (int i = 0; i < 0x100; i++) {
		for (int j = 0; j < 0x100; j++) {

			// Create testcase
			struct testcase tc = {
				.i = i,
				.j = j,
			};

			// Run testcase:
			__m128i c = test->run(&tc);

			// Save result to array:
			_mm_store_si128((__m128i *)buf, c);

			// Check that all elements in the result are identical:
			if (!epu8_all_same(buf)) {
				printf("FAIL: %d %s %d, not all identical\n", tc.i, test->op, tc.j);
				pass = false;
				continue;
			}

			// Does the expected result differ?
			if (buf[0] != tc.expect) {
				printf("FAIL: %d %s %d, expected %d, got %d\n", tc.i, test->op, tc.j, tc.expect, buf[0]);
				pass = false;
				continue;
			}
		}
	}
	return pass;
}

static __m128i
test_mm_cmplt_epu8 (struct testcase *tc)
{
	tc->expect = (tc->i < tc->j) ? 0xFF : 0x00;
	return _mm_cmplt_epu8(
		_mm_set1_epi8(tc->i),
		_mm_set1_epi8(tc->j));
}

static __m128i
test_mm_cmple_epu8 (struct testcase *tc)
{
	tc->expect = (tc->i <= tc->j) ? 0xFF : 0x00;
	return _mm_cmple_epu8(
		_mm_set1_epi8(tc->i),
		_mm_set1_epi8(tc->j));
}

static __m128i
test_mm_cmpge_epu8 (struct testcase *tc)
{
	tc->expect = (tc->i >= tc->j) ? 0xFF : 0x00;
	return _mm_cmpge_epu8(
		_mm_set1_epi8(tc->i),
		_mm_set1_epi8(tc->j));
}

static __m128i
test_mm_cmpgt_epu8 (struct testcase *tc)
{
	tc->expect = (tc->i > tc->j) ? 0xFF : 0x00;
	return _mm_cmpgt_epu8(
		_mm_set1_epi8(tc->i),
		_mm_set1_epi8(tc->j));
}

static __m128i
test_mm_absdiff_epu8 (struct testcase *tc)
{
	tc->expect = (tc->i > tc->j) ? (tc->i - tc->j) : (tc->j - tc->i);
	return _mm_absdiff_epu8(
		_mm_set1_epi8(tc->i),
		_mm_set1_epi8(tc->j));
}

static __m128i
test_mm_scale_epu8 (struct testcase *tc)
{
	tc->expect = (tc->i * tc->j) / 255;
	return _mm_scale_epu8(
		_mm_set1_epi8(tc->i),
		_mm_set1_epi8(tc->j));
}

static bool
test_mm_divfast_epu8 (void)
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

static bool
test_mm_div_epu8 (void)
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

int
main (void)
{
	// Map for testing simple bytewise functions:
	struct test map[] = {
		{ "_mm_cmplt_epu8",   "<",       test_mm_cmplt_epu8   },
		{ "_mm_cmple_epu8",   "<=",      test_mm_cmple_epu8   },
		{ "_mm_cmpge_epu8",   ">=",      test_mm_cmpge_epu8   },
		{ "_mm_cmpgt_epu8",   ">",       test_mm_cmpgt_epu8   },
		{ "_mm_absdiff_epu8", "absdiff", test_mm_absdiff_epu8 },
		{ "_mm_scale_epu8",   "scale",   test_mm_scale_epu8   },
	};

	bool pass = true;

	for (size_t i = 0; i < sizeof(map) / sizeof(map[0]); i++)
		pass &= test_epu8_two(&map[i]);

	pass &= test_mm_div_epu8();

	// This function is inexact by design:
	(void) test_mm_divfast_epu8();

	return (pass) ? 0 : 1;
}
