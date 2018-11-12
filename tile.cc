#include "world.h"

tile::tile(char kind, world *_home_world){
	home_world = _home_world;
	ascii = kind;
	switch(kind){
		case '.':
		case ' ':
			passable = true;
			ascii = ' ';
			break;
		case '|':
			ascii = '|';
			passable = false;
			break;
		default:
			ascii = kind;
	}
}

