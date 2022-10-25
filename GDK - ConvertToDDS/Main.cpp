// I haven't made this anything special, it just loads an image
// and then save it back out into another format.

// This program will convert the PNG images to DDS images
#include "DarkGDK.h"

void DarkGDK ( void )
{
	char path[] = "D:\\Cpp\\GDK - 3D Deathchase\\GDK - 3D Deathchase\\media\\";
	char file[] = "numbers.png";
	char dds[] = "dds";

	char input[256];
	char output[256];

	// make full file name for input
	strcpy( input, path );
	strcpy( input + strlen( path ), file );

	// make full file name for output
	strcpy( output, path );
	strcpy( output + strlen( path ), file );
	strcpy( output + strlen( output ) - 3, dds );

	// Load it up and save it out as the new image
	dbLoadImage( input, 1 );
	dbSaveImage( output, 1 );

	dbSyncOn   ( );
	dbSyncRate ( 60 );

	// Now the magic bit. Let the use know I am a god!
	while ( LoopGDK ( ) )
	{
		dbCLS();
		dbText( 0, 0, "CONVERTED:" );
		dbText( 0, 12, input );
		dbText( 0, 24, "TO:" );
		dbText( 0, 36, output );
		dbText( 0, 60, "Press Esc to exit..." );

		dbSync ( );
	}
	return;
}