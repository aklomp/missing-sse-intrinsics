CFLAGS += -std=c99 -Wall -Wextra -Werror -pedantic -msse2 -O3

.PHONY: all test clean

all: \
  exhaustive_8bit \
  exhaustive_16bit

exhaustive_%bit: exhaustive_%bit.o
	$(CC) $(LDFLAGS) -o $@ $^

exhaustive_%bit.o: exhaustive_%bit.c ../lib/*.h
	$(CC) $(CFLAGS) -o $@ -c $<

# This target rebuilds all tests and runs them:
test: clean all
	./exhaustive_8bit
	./exhaustive_16bit

clean:
	rm -f *.o exhaustive_{8,16}bit
