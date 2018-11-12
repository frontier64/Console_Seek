cxx = c++
cxxflags = -std=c++11
debugflags = -std=c++11 -ggdb

game: world.h world.cc main.cc tile.h tile.cc monsters.h
	$(cxx) $(cxxflags) main.cc world.cc tile.cc -o game
debug: world.cc main.cc tile.cc
	$(cxx) $(debugflags) main.cc world.cc tile.cc -o db_game