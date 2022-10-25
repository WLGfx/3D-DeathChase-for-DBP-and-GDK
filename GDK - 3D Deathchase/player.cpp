#include "main.h"

PLAYER::PLAYER(void)
{
}

PLAYER::~PLAYER(void)
{
	health_close();
}

void PLAYER::health_init(int sw, int sh)
{
	// load the main hud image (should've done this at the start but oh well)
	dbLoadImage( "media\\HUDlayout.dds", hudimg = free_image() );

	// store the width and height
	hudw = dbGetImageWidth( hudimg );
	hudh = dbGetImageHeight( hudimg );

	// create the temporary image
	dbMakeImage( hudtmp = free_image(), hudw, hudh );

	// make the 3d plane object
	dbMakeObjectPlane( hudobj = free_object(), 1, 0.6 );

	// set the 3d defaults
	dbTextureObject( hudobj, hudimg );
	dbSetObject( hudobj, 1, 1, 0, 1, 1, 1, 0 );
	dbSetObjectTransparency( hudobj, 1 );
}

void PLAYER::copy_hud_img()
{
	int Dest = hudtmp;
	int Src = hudimg;
	
	// dest surface
	LPDIRECT3DSURFACE9 DestSurface = 0; 
	D3DLOCKED_RECT DestRect;          
	D3DSURFACE_DESC DestDesc; 

	dbGetImagePointer(Dest)->GetSurfaceLevel(0, &DestSurface);
	DestSurface->GetDesc(&DestDesc);
	DestSurface->LockRect(&DestRect, 0, 0);

	// src surface
	LPDIRECT3DSURFACE9 SrcSurface = 0; 
	D3DLOCKED_RECT SrcRect;          
	D3DSURFACE_DESC SrcDesc; 

	dbGetImagePointer(Src)->GetSurfaceLevel(0, &SrcSurface);
	SrcSurface->GetDesc(&SrcDesc);
	SrcSurface->LockRect(&SrcRect, 0, 0);

	int b = GetMemOffset(DestDesc.Format);	// pixel byte offset
	int s = ( hudh * DestRect.Pitch );
	s = s >> 2;		// copying ints instead of bytes

	UINT* Addr_Dest = (UINT*) DestRect.pBits;
	UINT* Addr_Src  = (UINT*) SrcRect.pBits;

	while ( s-- )
	{
		*Addr_Dest++ = *Addr_Src++;
	}

	SrcSurface->UnlockRect();
	SrcSurface->Release();
	SrcSurface = 0;

	DestSurface->UnlockRect();
	DestSurface->Release();
	DestSurface = 0;
}

void PLAYER::health_close()
{
	dbDeleteObject( hudobj );
	dbDeleteImage( hudimg );
	dbDeleteImage( hudtmp );
}

int PLAYER::health_do(float sub_amount)
{
	char buff[128];

	do_hud();

	// subtract damage from the game score
	gg->game_score -= sub_amount;
	if ( gg->game_score < 0 )
		gg->game_score = 0;

	// temporarily print score to screen
	sprintf( buff, "Score - %d", (int)gg->game_score );
	dbText(0,12,buff);

	// before we exit then subtract the amount
	health -= sub_amount;

	if ( health < 0 )
	{
		health = 0;
		// also handle end of game here (okay so I'm not)
	}

	if ( health > 0 )	// this isn't actually needed
		return 0;
	else
		return -1;
}

void PLAYER::do_hud()
{
	float x, y, z;
	int w;

	// first off copy the hud image onto the new texture
	copy_hud_img();

	// draw the health bar
	w = health;
	DrawBox( hudtmp, 16, 80, w+16, 80+32, 0x80ff0000 );

	// draw the fuel bar
	w = (int)fuel;
	DrawBox( hudtmp, 138, 80, 138+w, 80+32, 0x8000ff00 );

	// draw the score and the item pickup counts
	draw_score();
	draw_count( stars_count, 142 );
	draw_count( health_count, 174 );
	draw_count( fuel_count, 206 );

	// set new texture
	dbTextureObject( hudobj, hudtmp );

	// position it near camera
	x = dbCameraPositionX();
	y = dbCameraPositionY();
	z = dbCameraPositionZ();
	dbPositionObject( hudobj, x, y, z );
	dbSetObjectToCameraOrientation( hudobj );

	// move it to the bottom left of the cameras view
	dbMoveObject( hudobj, 2 );
	dbMoveObjectLeft( hudobj, 0.9 );
	dbMoveObjectDown( hudobj, 0.8 );
	dbPointObject( hudobj, x, y, z );

	//dbPasteImage( hudtmp, 0, 24 );

	if ( health <= 0 || fuel <= 0 )
	{
		gg->game_over = 1;	// set the game over flag
		yang = y;
	}
}

void PLAYER::game_over()
{
	static char title[] = "GAME OVER - PRESS ESC FOR MAIN MENU";
	char buf[64];
	dbText( 0, 12, title );
	sprintf( buf, "FINAL SCORE - %d", (int)gg->game_score );
	dbText( 0, 24, buf );

	dbYRotateCamera( yang );
	yang = dbWrapValue( yang + 1 );		// this isn't working (darn it)
	// Cameras getting updated somewhere else...

	float x = dbCameraPositionX();
	float y = dbCameraPositionY();
	float z = dbCameraPositionZ();

	dbPositionObject( hudobj, x, y, z );
	dbSetObjectToCameraOrientation( hudobj );
	dbMoveObject( hudobj, 1.1 );
	dbPointObject( hudobj, x, y, z );
}

void PLAYER::draw_score()
{
	int score = gg->game_score;		// get current score locally
	int c = 5;						// going to draw 5 numbers only
	int pos = 90;					// xpos in hud to start numbers at

	while ( c-- != 0 )
	{
		gg->numbers.paste_number( score % 10, hudtmp, pos, 200 );
		score /= 10;
		pos -= 16;
	}
}

void PLAYER::draw_count(int cnt, int yp)
{
	int c = 3;		// only going to draw 2 numbers
	int pos = 220;	// starting x position

	while ( c-- != 0 )
	{
		gg->numbers.paste_number( cnt % 10, hudtmp, pos, yp );
		cnt /= 10;
		pos -= 16;
	}
}