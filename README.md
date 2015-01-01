# SSE intrinsics testfiles

This repository contains the code for some of the "missing SSE intrinsics" described [here](http://www.alfredklomp.com/programming/sse-intrinsics).
There are also some exhaustive tests for all 8-bit and 16-bit inputs.
The only function that doesn't pass these tests is `_mm_divfast_epu8()`, and that's deliberate.
It trades speed and simplicity for a few rare off-by-one errors.

[![Build Status](https://travis-ci.org/aklomp/sse-intrinsics-tests.svg?branch=master)](https://travis-ci.org/aklomp/sse-intrinsics-tests)

## License

I consider these functions so basic that I can't claim any ownership, so everything is in the public domain.
