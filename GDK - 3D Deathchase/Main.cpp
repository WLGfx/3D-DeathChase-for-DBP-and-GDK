#include "main.h"

std::string gstring;	// something about g-strings ;-)

// This _global structure is the entire game!!!
// All other classes are initialised from this.
_gglobal *gg;

void DarkGDK ( void )
{
	gg = new _gglobal;

	int flag;	// Value returned by the menu

	// setup the screen resolution (init now in class)
	//gg->init_screen(800,600,0x0040ff);
	dbDisableEscapeKey();

	while ( 1 )	// infinite loop
	{
		// initialise the menu
		menu_init();
		//init_test();

		flag = 0;

		// run the menu until a selection has been made
		while ( flag == 0 )
		{
			if ( !LoopGDK() )
				flag = -1;
			else
				flag = menu_run();

			//run_test();

			dbSync();
			
		}

		//close_test();

		// shut down all menu entities
		menu_close();

		if ( flag == -1 )
		{
			// exit back to windows because Escape Key was pressed
			delete gg;
			return;
		}

		// Set games view and fog distance
		dbSetCameraRange(1,500);
		dbFogDistance(500);

		// altering the main fog and background colour
		gg->main_colour = 0x000000;
		gg->level.level = flag;		// return value from menu selection
		
		gg->init_game();

		// re-use flag for game loop
		flag = 0;

		while ( flag == 0 )
		{
			// MAIN GAME LOOP
			if ( !LoopGDK() )
				flag = -1;
			else
				flag = gg->bike.do_all();	// this should go in the global run

			gg->run_game();
			gg->display_fps();

			dbSync ( );
		}

		gg->close_game();

	};	// EOF - do {

	
}