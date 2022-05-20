# A few missing SSE intrinsics

[![Build Status](https://travis-ci.org/aklomp/sse-intrinsics-tests.svg?branch=master)](https://travis-ci.org/aklomp/sse-intrinsics-tests)

C and C++ programmers already have many
[SSE intrinsics](https://software.intel.com/sites/landingpage/IntrinsicsGuide/)
available to them. Most of those map straightforwardly to their matching
hardware instructions, but there are holes where the hardware doesn't natively
support a given operation or type. For instance, there's an `_mm_cmpgt_epi8`
intrinsic, but not an `_mm_cmpgt_epu8`. The first one exists because there's a
hardware primitive, the second one is missing because its implementation would
have to be composite.

The set of intrinsics that we do have is quite rich, and can be combined in
many ways to make up for the ones we don't. Because SSE programming is tricky
enough as it is, I'm a big fan of building ever more small, self-contained
"primitives" that are composited inline into ever larger functional units. We
create the building blocks, forge them into subassemblies, and then let the
compiler inline everything into an optimized function body.

This repo contains some of the new "missing intrinsics" that I built on top of
the existing ones, and gradually also on top of the ones defined earlier. I
purposefully followed the existing naming conventions for type and operator
names, so that these functions act as a kind of "polyfill" for the holes in the
existing set (to use a JavaScript term). The naming convention probably
violates a standard somewhere, but I think it's defensible: most of these
functions are small enough to be considered almost-primitives, some of the
original intrinsics are also implemented as composites, and most operate in the
same way as the actual intrinsics, namely in vertical fashion. (`x0` and `y0`
form a pair, `x1` and `y1` form a pair, and so on.) With these new
"primitives", SSE code becomes easier to read, test and reason about.

## License

Informally speaking, these are fundamental building blocks as far as I'm
concerned, and I wouldn't want to claim any rights to these. For what it's
worth, I declare that I place these in the public domain. Do what you want with
this stuff.

Formally speaking, this repo is released under the
[Unlicense](https://unlicense.org/). The contents of this license can be found
in the `LICENSE` file.

## This repository

This repository contains the code for some of the "missing SSE intrinsics"
documented below. There are also some exhaustive tests for all 8-bit and 16-bit
inputs. The only function that doesn't pass these tests is
`_mm_divfast_epu8()`, and that's deliberate. It trades speed and simplicity for
a few rare off-by-one errors.

## Documentation

### \_mm\_cmple\_epu8

Compare each of the 8-bit unsigned ints in `x` to those in `y` and set the
result to `0xFF` where `x <= y`. Equivalent to taking the minimum of both
vectors and checking where `x` equals this minimum.

| R0                         | R1                         | ... | R15                          |
|----------------------------|----------------------------|-----|------------------------------|
| `(x0 <= y0) ? 0xFF : 0x00` | `(x1 <= y1) ? 0xFF : 0x00` | ... | `(x15 <= y15) ? 0xFF : 0x00` |

### \_mm\_cmpge\_epu8

Compare each of the 8-bit unsigned ints in `x` to those in `y` and set the
result to `0xFF` where `x >= y`. Equivalent to calling `_mm_cmple_epu8` above
with the arguments swapped.

| R0                        | R1                        | ... | R15                         |
|---------------------------|---------------------------|-----|-----------------------------|
| `(x0 >= y0) ? 0xFF : 0x0` | `(x1 >= y1) ? 0xFF : 0x0` | ... | `(x15 >= y15) ? 0xFF : 0x0` |

### \_mm\_cmpgt\_epu8

Compare each of the 8-bit unsigned ints in `x` to those in `y` and set the
result to `0xFF` where `x > y`. Equivalent to checking whether `x` is equal to
the maximum of `x` and `y`, but not equal to `y` itself.

| R0                       | R1                       | ... | R15                        |
|--------------------------|--------------------------|-----|----------------------------|
| `(x0 > y0) ? 0xFF : 0x0` | `(x1 > y1) ? 0xFF : 0x0` | ... | `(x15 > y15) ? 0xFF : 0x0` |

### \_mm\_cmplt\_epu8

Compare each of the 8-bit unsigned ints in `x` to those in `y` and set the
result to `0xFF` where `x < y`. Equivalent to calling `_mm_cmpgt_epu8` above
with swapped arguments.

| R0                       | R1                       | ... | R15                        |
|--------------------------|--------------------------|-----|----------------------------|
| `(x0 < y0) ? 0xFF : 0x0` | `(x1 < y1) ? 0xFF : 0x0` | ... | `(x15 < y15) ? 0xFF : 0x0` |

### \_mm\_cmple\_epu16

Compare each of the eight 16-bit unsigned ints in `x` to those in `y` and set
the result to `0xFFFF` where `x <= y`. Equivalent to doing the saturating
subtraction of `x − y` and checking where the result is zero (meaning `x` was
larger than or equal to `y`).

| R0                          | R1                          | ... | R7                          |
|-----------------------------|-----------------------------|-----|-----------------------------|
| `(x0 <= y0) ? 0xFFFF : 0x0` | `(x1 <= y1) ? 0xFFFF : 0x0` | ... | `(x7 <= y7) ? 0xFFFF : 0x0` |

### \_mm\_cmpge\_epu16

Compare each of the eight 16-bit unsigned ints in `x` to those in `y` and set
the result to `0xFFFF` where `x >= y`. Equivalent to calling `_mm_cmple_epu16`
above with the arguments swapped.

| R0                          | R1                          | ... | R7                          |
|-----------------------------|-----------------------------|-----|-----------------------------|
| `(x0 >= y0) ? 0xFFFF : 0x0` | `(x1 >= y1) ? 0xFFFF : 0x0` | ... | `(x7 >= y7) ? 0xFFFF : 0x0` |

### \_mm\_cmpgt\_epu16

Compare each of the eight 16-bit unsigned ints in `x` to those in `y` and set
the result to `0xFFFF` where `x > y`. Equivalent to checking whether `x` is
larger than, but not equal to, `y`.

| R0                         | R1                         | ... | R7                         |
|----------------------------|----------------------------|-----|----------------------------|
| `(x0 > y0) ? 0xFFFF : 0x0` | `(x1 > y1) ? 0xFFFF : 0x0` | ... | `(x7 > y7) ? 0xFFFF : 0x0` |

### \_mm\_cmplt\_epu16

Compare each of the 16-bit unsigned ints in `x` to those in `y` and set the
result to `0xFFFF` where `x < y`. Equivalent to calling `_mm_cmpgt_epu16` above
with swapped arguments.

| R0                         | R1                         | ... | R7                         |
|----------------------------|----------------------------|-----|----------------------------|
| `(x0 < y0) ? 0xFFFF : 0x0` | `(x1 < y1) ? 0xFFFF : 0x0` | ... | `(x7 < y7) ? 0xFFFF : 0x0` |

### \_mm\_cmpge\_epi16

Compare each of the eight 16-bit signed ints in `x` to those in `y` and set the
result to `0xFFFF` where `x >= y`. Equivalent to checking whether `x` is either
larger than or equal to `y`.

| R0                          | R1                          | ... | R7                          |
|-----------------------------|-----------------------------|-----|-----------------------------|
| `(x0 >= y0) ? 0xFFFF : 0x0` | `(x1 >= y1) ? 0xFFFF : 0x0` | ... | `(x7 >= y7) ? 0xFFFF : 0x0` |

### \_mm\_not\_si128

Returns `~x`, the bitwise complement (inverse) of `x`. Perform an `xor` of
every bit with 1, because `0 xor 1 = 1` and `1 xor 1 = 0`. This function can
usually be avoided by applying
[De Morgan's laws](https://en.wikipedia.org/wiki/De_Morgan's_laws) to your
logic statements, so that they use the native `_mm_andnot_si128`.

| R0    |
|-------|
| `~x0` |

### \_mm\_setone\_epi8

Just like `_mm_setzero_si128` sets everything to zero, this function sets all
bytes in the vector to the value `1` without touching memory. A vector where
every lane is `1` can be useful for incrementing a set of counters or
performing a shift. However, a `_mm_set1_epi8(1)` will probably still be
faster.

| R0    | R1    | ... | R15  |
|-------|-------|-----|------|
| `0x1` | `0x1` | ... |`0x1` |

### \_mm\_setone\_epi16

See above. Sets all words in the vector to the value `1` without touching
memory. A `_mm_set1_epi16(1)` is probably faster.

| R0    | R1    | ... | R7    |
|-------|-------|-----|-------|
| `0x1` | `0x1` | ... | `0x1` |

### \_mm\_blendv\_si128

Copies the bits from `x` to the output where `mask` is not set, and the bits
from `y` where mask is set. Combines the values in `x` with those in `y` based
on the bitmask. This is useful for replacing branching logic with masking
logic, because the SSE truth operations return all-ones for the bytes where a
given condition holds.

| R0                            |
|-------------------------------|
| `(x0 & ~mask) \| (y0 & mask)` |

### \_mm\_blendv\_epi8

This function was introduced as a hardware primitive in SSE 4.1. For each of
the 16 bytes, copy either the byte from `x` or the byte from `y` to the output,
depending on whether the matching byte in `mask` has its most significant bit
(MSB) set. If the MSB is set, the byte in `y` is copied, else the one from `x`
is passed through.

In practice with older SSE versions, when you're using logic masks in which
each byte is known to be `0x00` or `0xFF`, it's faster to use
`_mm_blendv_si128`.

| R0                         | R1                         | ... | R15                           |
|----------------------------|----------------------------|-----|-------------------------------|
| `(mask0 & 0x80) ? y0 : x0` | `(mask1 & 0x80) ? y1 : x1` | ... | `(mask15 & 0x80) ? y15 : x15` |

### \_mm\_min\_epu16

Compare each of the eight 16-bit unsigned ints in `x` to those in `y` and place
the one with the lowest value in the result. This demonstrates the use of
`_mm_blendv_si128` defined above to sculpt the output based on a condition
mask. This instruction was introduced as a hardware primitive in SSE4.

| R0            | R1            | ... | R7            |
|---------------|---------------|-----|---------------|
| `min(x0, y0)` | `min(x1, y1)` | ... | `min(x7, y7)` |

### \_mm\_max\_epu16

Compare each of the eight 16-bit unsigned ints in `x` to those in `y` and place
the one with the highest value in the result. Equivalent to `_mm_min_epu16`
above with the inverse result. This instruction was introduced as a hardware
primitive in SSE4.

| R0            | R1            | ... | R7            |
|---------------|---------------|-----|---------------|
| `max(x0, y0)` | `max(x1, y1)` | ... | `max(x7, y7)` |

### \_mm\_absdiff\_epu8

Calculate the absolute difference between the 8-bit unsigned int in `x` and
`y`. Use unsigned saturating subtraction for fast calculation. Saturating
subtraction clamps negative results to zero. The absolute difference is
`subs(x, y) + subs(y, x)`. For example, the absolute difference between 16 and
7 is `subs(16 − 7) + subs(7 − 16)`, or `9 + 0`. Since at least one of the
subtractions will be zero, we can use an `or` to combine them.

| R0             | R1             | ... | R15              |
|----------------|----------------|-----|------------------|
| `abs(x0 − y0)` | `abs(x1 − y1)` | ... | `abs(x15 − y15)` |

### \_mm\_absdiff\_epu16

Same as above, but for 16-bit unsigned ints.

| R0             | R1             | ... | R7             |
|----------------|----------------|-----|----------------|
| `abs(x0 − y0)` | `abs(x1 − y1)` | ... | `abs(x7 − y7)` |

### \_mm\_div255\_epu16

Divides all eight 16-bit unsigned ints in `x` by the constant value 255, using
the formula `x := ((x + 1) + (x >> 8)) >> 8`.

| R0         | R1         | ... | R7         |
|------------|------------|-----|------------|
| `x0 ∕ 255` | `x1 ∕ 255` | ... | `x7 ∕ 255` |

### \_mm\_scale\_epu8

"Alpha blend" the 8-bit unsigned ints in `x` with the 8-bit unsigned "opacity"
value in `y`. Calculates `x := x * (y / 255)`, or scaling `x` by the ratio in
`y`. Could be useful for image alpha blending.

| R0                | R1                | ... | R15                 |
|-------------------|-------------------|-----|---------------------|
| `(x0 * y0) ∕ 255` | `(x1 * y1) ∕ 255` | ... | `(x15 * y15) ∕ 255` |

### \_mm\_divfast\_epu8

Divide the 8-bit unsigned ints in `x` by the (scalar, non-vector) 8-bit
unsigned integer `d`, accepting a slight error for 0.12% of the input space.
This works on the principle that `a / b` is equal to `(a * c) / (b * c)`, where
`c` is some arbitrary constant. After rearranging parentheses, we have `(a * (c
/ b)) * c`. This reduces the problem to two multiplications and one constant
division. The trick is finding a properly sized constant `c` such that `c / b`
does not alias (implying that `c` should be at least `256 * b`), but not so big
that multiplying later by `c` will overflow the result. And `c` should be a
power of two so that we can do the last multiply with a left shift.

For all 65280 possible inputs (256 numerators with 255 denominators, since zero
is not a denominator), this function is wrong in just 78 cases, as compared to
regular truncating integer division. In each of those cases, the error is the
same, namely off by +1. If that is acceptable, this method is fast. Or we can
correct the error at a small performance cost as shown below with
`_mm_div_epu8`.

| R0       | R1       | ... | R15       |
|----------|----------|-----|-----------|
| `x0 ∕ d` | `x1 ∕ d` | ... | `x15 ∕ d` |

### \_mm\_div\_epu8

Divide the 8-bit unsigned ints in `x` by the (non-vector) 8-bit unsigned
integer `d`, doing back-substitution to check for and correct for the 78 error
cases of the faster but inaccurate method above. Once we found the result the
fast way, we multiply it with the divisor and check whether the result is close
enough to the original numerator. If not, we splice in the result minus one.
Surprisingly, this is just 10 to 20 percent slower than the fast version above.

| R0       | R1       | ... | R15       |
|----------|----------|-----|-----------|
| `x0 ∕ d` | `x1 ∕ d` | ... | `x15 ∕ d` |

### \_mm\_bswap\_epi16

Change endianness (reverse byte order) in each 16-bit word by exchanging the
high and the low byte.

| R0   | R1   | R2   | R3   | R4   | ... | R15   |
|------|------|------|------|------|-----|-------|
| `R1` | `R0` | `R3` | `R2` | `R5` | ... | `R14` |

### \_mm\_bswap\_epi32

Change endianness (reverse byte order) in each 32-bit word by reversing all
four bytes. Assume that the SSSE3 `_mm_shuffle_epi8` function plus a literal
value does the job faster if available.

| R0   | R1   | R2   | R3   | R4   | ... | R15   |
|------|------|------|------|------|-----|-------|
| `R3` | `R2` | `R1` | `R0` | `R7` | ... | `R12` |

### \_mm\_bswap\_epi64

Change endianness (reverse byte order) in each 64-bit word by reversing all
eight bytes.

| R0   | R1   | R2   | R3   | R4   | ... | R15  |
|------|------|------|------|------|-----|------|
| `R7` | `R6` | `R5` | `R4` | `R3` | ... | `R8` |

### \_mm\_bswap\_si128

Change endianness (reverse byte order) in the 128-bit vector by reversing all
sixteen bytes.

| R0    | R1    | R2    | R3    | R4    | ... | R15  |
|-------|-------|-------|-------|-------|-----|------|
| `R15` | `R14` | `R13` | `R12` | `R11` | ... | `R0` |
