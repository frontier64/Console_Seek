#include 	"world.h"
#include	"monsters.h"
#include	<fstream>

using std::ifstream;
using std::cout;

tile*& world::get(point *p){
	if (p->x >= 0 and p->x < WIDTH and p->y >= 0 and p->y < HEIGHT){
		return map[p->x][p->y];
	}
	return map[0][0];
}

tile*& world::operator[](point *p){
	if (p->x >= 0 and p->x < WIDTH and p->y >= 0 and p->y < HEIGHT){
		return map[p->x][p->y];
	}
	return map[0][0];
}
/*
//This doesn't work.
tile*& world::operator[](int x){
	return *(map[x]);
}
*/
world::world(){
	int i, j;
	for (i = 0; i < WIDTH; i++){
		for (j = 0; j < HEIGHT; j++){
			map[i][j] = new tile(0, this);
		}
	}
}
world::world(string filename){
	update_list = NULL;
	player = new point;
	player->next = NULL;
	if (!load(filename)){
		cout << "loading world failed\n";
	}
}

bool world::load(string filename){
	ifstream is(filename);
	int x, y;
	char c;
	tile *new_tile;
	point *next_updater;

	for (y = 0; y < HEIGHT; y++){
		for (x = 0; x < WIDTH; x++){
			if (!is.get(c)){
				return false;
			} else {
				switch(c){

//Player
					case '@':
						player->x = x;
						player->y = y;
						player_ent = new Player(this);
						new_tile = player_ent;
						break;
//Victory
					case '&':
						new_tile = new tile('&', this);
						new_tile->passable = true;
						victory_loc = new point;
						*victory_loc = {x, y, NULL};					
						break;
//Slider						
					case 'S':
						new_tile = new slider(this);
						break;
//Climber
					case 'V':
						new_tile = new climber(this);
						break;

//Finder
					case 'F':
						new_tile = new Finder(this);
						break;
//Default, now for walls and floors
					default:
						new_tile = new tile(c, this);
				}

//Add the tile to the map and do the update list if it should be updated
				this->map[x][y] = new_tile;
				if (new_tile->updatable){
					next_updater = new point;
					next_updater->x = x; next_updater->y = y;
					next_updater->next = update_list;
					update_list = next_updater;
				}
			}
		}
		is.get(c);
	}
	return true;
}

void world::do_step(){
	point *temp, *next;
	int i = 0;
//Remove nonupdaters
	while (update_list != NULL and map[update_list->x][update_list->y]->updatable == false){
		temp = update_list;
		update_list = update_list->next;
		free(temp);
	}
	temp = update_list;
	if (temp != NULL){
		next = temp->next;
	} else {
		next = NULL;
	}
	while (next != NULL){
		if (map[next->x][next->y]->updatable == false){
			temp->next = next->next;
			free(next);
			next = temp->next;
		} else {
			temp = next;
			next = temp->next;
		}
	}
//Do Updates
	temp = update_list;
	while (temp != NULL){
		i++;
		next = temp->next;
		map[temp->x][temp->y]->update(temp);
		temp = next;
	}

//Check game state changes
	if (player->x == victory_loc->x and player->y == victory_loc->y){
		game_state = states::won;
	}
	if (player_ent->hp <= 0){
		game_state = states::lost;
	}
}

void world::add_updater(int x, int y){
	//sanity check
	if (map[x][y]->updatable == false){
		return;
	}
	//cout << "added updater: " << map[x][y]->ascii << " x:" << x << " y:" << y << "\n";
	point *new_updater = new point;
	new_updater->x = x; new_updater->y = y;
	new_updater->next = update_list;
	update_list = new_updater;

}
void world::add(tile *next, int x, int y){
	free(map[x][y]);
	map[x][y] = next;
	if (next->updatable){
		add_updater(x, y);
	}
}

void world::remove(int x, int y){
	delete map[x][y];
	map[x][y] = new tile(0, this);
}
bool world::move(int x, int y, int to_x, int to_y){
	if (map[to_x][to_y]->passable == false){
		if (player->x == to_x and player->y == to_y){
			return true;
		}
		return false;
	}
	tile *temp;
	temp = map[to_x][to_y];

	map[to_x][to_y] = map[x][y];
	map[x][y] = temp;
	if (map[to_x][to_y]->updatable){
		add_updater(to_x,to_y);
	}

	if (map[x][y]->updatable){
		add_updater(x,y);
	}
	return true;
}

void world::player_command(char c){
	Player *p = dynamic_cast<Player*>(map[player->x][player->y]);
	if (p){
		p->take_command(c, player);
	} else {
		exit(0);
	}
}

int world::player_hp(){
	Player *p = dynamic_cast<Player*>(this->get(player));
	return p->hp;
}

world::~world(){
	point *temp;
	while (update_list!= NULL){
		temp = update_list;
		update_list = update_list->next;
		delete temp;
	}
	int x, y;
	for (x = 0; x < WIDTH; x++){
		for (y = 0; y < HEIGHT; y++){
			delete map[x][y];
		}
	}
	delete player;
}
