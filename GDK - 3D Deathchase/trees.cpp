#include "main.h"

using namespace std;

TREES::TREES()
{
	trees_xy.reserve(12000);
}

TREES::~TREES()
{
	close();
}

string tree_files[] = {
	"media\\tree1.dds",	"media\\tree2.dds",
	"media\\tree3.dds",	"media\\tree4.dds",
	"media\\tree5.dds",	"media\\tree6.dds",
	"media\\tree7.dds",	"media\\tree8.dds",
	"media\\tree9.dds",	"media\\tree10.dds" };

void TREES::load()
{
	// load in the tree images and setup the tree objects
	int a;
	int t;

	//dbCLS(); dbPrint("Loading Trees....."); dbSync();

	dbLoadObject( "media\\treebillboard.x", t = free_object() );

	for ( a=0; a<10; a++)
	{
		dbLoadImage( (char*)tree_files[a].c_str(), imgID[a] = free_image() );
		dbCloneObject( objID[a] = free_object(), t );
		dbTextureObject( objID[a], imgID[a] );
		dbSetObjectTransparency( objID[a], 6 );
		dbHideObject( objID[a] );
	}
	dbDeleteObject( t );
	// clear the trees collision flag
	coll = 0;
}

void TREES::place(int num)
{
	// places count trees onto the map and store an array of coords
	int a;
	int o, x, y, z;
	int t;

	MATRIX *mat = &gg->matrix;
	int matwid = mat->matwid;
	int mathgt = mat->mathgt;

	// set allocation ready (speed up)
	trees_xy.clear();

	// store the count of trees
	count = num;

	for ( a=0; a<num; a++ )
	{
		do {
			// make sure tree is above water level
			x = rand() % matwid;
			z = rand() % mathgt;
			y = dbGetGroundHeight( 1, x, z );
		} while ( y < 100 );

		// locate a free object first
		o = free_object();

		// store the trees coords into the collision array
		x -= mat->mathalfx;
		z -= mat->mathalfy;
		trees_xy.push_back( TREEXY( o, x, y, z ) );

		// based on the height choose that tree (100-400 height left)
		t = (y - 100) / 10;
		if ( t>9 ) t=9;

		// now instance the object and position it on the map
		dbInstanceObject( o, objID[t] );
		dbPositionObject( o, x, y, z );
		dbYRotateObject( o, rand()%360 );
		dbShowObject( o );
	}
}

void TREES::unplace()
{
	int a;
	for ( a=0; a<count; a++ )
	{
		// clear out all the trees instanced on the map
		dbDeleteObject( trees_xy[a].o );
	}
	// empty out the array
	trees_xy.clear();
	count = 0;
}

void TREES::close()
{
	// free up the resources used by the trees. (images and objects)
	int a;
	unplace();
	for ( a=0; a<10; a++ )
	{
		// clear out the original objects and images
		dbDeleteObject( objID[a] );
		dbDeleteImage( imgID[a] );
	}
	// now free up the arrays
	trees_xy.clear();
}
void TREES::left()
{
	// move the trees when the terrain gets shifted
	int a;
	MATRIX *mat = &gg->matrix;
	vector<TREEXY>::iterator ptr = trees_xy.begin();
	for ( a=0; a<count; a++ )
	{
		ptr->x -= mat->matxs;
		if ( ptr->x < -mat->mathalfx )
			ptr->x += mat->matwid;
		dbPositionObject( ptr->o, ptr->x, ptr->y, ptr->z );
		ptr++;
	}
}
void TREES::right()
{
	int a;
	MATRIX *mat = &gg->matrix;
	vector<TREEXY>::iterator ptr = trees_xy.begin();
	for ( a=0; a<count; a++ )
	{
		ptr->x += mat->matxs;
		if ( ptr->x > mat->mathalfx )
			ptr->x -= mat->matwid;
		dbPositionObject( ptr->o, ptr->x, ptr->y, ptr->z );
		ptr++;
	}
}
// These were correct before changing them (in order of up / down)
void TREES::up()
{
	int a;
	MATRIX *mat = &gg->matrix;
	vector<TREEXY>::iterator ptr = trees_xy.begin();
	for ( a=0; a<count; a++ )
	{
		ptr->z += mat->matys;
		if ( ptr->z > mat->mathalfy )
			ptr->z -= mat->mathgt;
		dbPositionObject( ptr->o, ptr->x, ptr->y, ptr->z );
		ptr++;
	}
}
void TREES::down()
{
	int a;
	MATRIX *mat = &gg->matrix;
	vector<TREEXY>::iterator ptr = trees_xy.begin();
	for ( a=0; a<count; a++ )
	{
		ptr->z -= mat->matys;
		if ( ptr->z < -mat->mathalfy )
			ptr->z += mat->mathgt;
		dbPositionObject( ptr->o, ptr->x, ptr->y, ptr->z );
		ptr++;
	}
}

int TREES::find_closest()
{
	int a=0;
	int curr_dist = 8000000;
	register int xd, yd, dist;

	// optimise the bikes positions for local vars and using ints...
	int bx = (int)gg->bike.xpos;
	int by = (int)gg->bike.ypos;

	vector<TREEXY>::iterator p = trees_xy.begin();

	while ( a < count )
	{
		// use the iterator and get the distance
		xd = bx - p->x;
		yd = by - p->z;		// z and not y

		dist = ( xd * xd ) + ( yd * yd );

		if ( dist < curr_dist )
		{
			// store the current closest distance
			curr_dist = dist;
		}
		p++;	// next iterator position
		a++;
	}
	// return the distance of the closest tree
	return (int)( ( sqrt( (float)curr_dist ) ) );
}

bool TREES::check_collision(int dist)
{
	BIKE *bike = &gg->bike;
	bool ret=0;
	if ( dist < 2 )
	{
		// set the timer for the background colour change
		coll = 2;

		// if collided then spin the bike out of control a bit
		if ( bike->turnspd < 0 )
			bike->turnspd = 4.0;
		else if ( bike->turnspd > 0 )
			bike->turnspd = -4.0;
		else if ( rand() & 1 )
			bike->turnspd = -4.0;
		else
			bike->turnspd = 4.0;

		ret=1;
	}
	if ( coll > 0 )
	{
		// this will get change to add sparks particles
		dbFogColor(0xff0000);
		dbColorBackdrop(0xff0000);

		// collision counter
		coll--;
	}
	else
	{
		// otherwise use the main background colour
		dbFogColor(gg->main_colour);
		dbColorBackdrop(gg->main_colour);
	}
	return ret;
}