/*
 * Author: Weir King
 * Purpose: Console Game
 * Date: 10/28/2018
 */

#ifndef __monsters__
#define	__monsters__

#include	<iostream>
#include	"tile.h"
#include	"world.h"

using std::cout;

typedef char small;

class entity{
public:
	small hp;
	small damage;
	void take_damage(small damage){
		hp -= damage;
	}
};


/*
 * Player
 * Behavior: is the player
 */
class Player: public tile, public entity{

public:
	Player(world *_home_world) : tile('@', _home_world), entity(){
		hp = 10;
		passable = false;
		updatable = false;
		ascii = "";ascii += KRED;ascii += "@";ascii += KNRM;
	}
	void update(point *self){}
	void take_command(char c, point *player);
};

void Player::take_command(char c, point *player){
	int x = player->x, y = player->y;
	switch(c){
		case 'w':
			if (home_world->map[x][y-1]->passable == true){
				home_world->move(x, y, x, y-1);
				player->x = x; player->y = y-1;

			}
			break;
		case 'a':
			if (home_world->map[x-1][y]->passable == true){
				home_world->move(x, y, x-1, y);
				player->x = x-1; player->y = y;

			}
			break;
		case 's':
			if (home_world->map[x][y+1]->passable == true){
				home_world->move(x, y, x, y+1);
				player->x = x; player->y = y+1;

			}
			break;
		case 'd':

			if (home_world->map[x+1][y]->passable == true){
				home_world->move(x, y, x+1, y);
				player->x = x+1; player->y = y;

			}
			break;
	}
}



/*
 * Slider
 * Behavior: Starts moving left. Bounces between nonpassable tiles left and right
 * HP: 2
 * Dangerous: Yes
 */
class slider: public tile, public entity{

public:
	void update(point *self);
	slider(world *home_world) : tile('S', home_world){
		passable = false;
		updatable = true;
		ascii = "";
		ascii += KGRN;
		ascii += "S";
		ascii += KNRM;
	}
	small hp = 2;
	small vx = -1;
	small damage = 1;
};

void slider::update(point *self){
	int x = self->x, y = self->y;
	if (hp <= 0){
		home_world->remove(x, y);
	}
	entity *move_to;
	if ((move_to = dynamic_cast<Player*>(home_world->map[x+vx][y]))){
		move_to->take_damage(damage);
	}
	if (home_world->map[x + vx][y]->passable == false){
		vx = -vx;
		if (home_world->map[x+vx][y]->passable == false){
			return;
		}
	}

	home_world->move(x, y, x+vx, y);
}


/*
 * Climber
 * Behavior: Bounces up and down
 * HP: 2
 * Dangerous: Yes
 */
class climber: public tile, public entity{
public:
	void update(point *self);
	climber(world *home_world) : tile('V', home_world){
		passable = true;
		updatable = true;
		ascii = "";
		ascii += KRED;
		ascii += "V";
		ascii += KNRM;
	}
	small hp = 2;
	small vy = -1;
	small damage = 1;
};

void climber::update(point *self){
	int x = self->x, y = self->y;
	if (hp <= 0){
		home_world->remove(x, y);
	}
	entity *move_to;
	if ((move_to = dynamic_cast<Player*>(home_world->map[x][y+vy]))){
		move_to->take_damage(damage);
	}
	if (home_world->map[x][y + vy]->passable == false){
		vy = -vy;
		if (home_world->map[x][y + vy]->passable == false){
			return;
		}
	}

	home_world->move(x, y, x, y + vy);
}

/*
 * Finder
 * Behavior: Moves towards the player
 * HP: 1
 * Dangerous: VERY
 */
class Finder: public tile, public entity{
private:
	point *player;
	small counter;
public:
	void update(point *self);
	Finder(world *home_world) : tile('V', home_world){
		counter = 0;
		passable = false;
		updatable = true;
		damage = 1;
		player = home_world->player;
		ascii = "";
		ascii += KBLU;
		ascii += "F";
		ascii += KNRM;
	}
};
void Finder::update(point *self){
	if (counter < 3){
		counter++;
		return;
	} else {
		counter = 0;
	}
	int x = self->x, y = self->y, vx = 0, vy = 0;
	vx = (x > player->x) ? - 1 : 1;
	vy = (y > player->y) ? - 1 : 1;

	if (abs(x-player->x) > abs(y-player->y)){
		entity *move_to;
		if ((move_to = dynamic_cast<Player*>(home_world->map[x+vx][y]))){
			move_to->take_damage(damage);
		}
		if (home_world->move(x, y, x + vx, y)){
			return;
		}
	}
	entity *move_to;
	if ((move_to = dynamic_cast<Player*>(home_world->map[x][y+vy]))){
		move_to->take_damage(damage);
	}
	if (!home_world->move(x, y, x, y + vy)){
		home_world->move(x, y, x + vx, y);
	}
	
}

#endif