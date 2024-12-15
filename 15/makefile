
input=input

.PHONY: all run
all: one two

run: one two
	echo "Part one: $$(./one < $(input))"
	echo "Part two: $$(./two < $(input))"

one: one.cc
	g++ -O3 -Wall -Wextra -g $< -o $@

two: two.cc
	g++ -O3 -Wall -Wextra -g $< -o $@