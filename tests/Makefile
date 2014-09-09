CFLAGS += -std=c99 -Wall -Wextra -Werror -pedantic -msse2 -O3

.PHONY: all clean

all: exhaustive_8bit

exhaustive_8bit: exhaustive_8bit.o
	$(CC) $(LDFLAGS) -o $@ $^

exhaustive_8bit.o: exhaustive_8bit.c ../lib/*.h
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o exhaustive_8bit
