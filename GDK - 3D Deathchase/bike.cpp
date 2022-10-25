#include "main.h"

BIKE::BIKE(void)
{
	// Nothing on init yet please
}

BIKE::~BIKE(void)
{
	// remove the main object/s on close
	close();
}


void BIKE::load()
{
	// loads the bike object (this will be from an embedded zip file eventually)
	dbLoadObject( "media\\superbikeoptsmall.X", obj = free_object() );
}

void BIKE::close()
{
	// frees up anything to do with the bike
	dbDeleteObject( obj );
}


void BIKE::mat_pos()
{
	MATRIX* mat = &gg->matrix;

	// set the bike position on the matrix
	float mhgt = height = dbGetGroundHeight( 1, xpos + mat->mathalfx, ypos + mat->mathalfy );

	// position bike
	dbPositionObject( obj, xpos, mhgt, ypos );

	// also position the sky dome so it's smooth
	dbPositionObject( mat->mat_skydome, xpos, 0, ypos );

	// reposition camera and angle to follow the bike
	dbPositionCamera(xpos, mhgt, ypos);
	dbSetCameraToObjectOrientation(obj);
	dbYRotateCamera( dbWrapValue( dbCameraAngleY() - ( turnspd * 4.0 ) ) );
	dbXRotateCamera( dbCameraAngleX() + 30 );
	dbMoveCamera(-10);
	dbXRotateCamera( dbCameraAngleX() - 25 );

	// reposition light
	dbPositionLight(1, xpos, mhgt + 20, ypos);
	dbSetLightToObjectOrientation(1, obj);
}

void BIKE::turn()
{
	// checks key inputs and alters the turn inertia controls
	if ( dbLeftKey() )
	{
		if ( turnspd > -4.0 )
		{
			turnspd -= 0.4;
		}
	}
	else if ( turnspd < 0 )
	{
		turnspd += 0.2;
	}
	if ( dbRightKey() )
	{
		if ( turnspd < 4.0 )
		{
			turnspd += 0.4;
		}
	}
	else if ( turnspd > 0 )
	{
		turnspd -= 0.2;
	}
	if ( turnspd >= -0.2 && turnspd <= 0.2 )
	{
		turnspd = 0;
	}

	// alter the bikes angle according to the turnspd
	angle = dbWrapValue( angle + turnspd );
	dbYRotateObject( obj, angle );
}

void BIKE::set_pos(float x, float y)
{
	// sets the initial bike position (dunno why I've done it this way)
	xpos = x;
	ypos = y;
}

void BIKE::set_angle_and_speed(float ang, float spd)
{
	// sets up the angle and speed of the bike at the start
	angle = ang;
	speed = spd;
}

void BIKE::init_height()
{
	// sets the initial height of the bike on the terrain
	height = dbGetGroundHeight(1, gg->matrix.mathalfx + xpos, gg->matrix.mathalfy + ypos );
}
void BIKE::move()
{
	// moves the bike with angle and speed and if its gone across a cell tile
	// then shift the map, trees and objects accordingly.
	// This helps for infinite terrain.
	bool flag = 0;

	xpos += dbSin( angle ) * speed;
	ypos += dbCos( angle ) * speed;

	MATRIX* mat = &gg->matrix;

	// check if a shift of the map on x axis
	if ( xpos < 0 )
	{
		// shift map right
		xpos += mat->matxs;
		dbShiftMatrixRight(1);
		flag = 1;

		// move all the trees too and the pickups
		gg->trees.right();
		gg->pickups.move_right();
	}
	else if ( xpos >= mat->matxs )
	{
		// shift map left
		xpos -= mat->matxs;
		dbShiftMatrixLeft(1);
		flag = 1;

		// move all trees as well and pickups
		gg->trees.left();
		gg->pickups.move_left();
	}

	// do the same for the ypos
	if ( ypos < 0 )
	{
		// shift map down
		ypos += mat->matys;
		dbShiftMatrixUp(1);
		flag = 1;

		// move all the trees up and pickups
		gg->trees.up();
		gg->pickups.move_up();
	}
	else if ( ypos >= mat->matys )
	{
		// shift everything down
		ypos -= mat->matys;
		dbShiftMatrixDown(1);
		flag = 1;

		// move all the trees down and pickups
		gg->trees.down();
		gg->pickups.move_down();
	}

	if ( flag )
	{
		// if shifted then update the terrain
		dbUpdateMatrix(1);
	}
}

void BIKE::set_angle()
{
	MATRIX* mat = &gg->matrix;

	// get height from matrix from 2 points (using bikes current angle)
	float x1 = xpos + ( dbSin( angle ) * 6 ) + mat->mathalfx;
	float y1 = ypos + ( dbCos( angle ) * 6 ) + mat->mathalfy;
	float h1 = dbGetGroundHeight(1, x1, y1 );
	float h2 = dbGetGroundHeight(1, xpos + mat->mathalfx, ypos + mat->mathalfy );
	
	// get the normal for the angles projection
	float norm = atan( h2 - h1 );

	// cheat and just multiply it to correct the angle of the bike
	dbXRotateObject( obj, norm * 20.0 );
}

int BIKE::do_all()
{
	// I'm leaving this open for now because I need to add a few more
	// functions within the time based loop
	int curr_time = game_timer;
	game_timer = dbTimer();

	int counter = game_timer - curr_time;
	game_timer -= counter % 50;		// do last frame next loop
	// Next update can catch up the skipped frame

	float player_damage = 0;		// damage taken during loop
	bool tree_hit = 0;				// flag if tree been hit

	while ( counter >= 50 )
	{
		if ( !gg->game_over )
		{
			// I've noticed something here
			// I should be adding to the players damage during the loop
			// instead of setting it in case theres multiple collisions
			// during the timer loop catchup. (NOT DONE YET)

			// run all the bike functions for the game loop
			if ( !gg->trees.check_collision( gg->trees.find_closest() ) )
			{
				turn();
			}
			else
			{
				player_damage = 5;
				// deal with the health bar further down
				tree_hit = 1;	// let me know to play the tree hit sound
			}
			if ( height < 100 )
			{
				// deal with bike health for being underwater (Cruel? yes!)
				player_damage = 20 - ( height - 80 );
				if ( player_damage > 20 )
					player_damage = 20;
				player_damage = ( player_damage + 3 ) / 6;

			}
			move();

			// increase the score a little here
			gg->game_score += 0.2;

			// dec the fuel a tad also (At least I'm fair here)
			gg->player.fuel -= 0.025;
		} // EOF if ( !game_over )

		counter -= 50;
	} // EOF while ( frame catchup counter )

	// outside the timer loop
	set_angle();
	mat_pos();

	//
	// update health bar
	//
	// This now is not a bar but is a complete hud showing
	// Health, fuel, score and pickup counts
	//
	if ( tree_hit )
		// play the crash sound
		gg->sounds.play( SOUNDS_BIKEHIT );

	// I think this draws the hud... he he
	gg->player.health_do( player_damage );

	if ( dbEscapeKey() )
		return -1;
	
	return 0;
}

void BIKE::init_all(int spd)
{
	// calls all the functions to initialise the bike
	set_pos(0,0);
	set_angle_and_speed(0,spd);
	init_height();

	// set the game timer (for timed movement)
	game_timer = dbTimer();

	// set players health
	//players_health_init();	// ******* put in to GAME_INIT
	gg->game_score = 0;			// This is also in game init too

	gg->player.health = 100;	// And this
}