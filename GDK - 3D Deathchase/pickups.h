#pragma once

//
// used by the pickups .cpp file
//

struct OBJ {
	int		obj;
	int		x, y, z;
	//OBJ() {};
	//OBJ(int o, int _x, int _y, int _z)
	//{ obj = obj; x = _x; y = _y; z = _z; }
};

class PICKUPS {
public:
	int		star_img;		// image ID's
	int		fuel_img;
	int		health_img;

	int		star_obj;		// Object ID's
	int		fuel_obj;
	int		health_obj;

	int		max;			// max objs on the level

	//vector<OBJ>	stars;	// sacked the slow vector lists off
	//vector<OBJ>	fuel;
	//vector<OBJ>	health;

	OBJ*	stars;			// Arrays to hold positions of the maps pickups
	OBJ*	fuel;
	OBJ*	health;

			PICKUPS();
			~PICKUPS();

	void	load();			// load images and setup objects
	void	unload();		// delete images and main objects
	void	init();			// initialise the maps instanced objects
	void	close();		// close down the maps objects
	void	face_camera();	// face all objects towards to camera

	int		mw, mh, mxs, mys, mhx, mhy;	// speed up object movement (optimised)

	void	obj_left(OBJ *o);	// move the objects across the matrix cells
	void	obj_right(OBJ *o);
	void	obj_up(OBJ *o);
	void	obj_down(OBJ *o);

	void	move_left();		// move all objects across the cells
	void	move_right();
	void	move_up();
	void	move_down();

	void	instance(OBJ* obj, int i);	// sets up a new object on the map

	void	collide();			// checks all lists collision
	int		find_collision(OBJ* list);	// checks one lists collision
};
