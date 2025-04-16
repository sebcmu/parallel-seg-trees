APP_NAME=rangequery

CXX = g++ -m64
NVCC = nvcc

CXXFLAGS = -Wall -O3 -std=c++20
NVCCFLAGS = -O3 -m64 -std=c++17 --gpu-architecture=compute_61 -g -G -ccbin /usr/bin/gcc
LDFLAGS = -L/usr/local/cuda-11.7/lib64 -lcudart
OBJS = main.o serial.o coarse.o fine.o lockfree.o cudaprefix.o cudalevels.o helpers.o

all: $(APP_NAME)

$(APP_NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cu
	$(NVCC) $(NVCCFLAGS) -c $< -o $@

clean:
	rm -f $(APP_NAME) *.o

.PHONY: all clean
