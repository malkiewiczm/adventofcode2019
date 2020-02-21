CC := g++
CFLAGS := -Wall -Wextra -Wpedantic -Wshadow -std=c++17 -Ofast -march=native

MAKEFLAGS += Rr

%.exe: %.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f *.exe
