#ifndef __tile__
#define __tile__

#include 	<string>

using std::string;

//Colors
#define KNRM		"\x1B[0m"
#define KRED		"\x1B[31m"
#define KGRN		"\x1B[32m"
#define KYEL		"\x1B[33m"
#define KBLU		"\x1B[34m"
#define KMAG		"\x1B[35m"
#define KCYN		"\x1B[36m"
#define KWHT  		"\x1B[37m"

class world;
struct POINT;
class tile{
protected:
	world *home_world;

public:
	bool passable = true;
	bool updatable = false;
	string ascii;

	tile(char kind, world *home_world);
	virtual void update(POINT *self){}
};

#endif

