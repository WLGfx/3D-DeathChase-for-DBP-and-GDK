#include "main.h"

// These functions are were I'm just scratching the surface
// with 2D DirectX programming. A learning curve for me.
// If I didn't use these then the game would have slowed
// a lot with creating bitmaps, drawing to them, grabbing an
// image and then texturing the hud. These number functions
// draw directly to the hud image.

NUMBERS::NUMBERS(void)
{
}

NUMBERS::~NUMBERS(void)
{
	unload();
}

void NUMBERS::load()
{
	// Load the numbers bitmap and creates seperate images from it

	int bm = free_bitmap();
	dbLoadBitmap( "media\\numbers.dds", bm );

	int x;
	for ( x=0; x<10; x++ )
	{
		ID[x] = free_image();
		dbGetImage( ID[x], x*16, 0, x*16+15, 31 );
	}
	dbDeleteBitmap( bm );
}

void NUMBERS::unload()
{
	int x;

	for ( x=0; x<10; x++ )
	{
		dbDeleteImage( ID[x] );
	}
}

void* NUMBERS::lock(int id)
{
	Surface = 0;	

	dbGetImagePointer( ID[id] )->GetSurfaceLevel(0, &Surface);

	Surface->GetDesc(&Desc);
	Surface->LockRect(&Rect, 0, 0);

	return (void*)Rect.pBits;
}

void NUMBERS::unlock()
{
	Surface->UnlockRect();
	Surface->Release();
	Surface = 0;
}

void* NUMBERS::img_lock(int img)
{
	ImgSurface = 0;
	dbGetImagePointer( img )->GetSurfaceLevel(0, &ImgSurface);
	ImgSurface->GetDesc(&ImgDesc);
	ImgSurface->LockRect(&ImgRect, 0, 0);
	return (void*)ImgRect.pBits;
}

void NUMBERS::img_unlock()
{
	ImgSurface->UnlockRect();
	ImgSurface->Release();
	ImgSurface = 0;
}

void NUMBERS::paste_number(int num, int img, int xp, int yp)
{
	// The main number drawing function which draws an
	// image from the numbers to another image

	// lock both surfaces
	char *src = (char*)lock( num );			// locks the main image
	char *dst = (char*)img_lock( img );		// locks the dest image

	UINT *srctmp;
	UINT *dsttmp;

	int x, y;
	int srcpit = Rect.Pitch;
	int cnt = srcpit >> 2;	// how many ints to copy

	int dstpit = ImgRect.Pitch;

	for ( y=0; y<32; y++ )
	{
		srctmp = (UINT*) ( src + ( y * srcpit ) );
		dsttmp = (UINT*) ( dst + ( xp * 4 ) + ( ( yp + y ) * dstpit ) );
		for ( x=0; x<cnt; x++)	// Why 24? And not 16?
		{
			*dsttmp++ = *srctmp++;
		}
	}

	// unlock both surfaces
	unlock();
	img_unlock();
}