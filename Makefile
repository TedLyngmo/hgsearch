all: hgsearch hgsearchmm

hgsearch: src/hgsearch.cpp
	g++ -o $@ $< -std=c++20 -O3 -ltbb

hgsearchmm: src/hgsearchmm.cpp
	g++ -o $@ $< -std=c++20 -O3 -lboost_iostreams -ltbb

format:
	clang-format -i src/*.cpp

clean:
	rm -f hgsearch hgsearchmm
