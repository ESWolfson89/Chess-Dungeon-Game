comp=g++
flags=-std=c++11 -O2
FLLF=-lSDL -lSDL_ttf

chessdungeon: main.o being.o cell.o game.o graphics.o grid.o level.o message.o point.o pathfinding.o io.o
	$(comp) $(flags) main.o being.o cell.o game.o graphics.o grid.o level.o message.o point.o pathfinding.o io.o $(FLLF) -o chessdungeon
main.o: main.cpp
	$(comp) $(flags) -c main.cpp -o main.o
game.o: game.cpp game.h
	$(comp) $(flags) -c game.cpp -o game.o
graphics.o: graphics.cpp graphics.h globals.h
	$(comp) $(flags) -c graphics.cpp -o graphics.o
grid.o: grid.cpp grid.h globals.h
	$(comp) $(flags) -c grid.cpp -o grid.o
cell.o: cell.cpp cell.h globals.h MersenneTwister.h
	$(comp) $(flags) -c cell.cpp -o cell.o
level.o: level.cpp level.h globals.h MersenneTwister.h
	$(comp) $(flags) -c level.cpp -o level.o
message.o: message.cpp message.h globals.h MersenneTwister.h
	$(comp) $(flags) -c message.cpp -o message.o
being.o: being.cpp being.h globals.h MersenneTwister.h
	$(comp) $(flags) -c being.cpp -o being.o
point.o: point.cpp point.h
	$(comp) $(flags) -c point.cpp -o point.o
pathfinding.o: pathfinding.cpp pathfinding.h
	$(comp) $(flags) -c pathfinding.cpp -o pathfinding.o
io.o: io.cpp io.h
	$(comp) $(flags) -c io.cpp -o io.o

.PHONY: clean

clean:
	rm chessdungeon *.o






