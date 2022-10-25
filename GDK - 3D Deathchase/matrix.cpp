#include "main.h"

using namespace std;

MATRIX::MATRIX(void)
{
}

MATRIX::~MATRIX(void)
{
	unload();
}

void MATRIX::load()
{
	// load the skydome
	//dbPrint("Creating Sky..."); dbSync();
	dbLoadObject( "media\\skydome.x", mat_skydome = free_object() );
	dbSetObject( mat_skydome, 1, 0, 1, 1, 0, 0, 0 );
	make_skydome_texture(1024);
	dbTextureObject( mat_skydome, sd_text );
	make_texture2();
}

void MATRIX::unload()
{
	dbDeleteObject( mat_skydome );
	dbDeleteImage( sd_text );
	dbDeleteImage( mtext );
}

void MATRIX::init(int w, int h, int ow, int oh, int trees, int oct)
{
	matwid = ow;			// store overall width and height
	mathgt = oh;

	dbMakeMatrix(1, ow, oh, w, h);

	// setup the perlin noise data to define the terrain
	PerlinNoise *pn = new PerlinNoise;

	double hgt;
	int x, y, t;
	
	matcw = w;				// set the cell counts
	match = h;

	matxs = matwid / matcw;	// set the cell sizes
	matys = matwid / match;

	mathalfx = matwid >> 1;	// half the overall width and height
	mathalfy = mathgt >> 1;

	// The perlin noise will change according to level difficulty
	pn->Set(0.75, 0.01, 1.25, oct, 0);

	//make_texture2();		// create a tiled texture for the terrain heights
	// the maketexture has been moved to the load() function to speed this up.
	dbPrepareMatrixTexture( 1, mtext, 16, 16 );

	dbSetMatrix(1,0,0,1,1,1,1,1);
	dbSetMatrixTrim( 1, 0.01, 0.01 );	// trim tiles UV to stop lines showing

	for (x=0; x<=w; x++) {
		for (y=0; y<=h; y++) {
			// use tiled perlin noise for endless terrain
			hgt = pn->GetHeightTiled(x, y, w, h);
			hgt = ( hgt + 1.0 ) / 2.0;
			hgt = hgt * 400.0;

			dbSetMatrixHeight(1, x, y, hgt);

			// Use the height value to set the tile texture
			t = (int)hgt;
			if (t<0) t=0; else if (t>255) t=255;

			dbSetMatrixTile(1, x, y, t); // set gradient tile texture
		}
	}

	dbSetMatrix( 1, 0, 0, 1, 0, 1, 1, 1 );	// whoops done this up there too

	//dbPrint("Lighting up the ground terrain..."); dbSync();

	// if the normals aren't set then the terrain looks awful
	calc_normals();		// fix matrix normals for proper lighting fx

	// reposition the matrix in the centre of the world
	dbPositionMatrix(1, -(matcw >> 1) * matxs, 0, -(match >> 1) * matys);

	// make the water plain
	make_water();

	// The plane object looked very crap!!! (Tried all kinds of effects)

	//water_obj = free_object();
	//dbMakeObjectPlain( water_obj, ow, oh );
	////dbColorObject( water_obj, 0x0000ff );
	//dbSetAlphaMappingOn( water_obj, 80 );
	//dbXRotateObject( water_obj, 90 );
	//dbPositionObject( water_obj, 0, 100, 0 );
	//dbSetSphereMappingOn( water_obj, sd_text );
	////dbSetReflectionShadingOn( water_obj );
	////dbTextureObject( water_obj, sd_text );
	//////dbSetObjectCull( water_obj, 0 );
	////dbScaleObjectTexture( water_obj, 0.01, 0.01 );
	//dbSetObject( water_obj, 1, 1, 0, 1, 0, 1, 0 );

	delete pn;		// finished with the perlin noise
	// a little note here about using class between seperate object files
	// they can bloat your code... (C can have an advantage over C++)

	// now place some trees on the map
	//dbPrint("Randomly placing a few trees..."); dbSync();
	gg->trees.place( trees );
	//dbSync();
}

void MATRIX::paste_mtext()
{
	// only used for testing the matrix texture
	dbPasteImage(mtext,0,0);
}


void MATRIX::calc_normals()
{
	// Calculates the matrix normals (non-DBP standard way)
	int x, y;
	float x1, x2, x3, y1, y2, y3, z1, z2, z3;
	float normx, normy, normz, normlength;

	float xstep = matwid / matcw;
	float zstep = mathgt / match;

	for ( x = 0; x <= matcw; x++ ) {
		for ( y = 0; y <= match; y++ ) {
			// Get a single triangle height coordinates
			x1 = xstep * x;
			x2 = xstep * ( x + 5 );		// changed from 1 for more definition
			x3 = xstep * x;

			z1 = zstep * y;
			z2 = zstep * y;
			z3 = zstep * ( y + 5 );

			y1 = dbGetGroundHeight( 1, x1, z1 );	// from matrix ground height
			y2 = dbGetGroundHeight( 1, x2, z2 );
			y3 = dbGetGroundHeight( 1, x3, z3 );

			normx = ( z1 - z2 ) * ( y3 - y2 ) - ( y1 - y2 ) * ( z3 - z2 );
			normy = ( x1 - x2 ) * ( z3 - z2 ) - ( z1 - z2 ) * ( x3 - x2 );
			normz = ( y1 - y2 ) * ( x3 - x2 ) - ( x1 - x2 ) * ( y3 - y2 );

			// done this way as it's faster than DBP's method
			normlength = sqrt( ( normx * normx ) + ( normy * normy ) + ( normz * normz ) );

			normx = normx / normlength;
			normy = normy / normlength;
			normz = normz / normlength;

			// and... it works...
			dbSetMatrixNormal( 1, x, y, sin(normx), cos(normy), cos(normz) );
		}
	}
}

void MATRIX::close()
{
	// yup got rid of loads here and move some too

	// release objects and images
	dbDeleteMatrix(1);
	//dbDeleteImage(mtext);
	/*dbDeleteObject(water_obj);*/
	//tree_delete();
	gg->trees.unplace();
	//dbDeleteObject( mat_skydome );
	//players_health_close();
	unmake_water();
}

void MATRIX::run()	// NOT USED (only for testing)
{
	dbShiftMatrixDown(1);
	if (dbLeftKey()) dbShiftMatrixRight(1);
	if (dbRightKey()) dbShiftMatrixLeft(1);
	dbUpdateMatrix(1);
}

void MATRIX::make_texture_grad()
{
	// now defunct, I've got a better more refined version
	int size = 2;

	int bm = free_bitmap();

	int range = 15;
	int hrange = 8;

	DWORD *ptr;

	// temp storage for the gradient colours
	int *cols = new int[256];
	int x, y, a, c;
	int r, g, b;
	int rr, gg, bb;

	// load the bitmap with the gradient
	dbLoadBitmap("media\\grad1.bmp", bm);
	dbSetCurrentBitmap(bm);

	// store the gradient colours from the image in the array
	dbLockPixels();
	ptr = (DWORD*)dbGetPixelsPointer();
	for (x=0; x<256; x++)
	{
		cols[x] = *ptr++;
	}
	dbUnlockPixels();

	// finished with the bitmap
	dbDeleteBitmap(bm);

	dbCreateBitmap(bm, size * 256, size);
	dbSetCurrentBitmap(bm);
	dbLockPixels();
	for (x=0; x<256; x++)
	{
		c = cols[x];

		r = ( c && 0xff0000 ) >> 16;
		g = ( c && 0x00ff00 ) >> 8;
		b = ( c && 0x0000ff );

		for (y=0; y<size; y++)
		{
			for (a=0; a<size; a++)
			{
				rr = ( rand() && range ) - hrange;
				gg = ( rand() && range ) - hrange;
				bb = ( rand() && range ) - hrange;

				r += rr;
				g += gg;
				b += bb;

				if (r<0) r=0; else if (r>255) r=255;
				if (g<0) g=0; else if (g>255) g=255;
				if (b<0) b=0; else if (b>255) b=255;

				c = ( r << 16 ) + ( g << 8 ) + b;

				dbDot( (x * size) + a, y, c);
			}
		}
	}
	dbUnlockPixels();

	mtext = free_image();
	dbGetImage(mtext, 0,0, ( size * 256 ) - 1, size - 1);
	dbPrepareMatrixTexture(1, mtext, 256, 1);

	dbDeleteBitmap(bm);
	dbSetCurrentBitmap(0);

	delete[] cols;
}

void MATRIX::make_texture2()
{
	// Earlier I loaded the bmp file with the gradient in it.
	// Here I create the 256 textures for the terrain using those
	// colour values.

	// this is the one that's now used (creates 256 textures for the matrix)
	int tile_size = 128;		// tile texture size
	int bm = free_bitmap();
	int grad[256];
	int x, y, xx, yy;
	int col, r, g, b;
	int rr, gg, bb;

	// Load the gradient ramp and store the RGB values
	// It's the only BMP file uses because the DDS wouldn't work
	// properly for some reason...
	dbLoadBitmap( "media\\grad1.bmp", bm );
	dbLockPixels();
	for ( x=0; x<256; x++) {
		grad[x]=dbPoint(x, 0); }
	dbUnlockPixels();
	dbDeleteBitmap( bm );

	// Create the 256 textures with a bit of distortion
	dbCreateBitmap( bm, tile_size * 16, tile_size * 16 );
	dbLockPixels();
	for ( x=0; x<16; x++ ) {
		for ( y=0; y<16; y++ ) {
			// get original RGB
			col = grad[x + (y*16)];
			// separate them
			r = ( col & 0xff0000 ) >> 16;
			g = ( col & 0x00ff00 ) >> 8;
			b = ( col & 0x0000ff );
			for ( xx=0; xx<tile_size; xx++ ) {
				for ( yy=0; yy<tile_size; yy++) {
					// distort the RGB values
					rr = r + ( rand() & 31 ) - 16;
					if ( rr<0 ) rr=0; else if ( rr>255 ) rr=255;
					gg = g + ( rand() & 31 ) - 16;
					if ( gg<0 ) gg=0; else if ( gg>255 ) gg=255;
					bb = b + ( rand() & 31 ) - 16;
					if ( bb<0 ) bb=0; else if ( bb>255 ) bb=255;
					// draw pixel to the correct tile
					dbDot( x * tile_size + xx, y * tile_size + yy, dbRGB(rr, gg, bb) );
				}
			}
		}
	}
	dbUnlockPixels();
	// grab the image and prepare it for the matrix
	dbGetImage( mtext = free_image(), 0,0, tile_size * 16, tile_size * 16 );
	dbDeleteBitmap( bm );
	//dbPrepareMatrixTexture( 1, mtext, 16, 16 );
}

void MATRIX::make_texture()
{
	// NOT USED - Although it was working while testing
	int cols[] = {
		0x000080,
		0x0000d0,
		0x0040d0,
		0x40d000,
		0x60a000,
		0xd0d040,
		0x808080,
		0xd0d0d0 };

	mtext = free_image();
	int size = 16;

	int x, y, a, i;
	
	dbLockPixels();

	for ( x = 0 ; x < size ; x++ ) {
		for ( y = 0 ; y < size ; y++ ) {
			a = rand() % 0x01f;
			a = a + ( a << 8 ) + ( a << 16 );
			for ( i = 0 ; i < 8 ; i++ ) {
				dbDot( x + ( i * size ), y, cols[ i ] + a );
			}
		}
	}

	dbUnlockPixels();
	dbGetImage( mtext, 0, 0, size * 8 - 1, size - 1 );
	dbPrepareMatrixTexture(1, mtext, 8, 1);
}
void MATRIX::make_skydome_texture(int size)
{
	// This texture for the clouds is also used for the water. (CHEAT!)
	// simple perlin noise clouds
	sd_text = free_image();
	PerlinNoise *pn = new PerlinNoise;
	pn->Set( 0.90, 0.2, 1.10, 8, 2 );
	int bm = free_bitmap();
	dbCreateBitmap( bm, size, size );

	int x, y, c;
	float hgt;

	dbLockPixels();

	for ( x=0; x<size; x++ )
	{
		for ( y=0; y<size; y++ )
		{
			hgt = pn->GetHeight( x, y );
			if ( hgt<0 ) hgt = 0;
			hgt *= 250;
			c = hgt;
			dbDot( x, y, dbRGB( c, c, 255 ) );
		}
	}

	dbUnlockPixels();

	dbGetImage( sd_text, 0, 0, size-1, size-1 );

	delete pn;
	dbDeleteBitmap( bm );
}

void MATRIX::make_water()
{
	// Another programmers cheating method
	// I'm creating a matrix and using the sky texture for the water ;-)
	dbMakeMatrix( 2, matwid, mathgt, 128, 128 );
	dbPrepareMatrixTexture( 2, sd_text, 1, 1 );
	dbFillMatrix( 2, 0, 0 );
	dbPositionMatrix( 2, -mathalfx, 100, -mathalfy );
	dbSetMatrix( 2, 0, 1, 0, 1, 1, 1, 1 );
	dbGhostMatrixOn( 2, 4 );	// iMode 4 gives best results
}

void MATRIX::unmake_water()
{
	dbDeleteMatrix( 2 );
}

void MATRIX::position_water()
{
	// This repositions the water matrix
	// To make it look like its moving then use the reverse of the bikes movement
	float x = matxs - gg->bike.xpos - mathalfx;
	float y = 100;
	float z = matys - gg->bike.ypos - mathalfy;

	dbPositionMatrix( 2, x, y, z );
}