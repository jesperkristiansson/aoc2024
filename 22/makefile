ifeq ($(OS),Windows_NT)
COMPILER = cl.exe
CFLAGS = -O2 -W4 -EHsc -std:c++20 -I../lib
OUTPUT_OPTION = /Fe
else
COMPILER = g++
CFLAGS = -O3 -Wall -Wextra -g -std=c++2a -I../lib
OUTPUT_OPTION = -o
endif

input=input

.PHONY: all run
all: one two

run: one two		#does not work on windows, run manually
	echo "Part one: $$(./one < $(input))"
	echo "Part two: $$(./two < $(input))"

one: one.cc
	$(COMPILER) $(CFLAGS) $< $(OUTPUT_OPTION)$@

two: two.cc
	$(COMPILER) $(CFLAGS) $< $(OUTPUT_OPTION)$@
