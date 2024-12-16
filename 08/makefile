CFLAGS = -O3 -Wall -Wextra -g -std=c++2a -I../lib

input=input

.PHONY: all run
all: one two

run: one two
	echo "Part one: $$(./one < $(input))"
	echo "Part two: $$(./two < $(input))"

one: one.cc
	g++ $(CFLAGS) $< -o $@

two: two.cc
	g++ $(CFLAGS) $< -o $@