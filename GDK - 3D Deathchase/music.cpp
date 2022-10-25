#include "main.h"

// Make sure the bassmod.lib is attached to the project

// This is just a very simple wrapper to play music
// in the game...

static string music1 = "media\\menu.it";
static string music2 = "media\\ingame.it";

MUSIC::MUSIC(void)
{
	// The constructor loads the dll
	// and sets the flag is successful
	if ( BASSMOD_GetVersion() != MAKELONG(2,0) )
		loaded = 0;
	else
	{
		loaded = 1;
		BASSMOD_Init(-1,44100,0);
	}
}

MUSIC::~MUSIC(void)
{
	if ( loaded )
		BASSMOD_Free();
}

void MUSIC::load_and_play( int m )
{
	if ( loaded )
	{
		switch ( m )
		{
		case 0:
			BASSMOD_MusicLoad(FALSE,(char*)music1.c_str(),0,0,BASS_MUSIC_LOOP);
			BASSMOD_MusicPlay();
			break;
		case 1:
			BASSMOD_MusicLoad(FALSE,(char*)music2.c_str(),0,0,BASS_MUSIC_LOOP);
			BASSMOD_MusicPlay();
			break;
		}
	}
}

void MUSIC::stop()
{
	if ( loaded )
	{
		BASSMOD_MusicStop();
		BASSMOD_MusicFree();
	}
}