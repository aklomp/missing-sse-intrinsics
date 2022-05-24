#include <string.h>
#include <stdio.h>
#include <emmintrin.h>

// Forward declarations of the functions under test:
__m128i _mm_bswap_epi16_sse2 (__m128i);
__m128i _mm_bswap_epi32_sse2 (__m128i);
__m128i _mm_bswap_epi64_sse2 (__m128i);
__m128i _mm_bswap_si128_sse2 (__m128i);
__m128i _mm_bswap_epi16_ssse3 (__m128i);
__m128i _mm_bswap_epi32_ssse3 (__m128i);
__m128i _mm_bswap_epi64_ssse3 (__m128i);
__m128i _mm_bswap_si128_ssse3 (__m128i);

#define TESTCASE(__name, __input, __expect)	\
	{ .name   = # __name			\
	, .input  = __input			\
	, .expect = __expect			\
	, .func   = _mm_bswap_ ## __name	\
	}

static struct testcase {
	char *name;
	char *input;
	char *expect;
	__m128i (* func)(__m128i);
}
testcases[] =
{
	TESTCASE (epi16_sse2,  "ABCDEFGHIJKLMNPO", "BADCFEHGJILKNMOP"),
	TESTCASE (epi16_ssse3, "ABCDEFGHIJKLMNPO", "BADCFEHGJILKNMOP"),
	TESTCASE (epi32_sse2,  "ABCDEFGHIJKLMNPO", "DCBAHGFELKJIOPNM"),
	TESTCASE (epi32_ssse3, "ABCDEFGHIJKLMNPO", "DCBAHGFELKJIOPNM"),
	TESTCASE (epi64_sse2,  "ABCDEFGHIJKLMNPO", "HGFEDCBAOPNMLKJI"),
	TESTCASE (epi64_ssse3, "ABCDEFGHIJKLMNPO", "HGFEDCBAOPNMLKJI"),
	TESTCASE (si128_sse2,  "ABCDEFGHIJKLMNPO", "OPNMLKJIHGFEDCBA"),
	TESTCASE (si128_ssse3, "ABCDEFGHIJKLMNPO", "OPNMLKJIHGFEDCBA"),
};

static int
run_testcase (struct testcase *t)
{
	char buf[17];
	__m128i in = _mm_loadu_si128((__m128i *)t->input);
	__m128i out = t->func(in);
	_mm_storeu_si128((__m128i *)buf, out);
	buf[16] = '\0';

	if (memcmp(t->expect, buf, 16) == 0) {
		return 0;
	}
	printf("FAIL: %s:\n for: %s\n exp: %s\n got: %s\n", t->name, t->input, t->expect, buf);
	return 1;
}

int
main (void)
{
	unsigned int i, ret = 0;

	for (i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++) {
		ret |= run_testcase(&testcases[i]);
	}
	return ret;
}
