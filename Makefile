CPPFLAGS=-g -I/usr/local/cuda/include
LDFLAGS=-g
LDLIBS=-L/usr/local/cuda/lib64

NVCC=nvcc 
NVCCFLAGS=--compiler-bindir=/usr/bin/g++-4.8 -gencode arch=compute_35,code=sm_35

OBJECTS = benchmarks/benchmarks.a utils/utils.a kernels.cu 

all: $(OBJECTS) 
	$(NVCC) $(NVCCFLAGS) -o streams main.cpp $(OBJECTS) -Ibenchmarks -Iutils -lnvToolsExt 

$(OBJECTS): $(shell find utils -type f) $(shell find benchmarks -type f)
	$(MAKE) -C utils
	$(MAKE) -C benchmarks

clean:
	$(MAKE) -C utils clean
	$(MAKE) -C benchmarks clean
	rm -rf streams *.o
