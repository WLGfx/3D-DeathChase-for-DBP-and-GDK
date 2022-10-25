#pragma once

using namespace std;

// an array to store the x and z for the trees
class TREEXY {
public:
	int o, x, y, z;
	TREEXY() {};
	TREEXY(int _o, int _x, int _y, int _z) { o=_o; x=_x; y=_y; z=_z; };
};

class TREES
{
public:
	int			imgID[10];	// tree image ID's
	int			objID[10];	// tree object ID's
	
	// tree collision flag
	int			coll;		// Counter for the collision with trees BG colour

	int			count;		// number of trees on the map

	vector<TREEXY> trees_xy;

				TREES();
				~TREES();
	void		load();
	void		place(int num);
	void		unplace();
	void		close();
	void		left();
	void		right();
	void		up();
	void		down();
	int			find_closest();
	bool		check_collision(int dist);
};
