APP_NAME=gen

OBJS=gen.o

CXX = g++
CXXFLAGS = -Wall -O3 -std=c++20

all: $(APP_NAME)

$(APP_NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(APP_NAME) $(OBJS)

.PHONY: all clean