all: game

game:
	g++ -std=c++11 main.cpp -o game

clean:
	rm game