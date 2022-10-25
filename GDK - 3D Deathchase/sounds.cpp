#include "main.h"

//
// Getting close to the end of the .cpp files and decided not to
// comment on these because they are easy to understand.
//
// And I'm getting tired...
//

SOUNDS::SOUNDS(void)
{
}

SOUNDS::~SOUNDS(void)
{
	unload();
}

int SOUNDS::free_sound()
{
	int c=1;
	while ( dbSoundExist( c ) )
		c++;
	return c;
}

void SOUNDS::load()
{
	int s;
	soundID.clear();
	dbLoadSound( "media\\bikehittree.wav", s = free_sound() );
	soundID.push_back( s );
	dbLoadSound( "media\\healthhit.wav", s = free_sound() );
	soundID.push_back( s );
	dbLoadSound( "media\\starhit.wav", s = free_sound() );
	soundID.push_back( s );
	dbLoadSound( "media\\fuelhit.wav", s = free_sound() );
	soundID.push_back( s );
}

void SOUNDS::unload()
{
	int c=0;
	int s = soundID.size();		// 0 means its emtpy

	while ( c < s )
	{
		dbDeleteSound( soundID[c] );
		c++;
	}
	soundID.clear();
}

void SOUNDS::play(int s)
{
	if ( s < SOUNDS_MAX )
		dbPlaySound( soundID[s] );
}
