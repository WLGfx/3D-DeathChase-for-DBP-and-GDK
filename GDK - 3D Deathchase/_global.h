#pragma once

#include "bike.h"
#include "trees.h"
#include "pickups.h"
#include "player.h"
#include "matrix.h"
#include "sounds.h"
#include "music.h"
#include "numbers.h"

struct LEVEL {
	// this is setup when a selection is made from the main menu
	int bike_speed;
	int tree_count;
	int octaves;
	int level;
};

class _gglobal
{
public:
	int			main_colour;	// background and fog colour
	int			screen_width;	// main displays dimensions
	int			screen_height;

	float		game_score;		// main game score (stays here now)

	int			game_over;		// 0=game running (1=game over)

	// On init these will load/prep their own media
	BIKE		bike;			// The bike class
	TREES		trees;			// The trees class
	PICKUPS		pickups;		// The pickups class
	PLAYER		player;			// The puny players class
	MATRIX		matrix;			// The map class
	LEVEL		level;			// The level selection class
	SOUNDS		sounds;			// Sound effects
	MUSIC		music;			// music handler
	NUMBERS		numbers;		// for drawing the scores, etc

				_gglobal(void);
				~_gglobal(void);

	void		display_fps();	// Display the current frame rate
	void		init_screen(int sw, int sh, int colour);

	void		init_game();	// Sets up the game objects etc
	void		close_game();	// close down the games objects, etc
	void		run_game();		// Runs all the games functions
	void		show_game_over();
};
