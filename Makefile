# FastIO Benchmark Makefile

CXX = g++
CXXFLAGS = -Ofast -std=c++17 -Wall -g
TARGET = benchmark
SRC = benchmark.cpp

all: $(TARGET)

$(TARGET): $(SRC) fastio.hpp
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *_data.txt
