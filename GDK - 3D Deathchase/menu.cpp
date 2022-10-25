#include "main.h"

// I started the menu before I started throwing everything
// into their own seperate classes. I never got round to doing
// it for the menu.

// So as a result here's some static globals for the menu.

static float menu_cam_pos;

static int menu_obj;
static int menu_img;

static float menuax, menuay, menuaz;
static float menuix, menuiy, menuiz;


void menu_init()
{
	// setup map for background display on the menu
	gg->matrix.init(128, 128, 2048, 2048, 5000, 6);

	// setup some vars
	menu_cam_pos = 0.0;

	menuax = menuay = menuaz = 0;
	menuix = 1.5;
	menuiy = 1.7;
	menuiz = 2.1;

	dbSetCameraRange(1,500);
	dbFogDistance(500);
	dbFogColor(0x000040);
	dbColorBackdrop(0x000040);

	dbLoadImage( "media\\mainmenu.dds", menu_img = free_image(), 6 );
	dbMakeObjectPlane( menu_obj = free_object(), 8, 6 );
	dbTextureObject( menu_obj, menu_img );
	dbSetObjectTransparency( menu_obj, 1 );

	// test the pickups on the menu
	/*dbShowObject( gg->pickups.fuel_obj );*/

	gg->music.load_and_play(0);
}

void menu_close()
{
	gg->matrix.close();

	dbDeleteObject( menu_obj );
	dbDeleteImage( menu_img );

	gg->music.stop();
}

int menu_run()
{
	float x, y, z;
	float x2, y2, z2;

	LEVEL *level = &gg->level;

	x = z = 0;
	y = dbGetGroundHeight( 1, x + gg->matrix.mathalfx, z + gg->matrix.mathalfy ) + 50;

	dbPositionCamera( x, y, z );
	dbRotateCamera( 30, 0, 0 );
	dbYRotateCamera( menu_cam_pos );

	menu_cam_pos = dbWrapValue( menu_cam_pos + 0.5 );

	x = dbCameraPositionX();
	y = dbCameraPositionY();
	z = dbCameraPositionZ();

	dbPositionObject( menu_obj, x, y, z );
	dbSetObjectToCameraOrientation( menu_obj );
	dbMoveObject( menu_obj, 7 );
	x2 = dbObjectPositionX( menu_obj );
	y2 = dbObjectPositionY( menu_obj );
	z2 = dbObjectPositionZ( menu_obj );
	dbMoveObject( menu_obj, -2 );
	dbPointObject( menu_obj, x, y, z );

	

	dbPositionLight( 1, x, y, z );

	dbPositionObject( gg->bike.obj, x2, y2, z2 );

	// test a pickup item
	/*dbPositionObject( gg->pickups.fuel_obj, x, y, z );
	dbSetObjectToCameraOrientation( gg->pickups.fuel_obj );
	dbMoveObject( gg->pickups.fuel_obj, 4 );
	dbPointObject( gg->pickups.fuel_obj, x, y, z );*/

	x = dbWrapValue( dbObjectAngleX( menu_obj ) + dbSin( menuax ) * 10 );
	y = dbWrapValue( dbObjectAngleY( menu_obj ) + dbSin( menuay ) * 10 );
	z = dbWrapValue( dbObjectAngleZ( menu_obj ) + dbSin( menuaz ) * 10 );

	dbRotateObject( menu_obj, x, y, z );

	menuax = dbWrapValue( menuax + menuix );
	menuay = dbWrapValue( menuay + menuiy );
	menuaz = dbWrapValue( menuaz + menuiz );

	if ( dbKeyState(2) )	// key 1
	{
		// set level 1
		level->bike_speed = 2;
		level->octaves = 3;
		level->tree_count = 4000;
		return 1;
	}
	if ( dbKeyState(3) )	// key 2
	{
		// set level 2
		level->bike_speed = 2.5;
		level->octaves = 4;
		level->tree_count = 5000;
		return 2;
	}
	if ( dbKeyState(4) )	// key 3
	{
		// set level 3
		level->bike_speed = 3;
		level->octaves = 5;
		level->tree_count = 6000;
		return 3;
	}
	if ( dbKeyState(5) )	// key 4
	{
		// set level 4
		level->bike_speed = 3.5;
		level->octaves = 5;
		level->tree_count = 7000;
		return 4;
	}
	if ( dbKeyState(6) )	// key 5
	{
		// set level 5
		level->bike_speed = 4;
		level->octaves = 6;
		level->tree_count = 8000;
		return 5;
	}
	if ( dbEscapeKey() )	// ESCAPE key
	{
		return -1;
	}

	return 0;
}