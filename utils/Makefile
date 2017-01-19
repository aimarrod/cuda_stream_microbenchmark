CC=gcc
CPPFLAGS=-g -I/usr/local/cuda/include

SOURCES=allocations.cpp sequences.cpp events.cpp streams.cpp
LIBRARY=utils.a
OBJECTS=allocations.o sequences.o events.o streams.o

$(LIBRARY): $(OBJECTS)
	ar rvs $(LIBRARY) $(OBJECTS)

$(OBJECTS): $(SOURCES) utils.hpp
	$(CC) $(CPPFLAGS) -c $(SOURCES)

clean:
	rm -rf *.a *.o
