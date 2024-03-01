
.PHONY: all, clean 


MAIN_OBJECTS = main.o 
CODE_OBJECTS = hashmap.o vector.o pair.o
TEST_OBJECTS = test_suite.o


ALL_LIBRARIES := main.o libhashmap.a  libhashmap_test.a 
#libhashmap_main.a

CC = gcc 
AR = ar -rc
LD = ld -lm

CCFLAGS = -Wall -Wextra -Wvla -Werror -g -lm -std=c99  
#CCFLAGS = -Wall -Wextra -Wvla -Werror -g -lm -std=c99  


all: $(ALL_LIBRARIES) 
	$(CC) $(CCFLAGS) -o output main.o -L/mnt/stbdrm/skahana/files_for_adi -lhashmap_test -lhashmap 
#-L<Path to libs> -l<libname without 'lib'>
	
	
libhashmap.a:$(CODE_OBJECTS) 
	$(AR)   libhashmap.a $(CODE_OBJECTS) 

libhashmap_test.a:$(TEST_OBJECTS) 
	$(AR)   libhashmap_test.a $(TEST_OBJECTS) 

#libhashmap_main.a:$(MAIN_OBJECTS) 
#	$(AR) libhashmap_main.a $(MAIN_OBJECTS) 
	#libhashmap.a  libhashmap_test.a

	
main.o: main.c hashmap.h test_suite.h
	$(CC) $(CCFLAGS) -c $< 
	
test_suite.o: test_suite.c test_suite.h hash_funcs.h test_pairs.h hashmap.h pair.h
	$(CC) $(CCFLAGS) -c $<	


	
hashmap.o: hashmap.c hashmap.h vector.h
	$(CC) $(CCFLAGS) -c $< 
	
vector.o: vector.c vector.h 
	$(CC) $(CCFLAGS) -c $<	
	
pair.o: pair.c pair.h 
	$(CC) $(CCFLAGS) -c $<		
		

clean:
	rm *.o *.a
	
