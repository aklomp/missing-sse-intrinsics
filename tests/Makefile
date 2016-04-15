CFLAGS += -std=c99 -Wall -Wextra -Werror -pedantic -msse2 -O3

.PHONY: test clean

PROGS = \
  bswap \
  exhaustive_8bit \
  exhaustive_16bit

# The default target rebuilds and runs all tests:
test: clean all
	./bswap
	./exhaustive_8bit
	./exhaustive_16bit

all: $(PROGS)

exhaustive_%bit: exhaustive_%bit.o
	$(CC) $(LDFLAGS) -o $@ $^

exhaustive_%bit.o: exhaustive_%bit.c ../lib/*.h
	$(CC) $(CFLAGS) -o $@ -c $<

bswap: bswap.o bswap_sse2.o bswap_ssse3.o
	$(CC) $(LDFLAGS) -o $@ $^

bswap_sse2.o: bswap_trampoline.c
	$(CC) $(CFLAGS) -DFUNC_POSTFIX=sse2 -o $@ -c $<

bswap_ssse3.o: bswap_trampoline.c
	$(CC) $(CFLAGS) -DFUNC_POSTFIX=ssse3 -mssse3 -o $@ -c $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o $(PROGS)
