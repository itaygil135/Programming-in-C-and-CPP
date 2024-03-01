cmake_minimum_required(VERSION 3.17)
project(ex4_itaygil135 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(ex4_itaygil135
        main.c
        vector.c
        vector.h
        test_suite.c
        test_suite.h
        test_pairs.h
        hash_funcs.h
        hashmap.c
        hashmap.h
        pair.c
        pair.h)
