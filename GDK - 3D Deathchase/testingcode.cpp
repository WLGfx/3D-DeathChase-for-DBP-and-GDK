#include "main.h"

//
// This is just the test the direct access to Image data in GDK
//

int test_img;

void init_test()
{
	test_img = free_image();
	dbSetImageColorKey(0,0,0);
	dbMakeImage( test_img, gg->screen_width, gg->screen_height );
	//dbMakeImageUsage( test_img, screen_width, screen_height, 1 );
	dbDrawToFront();
}

void run_test()
{
	int y;
	int sw = gg->screen_width;	// optimised
	int sh = gg->screen_height;

	for ( y=0; y<sh; y++ )
	{
		DrawImageLine( test_img, 0,y, sw-1,y, 0x00000000 );
	}

	y = rand() % sh;
	DrawImageLine( test_img, 0,y, sw-1,y, 0xffffff00 );

	dbPasteImage( test_img, 0, 0, 1 );
}

void close_test()
{
	dbDeleteImage( test_img );
}
