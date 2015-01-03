CFLAGS += -std=c99 -Wall -Wextra -Werror -pedantic -msse2 -O3

.PHONY: test clean

# This target rebuilds and runs all tests:
test: clean exhaustive_8bit exhaustive_16bit
	./exhaustive_8bit
	./exhaustive_16bit

exhaustive_%bit: exhaustive_%bit.o
	$(CC) $(LDFLAGS) -o $@ $^

exhaustive_%bit.o: exhaustive_%bit.c ../lib/*.h
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o exhaustive_{8,16}bit
