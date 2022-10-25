#include "main.h"

//
// I've decided to put this into a separate file because the collapsed functions
// just keep on re-openeing.
//

using namespace std;

PICKUPS::PICKUPS()
{
	// Don't do nothing yet Mr. Compiler
}

PICKUPS::~PICKUPS()
{
	// On exit then unload the main objects
	unload();
}

void PICKUPS::load()
{
	// load up the original objects and hide them for later

	static int size = 4;

	//dbPrint("Loading Pickups....."); dbSync();
	// load the media used by the pickups

	dbLoadImage( "media\\gascan.dds",	fuel_img = free_image() );
	dbLoadImage( "media\\goldstar.dds",	star_img = free_image() );
	dbLoadImage( "media\\heart.dds",	health_img = free_image() );

	dbMakeObjectPlane		( fuel_obj = free_object(), size, size );
	dbTextureObject			( fuel_obj, fuel_img );
	dbSetObjectTransparency	( fuel_obj, 1 );
	dbSetObjectCull			( fuel_obj, 0 );
	dbHideObject			( fuel_obj );

	dbMakeObjectPlane		( health_obj = free_object(), size, size );
	dbTextureObject			( health_obj, health_img );
	dbSetObjectTransparency	( health_obj, 1 );
	dbSetObjectCull			( health_obj, 0 );
	dbHideObject			( health_obj );

	dbMakeObjectPlane		( star_obj = free_object(), size, size );
	dbTextureObject			( star_obj, star_img );
	dbSetObjectTransparency	( star_obj, 1 );
	dbSetObjectCull			( star_obj, 0 );
	dbHideObject			( star_obj );

	// That bit was easy, now to setup the vars for the game
}

void PICKUPS::unload()
{
	// remove the pickups stuff (the original objects)
	dbDeleteImage( fuel_img );
	dbDeleteImage( health_img );
	dbDeleteImage( star_img );

	dbDeleteObject( fuel_obj );
	dbDeleteObject( health_obj );
	dbDeleteObject( star_obj );
}

void PICKUPS::init()
{
	// initialise the pickups for the game
	//int level = gg->level.level;

	// Set the max objects on the level
	max = 50;

	stars	= new OBJ[max];		// allocate memory for maps pickups
	fuel	= new OBJ[max];
	health	= new OBJ[max];

	int a;
	int o, x, y, z;

	mw = gg->matrix.matwid;		// copy over for speed up (localises vars)
	mh = gg->matrix.mathgt;
	mxs= gg->matrix.matxs;
	mys= gg->matrix.matys;
	mhx= gg->matrix.mathalfx;
	mhy= gg->matrix.mathalfy;

	int mw2 = gg->matrix.mathalfx;	// should really need these
	int mh2 = gg->matrix.mathalfy;

	for ( a=0; a<max; a++ )		// place all the maps pickups
	{
		instance( &stars[a], star_obj );
		instance( &fuel[a], fuel_obj );
		instance( &health[a], health_obj );
	}
}

void PICKUPS::instance( OBJ* obj, int i)
{
	// place a copy of the original into the game on the map
	int		x, y, z, o;

	do {
		x = rand() % mw;
		z = rand() % mh;
		y = dbGetGroundHeight( 1, x, z );
	} while ( y < 100 );	// make sure its not under water

	x -= mhx;		// alter position to half the maps wid and hgt
	y += 3;
	z -= mhy;

	o = free_object();		// find a free object ID

	dbInstanceObject( o, i );	// copy and place it
	dbShowObject( o );
	dbPositionObject( o, x, y, z );

	obj->obj	= o;	// store in the main array
	obj->x		= x;
	obj->y		= y;
	obj->z		= z;
}

void PICKUPS::close()
{
	// just delete the games objects and arrays

	int a;

	for ( a=0; a<max; a++ )
	{
		dbDeleteObject( stars[a].obj );
		dbDeleteObject( fuel[a].obj );
		dbDeleteObject( health[a].obj );
	}

	delete[] stars;
	delete[] fuel;
	delete[] health;
}

void PICKUPS::face_camera()
{
	int a;

	//int bx = gg->bike.xpos;		// get current bikes position
	//int bz = gg->bike.ypos;
	//int by = gg->bike.height;
	int bx = dbCameraPositionX();	// face the camera instead
	int by = dbCameraPositionY();
	int bz = dbCameraPositionZ();

	for ( a=0; a<max; a++ )
	{
		dbPointObject( stars[a].obj, bx, by, bz );
		dbPointObject( fuel[a].obj, bx, by, bz );
		dbPointObject( health[a].obj, bx, by, bz );
	}
}

//
// These next functions do the same as the movement of the trees.
// If the players bike crosses the cell boundary in any direction
// these functions will shift the position of the pickups.
// If they go over the entire boundary of the map then it will
// be placed on the opposite side.
//

void PICKUPS::move_left()
{
	// move all pickups across the map cells
	int a;
	for ( a=0; a<max; a++ )
	{
		obj_left( &stars[a] );
		obj_left( &fuel[a] );
		obj_left( &health[a] );
	}
}
void PICKUPS::move_right()
{
	int a;
	for ( a=0; a<max; a++ )
	{
		obj_right( &stars[a] );
		obj_right( &fuel[a] );
		obj_right( &health[a] );
	}
}
void PICKUPS::move_up()
{
	int a;
	for ( a=0; a<max; a++ )
	{
		obj_up( &stars[a] );
		obj_up( &fuel[a] );
		obj_up( &health[a] );
	}
}
void PICKUPS::move_down()
{
	int a;
	for ( a=0; a<max; a++ )
	{
		obj_down( &stars[a] );
		obj_down( &fuel[a] );
		obj_down( &health[a] );
	}
}
void PICKUPS::obj_left(OBJ* o)
{
	o->x -= mxs;		// cell width
	if ( o->x < -mhx )	// over the maps extents?
		o->x += mw;		// loop to the other side of map
	dbPositionObject( o->obj, o->x, o->y, o->z );
}
void PICKUPS::obj_right(OBJ *o)
{
	o->x += mxs;
	if ( o->x > mhx )
		o->x -= mw;
	dbPositionObject( o->obj, o->x, o->y, o->z );
}
void PICKUPS::obj_up(OBJ *o)
{
	o->z += mys;
	if ( o->z > mhy )
		o->z -= mh;
	dbPositionObject( o->obj, o->x, o->y, o->z );
}
void PICKUPS::obj_down(OBJ *o)
{
	o->z -= mys;
	if ( o->z < -mhy )
		o->z += mh;
	dbPositionObject( o->obj, o->x, o->y, o->z );
}

//
// Also similar to the trees collision algorithm.
// The trees version is so much faster though.
//

void PICKUPS::collide()
{
	// check if any objects collide with the bike and process it
	int c;

	// check stars collision
	c = find_collision( stars );
	if ( c >= 0 )	// index of object or -1
	{
		// Add score and reposition the object
		dbDeleteObject( stars[c].obj );
		instance( &stars[c], star_obj );
		gg->game_score += 100;
		// play the star hit sound
		gg->sounds.play( SOUNDS_STARHIT );
		// increase the stars hit counter
		gg->player.stars_count++;
	}
	// Check health collision
	c = find_collision( health );
	if ( c >= 0 )	// index of object or -1
	{
		// Increase health and reposition object
		dbDeleteObject( health[c].obj );
		instance( &health[c], health_obj );
		gg->player.health += 25;
		if ( gg->player.health > 100 )
			gg->player.health = 100;
		// play the health hit sound
		gg->sounds.play( SOUNDS_HEALTHHIT );
		// increase the health hit counter
		gg->player.health_count++;
	}
	// check collision with fuel cans
	c = find_collision( fuel );
	if ( c >= 0 )
	{
		gg->sounds.play( SOUNDS_FUELHIT );	// play sound
		dbDeleteObject( fuel[c].obj );		// change pos of item
		instance( &fuel[c], fuel_obj );
		// now process the fuel gauge
		gg->player.fuel += 50;
		if ( gg->player.fuel > 100 )
			gg->player.fuel = 100;
		// increase the hit counter
		gg->player.fuel_count++;
	}
}

int PICKUPS::find_collision(OBJ *list)
{
	int c = 0;

	int bx = gg->bike.xpos;		// grab these locally
	int by = gg->bike.ypos;

	int dx, dy, r;

	while ( c<max )
	{
		dx = bx - list[c].x;	// simple optimisation for cache friendliness
		dx *= dx;
		dy = by - list[c].z;
		dy *= dy;
		r = dx + dy;
		if ( fsqrt(r) < 3 )		// the fsqrt() is inline and using not yet using ASM
			return c;
		c++;
	}
	return -1;
}