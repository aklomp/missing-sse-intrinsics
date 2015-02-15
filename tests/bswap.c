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

static struct testcase {
	char *name;
	char *input;
	char *expect;
	__m128i (* func)(__m128i);
}
testcases[] =
{
  { .name   = "epi16_sse2"
  , .input  = "ABCDEFGHIJKLMNPO"
  , .expect = "BADCFEHGJILKNMOP"
  , .func   = _mm_bswap_epi16_sse2
  }
, { .name   = "epi16_ssse3"
  , .input  = "ABCDEFGHIJKLMNPO"
  , .expect = "BADCFEHGJILKNMOP"
  , .func   = _mm_bswap_epi16_ssse3
  }
, { .name   = "epi32_sse2"
  , .input  = "ABCDEFGHIJKLMNPO"
  , .expect = "DCBAHGFELKJIOPNM"
  , .func   = _mm_bswap_epi32_sse2
  }
, { .name   = "epi32_ssse3"
  , .input  = "ABCDEFGHIJKLMNPO"
  , .expect = "DCBAHGFELKJIOPNM"
  , .func   = _mm_bswap_epi32_ssse3
  }
, { .name   = "epi64_sse2"
  , .input  = "ABCDEFGHIJKLMNPO"
  , .expect = "HGFEDCBAOPNMLKJI"
  , .func   = _mm_bswap_epi64_sse2
  }
, { .name   = "epi64_ssse3"
  , .input  = "ABCDEFGHIJKLMNPO"
  , .expect = "HGFEDCBAOPNMLKJI"
  , .func   = _mm_bswap_epi64_ssse3
  }
, { .name   = "si128_sse2"
  , .input  = "ABCDEFGHIJKLMNPO"
  , .expect = "OPNMLKJIHGFEDCBA"
  , .func   = _mm_bswap_si128_sse2
  }
, { .name   = "si128_ssse3"
  , .input  = "ABCDEFGHIJKLMNPO"
  , .expect = "OPNMLKJIHGFEDCBA"
  , .func   = _mm_bswap_si128_ssse3
  }
} ;

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
