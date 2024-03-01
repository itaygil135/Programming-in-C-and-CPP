
PHONY: run_tests, run_tests_verbose, clean

CCFLAGS = -Wall -Wvla -Wextra -Werror -g -std=c++14

CC = g++

run_tests: tests
	tests --color_output=true

run_tests-verbose: tests
	tests --color_output=true -l all

run_tests-white: tests
	tests --color_output=false

run_tests-white-verbose: tests
	tests --color_output=false -l all

valgrind_tests: tests
	valgrind --track-origins=yes tests


tests: ex6_tests.cpp vl_vector.h
	$(CC) $(CCFLAGS) $< -o tests

clean:
	rm tests