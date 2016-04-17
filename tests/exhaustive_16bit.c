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

// Inputs to test function:
struct testcase {
	const uint16_t	i;	// First input value
	const uint16_t	j;	// Second input value
	uint16_t	expect;	// Expected output value
};

// Struct to define a simple testcase:
struct test {
	const char *name;
	const char *op;
	__m128i    (*run) (struct testcase *);
};

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
test_epu16_two (struct test *test)
{
	uint16_t buf[8] __attribute__ ((aligned(16)));
	bool pass = true;

	puts(test->name);

	for (int i = 0; i < 0x10000; i++) {
		for (int j = 0; j < 0x10000; j++) {

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
			if (!epu16_all_same(buf)) {
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
test_mm_cmplt_epu16 (struct testcase *tc)
{
	tc->expect = (tc->i < tc->j) ? 0xFFFF : 0x0000;
	return _mm_cmplt_epu16(
		_mm_set1_epi16(tc->i),
		_mm_set1_epi16(tc->j));
}

static __m128i
test_mm_cmple_epu16 (struct testcase *tc)
{
	tc->expect = (tc->i <= tc->j) ? 0xFFFF : 0x0000;
	return _mm_cmple_epu16(
		_mm_set1_epi16(tc->i),
		_mm_set1_epi16(tc->j));
}

static __m128i
test_mm_cmpge_epu16 (struct testcase *tc)
{
	tc->expect = (tc->i >= tc->j) ? 0xFFFF : 0x0000;
	return _mm_cmpge_epu16(
		_mm_set1_epi16(tc->i),
		_mm_set1_epi16(tc->j));
}

static __m128i
test_mm_cmpgt_epu16 (struct testcase *tc)
{
	tc->expect = (tc->i > tc->j) ? 0xFFFF : 0x0000;
	return _mm_cmpgt_epu16(
		_mm_set1_epi16(tc->i),
		_mm_set1_epi16(tc->j));
}

static __m128i
test_mm_min_epu16 (struct testcase *tc)
{
	tc->expect = (tc->i > tc->j) ? tc->j : tc->i;
	return _mm_min_epu16(
		_mm_set1_epi16(tc->i),
		_mm_set1_epi16(tc->j));
}

static __m128i
test_mm_max_epu16 (struct testcase *tc)
{
	tc->expect = (tc->i > tc->j) ? tc->i : tc->j;
	return _mm_max_epu16(
		_mm_set1_epi16(tc->i),
		_mm_set1_epi16(tc->j));
}

static __m128i
test_mm_absdiff_epu16 (struct testcase *tc)
{
	tc->expect = (tc->i > tc->j) ? (tc->i - tc->j) : (tc->j - tc->i);
	return _mm_absdiff_epu16(
		_mm_set1_epi16(tc->i),
		_mm_set1_epi16(tc->j));
}

static bool
test_mm_div255_epu16 (void)
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

int
main (void)
{
	// Map for testing simple bytewise functions:
	struct test map[] = {
		{ "_mm_cmplt_epu16",   "<",       test_mm_cmplt_epu16   },
		{ "_mm_cmple_epu16",   "<=",      test_mm_cmple_epu16   },
		{ "_mm_cmpge_epu16",   ">=",      test_mm_cmpge_epu16   },
		{ "_mm_cmpgt_epu16",   ">",       test_mm_cmpgt_epu16   },
		{ "_mm_min_epu16",     "min",     test_mm_min_epu16     },
		{ "_mm_max_epu16",     "max",     test_mm_max_epu16     },
		{ "_mm_absdiff_epu16", "absdiff", test_mm_absdiff_epu16 },
	};

	bool pass = true;

	for (size_t i = 0; i < sizeof(map) / sizeof(map[0]); i++)
		pass &= test_epu16_two(&map[i]);

	// Handle this one separately:
	pass &= test_mm_div255_epu16();

	return (pass) ? 0 : 1;
}
