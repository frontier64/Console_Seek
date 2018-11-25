#include 	<iostream>
#include	<chrono>
#include	<thread>
#include	<string>
#include	<unistd.h>
#include	<termios.h>

#include 	"world.h"

using std::cin;
using std::cout;

struct termios terminalSettings;
void input_on()
{
    struct termios newTerminalSettings;

    tcgetattr( STDIN_FILENO, &terminalSettings );

    newTerminalSettings = terminalSettings;

    newTerminalSettings.c_lflag &= ~( ICANON | ECHO );
    newTerminalSettings.c_cc[VTIME] = 0;
    newTerminalSettings.c_cc[VMIN] = 1;

    tcsetattr( STDIN_FILENO, TCSANOW, &newTerminalSettings );
}

void input_off()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &terminalSettings);
}

char last_c;
world *home_world;

void draw();

int main(){
	home_world = new world("test_map.map");
	input_on();
	timeval timeout;
	timeout.tv_usec = 1000 * 200;
	timeout.tv_sec = 0;
	fd_set rfds;
	last_c = 'a';
	int fd_result;
	bool running = true;
	while (home_world->game_state == states::active and running){
		draw();
		FD_ZERO(&rfds);
		FD_SET(STDIN_FILENO, &rfds);

		fd_result = select(1, &rfds, NULL, NULL, &timeout);
		if (FD_ISSET(STDIN_FILENO, &rfds)){
			last_c = getchar();
			if (last_c == 'q'){
				running = false;
			}
			home_world->player_command(last_c);

		}

		home_world->do_step();

	}

	if (home_world->game_state == states::won){
		system("clear");
		printf("\n\n\n\n\n%s\n\n\n\n\n", "YOU WON THE FREAKING GAME GZ!!!!!!");
	} else if (home_world->game_state == states::lost){
		system("clear");
		printf("\n\n\n\n\n%s\n\n\n\n\n", "HOLY SHIT YOU ACTUALLY LOST THE GAME WHATALOSERLMAO :( ");		
	}
	delete home_world;
	input_off();
}


void draw(){
	system("clear");
	int x, y;
	for (y = 0; y < HEIGHT; y++){
		for (x = 0; x < WIDTH; x++){
			printf("%s", home_world->map[x][y]->ascii.c_str());
		}
		putchar('\n');
	}
	putchar('\n');
	printf("%s%d\n", "HP: ", home_world->player_hp());
}