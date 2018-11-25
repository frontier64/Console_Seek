#ifndef __world__
#define __world__

#include	<iostream>
#include 	"tile.h"

using std::string;

enum class states {active, won, lost};

typedef struct POINT{
	int x, y;
	struct POINT *next;
} point;

class Player;

#define WIDTH 30
#define HEIGHT 20

class world{

private:
char SELECT_UPDATE[10] = {'a', 'b', 'c', 'd', 'e', 'f', 0};
point *update_list;

public:
point *player;
Player *player_ent;
tile *map[WIDTH][HEIGHT];
states game_state = states::active;
point *victory_loc;


int player_hp();
world();
world(string filename);
~world();
bool load(string filename);
void do_step();
void add(tile *next, int x, int y);
void remove(int x, int y);
bool move(int x, int y, int to_x, int to_y);
void add_updater(int x, int y);
void player_command(char c);
tile*& get(point *p);
tile*& operator[](point *p);
//tile*& operator[](int x);
};

#endif