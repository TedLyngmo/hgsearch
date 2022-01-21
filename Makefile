CXX = g++
STD = -std=c++20
OPTS = $(STD) -O3 -Wall -Wextra -pedantic-errors
HEADERS = src/cppmain.hpp src/main.hpp src/runner.hpp src/tsv.hpp
OBJS = build/main.o build/tsv.o

all: bin/hgsearch bin/hgsearchmm

bin/hgsearch: build/hgsearch.o $(OBJS)
	g++ -o $@ $< $(OBJS) -ltbb

bin/hgsearchmm: build/hgsearchmm.o $(OBJS)
	g++ -o $@ $< $(OBJS) -lboost_iostreams -ltbb

build/%.o : src/%.cpp $(HEADERS) | build
	$(CXX) -o $@ $< -c $(OPTS) -DNDEBUG

build:
	mkdir build

format:
	clang-format -i src/*.?pp

clean:
	rm -rf bin/hgsearch bin/hgsearchmm build
