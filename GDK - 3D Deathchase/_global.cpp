#include "main.h"

_gglobal::_gglobal(void)
{
	// do nothing please Mr. Compiler (Okay now it does stuff)

	// Okay init the screen for me
	init_screen(800, 600, 0x000000);

	// And load up all the resources
	trees.load();
	bike.load();
	player.health_init(screen_width, screen_height);
	matrix.load();
	pickups.load();
	sounds.load();
	numbers.load();
}

_gglobal::~_gglobal(void)
{
	// do nothing please Mr. Compiler
}

void _gglobal::display_fps() {
	// Might change this to 'NOT' use <string>
	/*gstring.clear();
	gstring.assign("FPS : "); 
	gstring.append(dbStr(dbScreenFPS()));
	dbText(0,0,(char*)gstring.c_str());*/
	static char fps[] = "FPS: 000";
	sprintf( &fps[5], "%d", dbScreenFPS() );
	dbText(0,0,fps);
}

void _gglobal::init_screen(int sw, int sh, int colour)
{
	// do some of my extra stuff
	InitCommonControls();

	// setup the screen and stuff
	dbSetBitmapFormat( 21 );
	dbSyncOn   ( );
	dbSyncRate ( 60 );
	dbSetDisplayMode(sw, sh, 32);
	dbSync();

	// Tell user I'm busy loading up defaults
	char intro[] = "Initialising all Game data";
	dbText( sh / 2, ( sw / 2 )-( dbTextWidth( intro ) / 2 ), intro );
	dbSync();

	screen_width = sw;
	screen_height = sh;
	//dbSetWindowOff();
	dbAutoCamOff();
	dbPositionCamera(0,200,0);
	dbPointCamera(0,0,500);
	dbMakeLight(1);
	dbPositionLight(1, 0, 200, 0); 
	dbSetLightRange(1, 2000);
	//dbSetSpotLight(1, 0, 45);
	dbColorLight(1, 0xffffff);
	dbHideLight(0);
	dbSetAmbientLight(20);
	dbColorLight(0,0x800000);
	dbFogOn();
	dbFogDistance(300);
	dbFogColor(colour);
	dbSetNormalizationOn();
	dbColorBackdrop(colour);
	dbSetCameraRange(1,300);

	// This changes when player hits a tree
	main_colour = colour;
	// I think I'm done with the main stuff
}

void _gglobal::init_game()
{
	// start the music here instead
	gg->music.load_and_play(1);

	// initialise the level to the settings selected
	gg->matrix.init(128, 128, 2048, 2048, gg->level.tree_count, gg->level.octaves);
	gg->bike.init_all( gg->level.bike_speed );

	// setup the pickups on the level
	gg->pickups.init();

	// put these here cos I found that the player_init() was the wrong one. Oops!
	gg->player.health = 100;
	gg->player.fuel = 100;
	gg->game_over = 0;
	gg->player.stars_count = 0;
	gg->player.health_count = 0;
	gg->player.fuel_count = 0;

	// the game over flag
	gg->game_over = 0;
}

void _gglobal::close_game()
{
	// Shut down the game entities and thingies
	gg->matrix.close();
	gg->pickups.close();
	gg->music.stop();
}

void _gglobal::run_game()
{
	// test the NUMBERS on the hud
	//numbers.paste_number( 3, player.hudimg, 16, 160 );

	if ( !game_over )
	{
		// run the game as normal
		gg->pickups.face_camera();
		gg->pickups.collide();
		gg->matrix.position_water();
	}
	else
		show_game_over();
}

void _gglobal::show_game_over()
{
	player.game_over();

}
