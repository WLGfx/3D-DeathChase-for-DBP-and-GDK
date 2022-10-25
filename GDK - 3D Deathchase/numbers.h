#pragma once

#include "d3d9.h"
#include "d3d9types.h"

class NUMBERS
{
public:
	int		ID[10];		// image ID list for each number

	LPDIRECT3DSURFACE9	Surface;	// used to lock surface
	D3DLOCKED_RECT		Rect;
	D3DSURFACE_DESC		Desc;

	LPDIRECT3DSURFACE9	ImgSurface;	// used draw numbers to
	D3DLOCKED_RECT		ImgRect;
	D3DSURFACE_DESC		ImgDesc;

			NUMBERS(void);
			~NUMBERS(void);

	void	load();
	void	unload();

	void*	lock(int id);		// lock surface for reading
	void	unlock();			// unlock it

	void*	img_lock(int img);	// lock dest surface for drawing to
	void	img_unlock();

	void	paste_number(int num, int img, int xp, int yp);
};
