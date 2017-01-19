CC=g++
CPPFLAGS=-g -I/usr/local/cuda/include -I../utils --std=c++11

SOURCES=non_unified_separate.cpp unified_separate.cpp non_unified_same.cpp unified_same.cpp
LIBRARY=benchmarks.a
OBJECTS=non_unified_separate.o unified_separate.o non_unified_same.o unified_same.o

$(LIBRARY): $(OBJECTS)
	ar rvs $(LIBRARY) $(OBJECTS)

$(OBJECTS): $(SOURCES) 
	$(CC) $(CPPFLAGS) -c $(SOURCES) benchmarks.hpp

clean:
	rm -rf *.a *.o
