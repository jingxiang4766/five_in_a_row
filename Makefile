all: gobang

gobang:
	g++ -std=c++11 main.cpp -o gobang

clean:
	rm gobang