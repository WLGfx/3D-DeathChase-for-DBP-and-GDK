#include "main.h"

#include "d3d9.h"
#include "d3d9types.h"

//
// I found these on the TGC forums which have helped amazingly
// during the hud development. I was intending on using more of
// this code to fancy up the game but decided to finish it first.
//
// With these functions I have just started to scratch the surface
// of Direct X programming.
//

struct S_TRIANGLE {
	float x,y,z;
	DWORD c;
};


struct {
	int x1, x2, y1, y2;
	DWORD c;
} sLines[2800];

int nCurrentLine = 0;

struct {
	int x1, x2, y1, y2, x3, y3;
	DWORD c;
} sTriangles[4000];

int nCurrentTriangle = 0;

void PutImagePixel(int Dest, int x, int y, DWORD c) {    
	LPDIRECT3DSURFACE9 DestSurface = 0; 
	D3DLOCKED_RECT DestRect;          
	D3DSURFACE_DESC DestDesc;         

	int dmem_loc;
	int w, h, b;
	unsigned char *DestAddr;
	
	

	dbGetImagePointer(Dest)->GetSurfaceLevel(0, &DestSurface);

	DestSurface->GetDesc(&DestDesc);
	DestSurface->LockRect(&DestRect, 0, 0);

	DestAddr = (unsigned char*)DestRect.pBits; 	
	w = dbGetImageWidth(Dest);
	h = dbGetImageHeight(Dest);

	b = GetMemOffset(DestDesc.Format);

	if (x >= 0 && x < w-1 && y >= 0 && y < h-1) {
		dmem_loc = (x * b) + ((y) * DestRect.Pitch);
		*(DWORD*)(DestAddr + dmem_loc) = c;
	}

	DestSurface->UnlockRect();
	DestSurface->Release();
	DestSurface = 0;    
}

void StartNew2dLineBatch() {
	nCurrentLine = 0;
}

void Add2dLineToBatch(int x, int y, int x2, int y2, DWORD dwCol) {
	sLines[nCurrentLine].x1 = x;
	sLines[nCurrentLine].y1 = y;
	sLines[nCurrentLine].x2 = x2;
	sLines[nCurrentLine].y2 = y2;
	sLines[nCurrentLine].c = dwCol;
	nCurrentLine++;
}

void Draw2dLineBatch(int Dest) {
	LPDIRECT3DSURFACE9 DestSurface = 0; 
	D3DLOCKED_RECT DestRect;          
	D3DSURFACE_DESC DestDesc; 

	dbGetImagePointer(Dest)->GetSurfaceLevel(0, &DestSurface);
	DestSurface->GetDesc(&DestDesc);
	DestSurface->LockRect(&DestRect, 0, 0);
	
	int w, h, b;
	unsigned char *DestAddr;	

	DestAddr = (unsigned char*)DestRect.pBits; 	    
	w = dbGetImageWidth(Dest);
	h = dbGetImageHeight(Dest);
	
	b = GetMemOffset(DestDesc.Format);

	for (int n = 0; n < nCurrentLine; n++) {		
		if (sLines[n].y1 == sLines[n].y2) {
			HorizontalImageLine(Dest, sLines[n].x1, sLines[n].y1, sLines[n].x2, sLines[n].c, b, DestAddr, w, h, DestRect.Pitch);
		} else {
			if (sLines[n].x1 == sLines[n].x2) {
				VerticalImageLine(Dest, sLines[n].x1, sLines[n].y1, sLines[n].y2, sLines[n].c, b, DestAddr, w, h, DestRect.Pitch);
			} else {
				DrawImageLine(Dest, sLines[n].x1, sLines[n].y1, sLines[n].x2, sLines[n].y2, sLines[n].c, b, DestAddr, w, h, DestRect.Pitch);
			}
		}
	}

	
	DestSurface->UnlockRect();
	DestSurface->Release();
	DestSurface = 0;
}

void DrawImageLine(int Dest, int x1, int y1, int x2, int y2, DWORD c2) {
	LPDIRECT3DSURFACE9 DestSurface = 0; 
	D3DLOCKED_RECT DestRect;          
	D3DSURFACE_DESC DestDesc; 

	dbGetImagePointer(Dest)->GetSurfaceLevel(0, &DestSurface);
	DestSurface->GetDesc(&DestDesc);
	DestSurface->LockRect(&DestRect, 0, 0);
	
	int w, h, b;
	unsigned char *DestAddr;	

	DestAddr = (unsigned char*)DestRect.pBits; 	    
	w = dbGetImageWidth(Dest);
	h = dbGetImageHeight(Dest);
	
	b = GetMemOffset(DestDesc.Format);

	if (y1 == y2) {
		HorizontalImageLine(Dest, x1, y1, x2, c2, b, DestAddr, w, h, DestRect.Pitch);
	} else {
		if (x1 == x2) {
			VerticalImageLine(Dest, x1, y1, y2, c2, b, DestAddr, w, h, DestRect.Pitch);
		} else {
			DrawImageLine(Dest, x1, y1, x2, y2, c2, b, DestAddr, w, h, DestRect.Pitch);
		}
	}

	
	DestSurface->UnlockRect();
	DestSurface->Release();
	DestSurface = 0;
}

void HorizontalImageLine(int Dest, int x1, int y1, int x2, DWORD c2, int nMemOffset, unsigned char *DestAddr, int w, int h, int nPitch) {
	int dmem_loc;
	int t;

	if (x1 > x2) {
		t = x1;
		x1 = x2;
		x2 = t;
	}


	if (x2 >= w) {
		x2 = w - 1;
	} else {
		if (x2 < 0) {
			x2 = 0;
		}
	}

	if (x1 < 0) {
		x1 = 0;
	} else {
		if (x1 >= w) {
			x1 = w - 1;
		}
	}

	if (y1 < 0) {
		return;
	} else {
		if (y1 >= h) {
			return;
		}
	}


	for (int x = x1; x <= x2; x++) {
		if (x < 0 || x >= w || y1 < 0 || y1 >= h) {
			continue;
		}

		dmem_loc = (x * nMemOffset) + ((y1) * nPitch);
		*(DWORD*)(DestAddr + dmem_loc) = c2;
	}	
}

void VerticalImageLine(int Dest, int x1, int y1, int y2, DWORD c2, int nMemOffset, unsigned char *DestAddr, int w, int h, int nPitch) {
	int dmem_loc;
	int t;

	if (y1 > y2) {
		t = y1;
		y1 = y2;
		y2 = t;
	}
	
	if (y1 < 0) {
		y1 = 0;
	} else {
		if (y1 >= h) {
			y1 = h -1;
		}
	}

	if (y2 < 0) {
		y2 = 0;
	} else {
		if (y2 >= h) {
			y2 = h - 1;
		}
	}

	if (x1 < 0) {
		return;
	} else {
		if (x1 >= w) {
			return;
		}
	}

	for (int y = y1; y <= y2; y++) {
		dmem_loc = (x1 * nMemOffset) + ((y) * nPitch);
		*(DWORD*)(DestAddr + dmem_loc) = c2;
	}
}

void DrawImageLine(int Dest, int x1, int y1, int x2, int y2, DWORD c2, int nMemOffset, unsigned char *DestAddr, int w, int h, int nPitch) {
	int dmem_loc;
	int u = 0;
    int dx, dy, c, M, xinc = 1, yinc = 1;
	int hits=0;
    int x3 = x1;
    int y3 = y1;
    dx = x2 - x1;
    dy = y2 - y1;

    if (dx < 0) { xinc = -1; dx = -dx; }
    if (dy < 0) { yinc = -1; dy = -dy; }

    if (dy < dx) {
        c = 2 * dx; 
		M = 2 * dy;

        while (x3 != x2) {
			if (x3 >= 0 && x3 < w && y3 >= 0 && y3 < h) {
				dmem_loc = (x3 * nMemOffset) + ((y3) * nPitch);
				*(DWORD*)(DestAddr + dmem_loc) = c2;
			}

            x3 += xinc; 
			u += M;

            if (u > dx) { y3 += yinc; u -= c; }
        }

    } else {
        c = 2 * dy; 
		M = 2 * dx;

        while(y3 != y2) {
			if (x3 >= 0 && x3 < w && y3 >= 0 && y3 < h) {
				dmem_loc = (x3 * nMemOffset) + ((y3) * nPitch);
				*(DWORD*)(DestAddr + dmem_loc) = c2;
			}

            y3 += yinc; 
			u += M;

            if(u > dy) { x3 += xinc; u -= c; }
        }
    }

	if (x3 >= 0 && x3 < w && y3 >= 0 && y3 < h) {
		dmem_loc = (x3 * nMemOffset) + ((y3) * nPitch);
		*(DWORD*)(DestAddr + dmem_loc) = c2;
	}
}

void CopyHorizontalImageLine(int Dest, int ySource, int yDest) {
	int dmem_loc;
	int dmem_loc2;
	LPDIRECT3DSURFACE9 DestSurface = 0; 
	D3DLOCKED_RECT DestRect;          
	D3DSURFACE_DESC DestDesc; 

	if (!dbImageExist(Dest)) {
		return;
	}

	dbGetImagePointer(Dest)->GetSurfaceLevel(0, &DestSurface);
	DestSurface->GetDesc(&DestDesc);
	DestSurface->LockRect(&DestRect, 0, 0);
	
	int w, h, b;
	unsigned char *DestAddr;	

	DestAddr = (unsigned char*)DestRect.pBits; 	    
	w = dbGetImageWidth(Dest);
	h = dbGetImageHeight(Dest);
	
	b = GetMemOffset(DestDesc.Format);

	if (ySource < 0) {
		return;
	} else {
		if (ySource >= h) {
			return;
		}
	}

	if (yDest < 0) {
		return;
	} else {
		if (yDest >= h) {
			return;
		}
	}

	for (int x = 0; x < w; x++) {
		dmem_loc = (x * b) + ((ySource) * DestRect.Pitch);
		dmem_loc2 = (x * b) + ((yDest) * DestRect.Pitch);
		*(DWORD*)(DestAddr + dmem_loc2) = *(DWORD*)(DestAddr + dmem_loc);
		//*(DWORD*)(DestAddr + dmem_loc2) = dbRgb(255, 0, 0);
	}	

	DestSurface->UnlockRect();
	DestSurface->Release();
	DestSurface = 0;
}


void StartNewFilledTriangleBatch() {
	nCurrentTriangle = 0;
}

void AddFilledTriangleToBatch(int x, int y, int x2, int y2, int x3, int y3, DWORD dwCol) {
	sTriangles[nCurrentTriangle].x1 = x;
	sTriangles[nCurrentTriangle].y1 = y;
	sTriangles[nCurrentTriangle].x2 = x2;
	sTriangles[nCurrentTriangle].y2 = y2;
	sTriangles[nCurrentTriangle].x3 = x3;
	sTriangles[nCurrentTriangle].y3 = y3;
	sTriangles[nCurrentTriangle].c = dwCol;
	nCurrentTriangle++;
}

struct Vertex
{
    float x, y, z;
    float tu, tv;

	enum FVF
	{
		FVF_Flags = D3DFVF_XYZ | D3DFVF_TEX1
	};
};


void DrawFilledTriangleBatch(int Dest) {
	//random dx stuff, ignore...
	LPDIRECT3DSURFACE9 DestSurface = 0; 
	D3DLOCKED_RECT DestRect;          
	D3DSURFACE_DESC DestDesc; 

	dbGetImagePointer(Dest)->GetSurfaceLevel(0, &DestSurface);
	DestSurface->GetDesc(&DestDesc);
	DestSurface->LockRect(&DestRect, 0, 0);
	
	int w, h, b;
	unsigned char *DestAddr;	

	DestAddr = (unsigned char*)DestRect.pBits; 	    
	w = dbGetImageWidth(Dest);
	h = dbGetImageHeight(Dest);
	
	b = GetMemOffset(DestDesc.Format);

	int x1, y1, x2, y2, x3, y3;
	DWORD c;

	//begin triangle code
	int dx1, dx2, dx3, t, t2;

	for (int n = 0; n < nCurrentTriangle; n++) {
		x1 = sTriangles[n].x1;
		x2 = sTriangles[n].x2;
		x3 = sTriangles[n].x3;

		y1 = sTriangles[n].y1;
		y2 = sTriangles[n].y2;
		y3 = sTriangles[n].y3;

		c = sTriangles[n].c;

		//order coordinates so y1 < y2 < y3 (
		if (y2 < y1) {
			t = y2;
			t2 = x2;

			y2 = y1;
			x2 = x1;
			
			y1 = t;
			x1 = t2;
		}

		if (y3 < y1) {
			t = y3;
			t2 = x3;

			y3 = y1;
			x3 = x1;

			y1 = t;
			x1 = t2;
		}

		if (y3 < y2) {
			t = y3;
			t2 = x3;

			y3 = y2;
			x3 = x2;

			y2 = t;
			x2 = t2;
		}


		//calculate the deltas

		if (y2-y1 > 0) { 
			dx1 = (x2 - x1) / (y2 - y1);
		} else { 
			dx1 = x2 - x1; 
		}

		if (y3-y1 > 0) {
			dx2 = (x3 - x1) / (y3 - y1);
		} else {
			dx2 = 0;
		}

		if (y3-y2 > 0) {
			dx3 = (x3 - x2) / (y3 - y2);
		} else {
			dx3 = 0;
		}

		//draw the triangle

		int sx, sy, ex, ey;

		sx = x1;
		sy = y1;
		ex = sx;
		ey = sy;
		
		if (dx1 > dx2) {
			while(sy <= y2) {
				sy++;
				ey++;
				sx += dx2;
				ex += dx1;

				HorizontalImageLine(Dest, sx, sy, ex, c, b, DestAddr, w, h, DestRect.Pitch);
			}

			ex = x2;
			ey = y2;

			while (sy <= y3) {
				sy++;
				ey++;
				sx += dx2;
				ex += dx3;

				HorizontalImageLine(Dest, sx, sy, ex, c, b, DestAddr, w, h, DestRect.Pitch);
			}
		} else {
			while (sy <= y2) {
				sy++;
				ey++;
				sx += dx1;
				ex += dx2;

				HorizontalImageLine(Dest, sx, sy, ex, c, b, DestAddr, w, h, DestRect.Pitch);
			}

			sx = x2;
			sy = y2;

			while (sy <= y3) {
				sy++;
				ey++;
				sx += dx3;
				ex += dx2;

				HorizontalImageLine(Dest, sx, sy, ex, c, b, DestAddr, w, h, DestRect.Pitch);
			}
		}
	}
	//more dx stuff to ignore..
	DestSurface->UnlockRect();
	DestSurface->Release();
	DestSurface = 0;
}


void DrawFilledTriangle(int Dest, int x1, int y1, int x2, int y2, int x3, int y3, DWORD c) {
	//random dx stuff, ignore...
	LPDIRECT3DSURFACE9 DestSurface = 0; 
	D3DLOCKED_RECT DestRect;          
	D3DSURFACE_DESC DestDesc; 

	dbGetImagePointer(Dest)->GetSurfaceLevel(0, &DestSurface);
	DestSurface->GetDesc(&DestDesc);
	DestSurface->LockRect(&DestRect, 0, 0);
	
	int w, h, b;
	unsigned char *DestAddr;	

	DestAddr = (unsigned char*)DestRect.pBits; 	    
	w = dbGetImageWidth(Dest);
	h = dbGetImageHeight(Dest);
	
	b = GetMemOffset(DestDesc.Format);

	//begin triangle code
	int dx1, dx2, dx3, t, t2;

	//order coordinates so y1 < y2 < y3 (
	if (y2 < y1) {
		t = y2;
		t2 = x2;

		y2 = y1;
		x2 = x1;
		
		y1 = t;
		x1 = t2;
	}

	if (y3 < y1) {
		t = y3;
		t2 = x3;

		y3 = y1;
		x3 = x1;

		y1 = t;
		x1 = t2;
	}

	if (y3 < y2) {
		t = y3;
		t2 = x3;

		y3 = y2;
		x3 = x2;

		y2 = t;
		x2 = t2;
	}


	//calculate the deltas

	if (y2-y1 > 0) { 
		dx1 = (x2 - x1) / (y2 - y1);
	} else { 
		dx1 = x2 - x1; 
	}

	if (y3-y1 > 0) {
		dx2 = (x3 - x1) / (y3 - y1);
	} else {
		dx2 = 0;
	}

	if (y3-y2 > 0) {
		dx3 = (x3 - x2) / (y3 - y2);
	} else {
		dx3 = 0;
	}

	//draw the triangle

	int sx, sy, ex, ey;

	sx = x1;
	sy = y1;
	ex = sx;
	ey = sy;
	
	if (dx1 > dx2) {
		while(sy <= y2) {
			sy++;
			ey++;
			sx += dx2;
			ex += dx1;

			HorizontalImageLine(Dest, sx, sy, ex, c, b, DestAddr, w, h, DestRect.Pitch);
		}

		ex = x2;
		ey = y2;

		while (sy <= y3) {
			sy++;
			ey++;
			sx += dx2;
			ex += dx3;

			HorizontalImageLine(Dest, sx, sy, ex, c, b, DestAddr, w, h, DestRect.Pitch);
		}
	} else {
		while (sy <= y2) {
			sy++;
			ey++;
			sx += dx1;
			ex += dx2;

			HorizontalImageLine(Dest, sx, sy, ex, c, b, DestAddr, w, h, DestRect.Pitch);
		}

		sx = x2;
		sy = y2;

		while (sy <= y3) {
			sy++;
			ey++;
			sx += dx3;
			ex += dx2;

			HorizontalImageLine(Dest, sx, sy, ex, c, b, DestAddr, w, h, DestRect.Pitch);
		}
	}

	//more dx stuff to ignore..
	DestSurface->UnlockRect();
	DestSurface->Release();
	DestSurface = 0;
}

void DrawBox(int Dest, int nLeft, int nTop, int nRight, int nBottom, DWORD c2) {

	LPDIRECT3DSURFACE9 DestSurface = 0; 
	D3DLOCKED_RECT DestRect;          
	D3DSURFACE_DESC DestDesc; 


	dbGetImagePointer(Dest)->GetSurfaceLevel(0, &DestSurface);
	DestSurface->GetDesc(&DestDesc);
	DestSurface->LockRect(&DestRect, 0, 0);
	
	int w, h, b;
	unsigned char *DestAddr;	

	DestAddr = (unsigned char*)DestRect.pBits; 	    
	w = dbGetImageWidth(Dest);
	h = dbGetImageHeight(Dest);
	
	b = GetMemOffset(DestDesc.Format);

	for (int x = nLeft; x <= nRight; x++) {
		DrawImageLine(Dest, x, nTop, x, nBottom, c2, b, DestAddr, w, h, DestRect.Pitch);
	}

	DestSurface->UnlockRect();
	DestSurface->Release();
	DestSurface = 0;
}

void DrawImageCircle(int Dest, int CX, int CY, int R, DWORD c) {
	LPDIRECT3DSURFACE9 DestSurface = 0; 
	D3DLOCKED_RECT DestRect;          
	D3DSURFACE_DESC DestDesc;         

	int dmem_loc;
	int w, h;
	unsigned char *DestAddr;

	dbGetImagePointer(Dest)->GetSurfaceLevel(0, &DestSurface);
	DestSurface->GetDesc(&DestDesc);
	DestSurface->LockRect(&DestRect, 0, 0);

	DestAddr = (unsigned char*)DestRect.pBits; 	

	int X, Y;
	int XChange, YChange;
	int RadiusError;

	X = R;
	Y = 0;

	XChange = 1 - 2 * R;
	YChange = 1;
	RadiusError = 0;

	w = dbGetImageWidth(Dest);
	h = dbGetImageHeight(Dest);

	while(X > Y) {
	
		/*
		dbDot(CX+X, CY+Y);// {point in octant 1}
		dbDot(CX-X, CY+Y);// {point in octant 4}
	//	dbDot(CX-X, CY-Y);// {point in octant 5}
	//	dbDot(CX+X, CY-Y);// {point in octant 8}
	//	dbDot(CX+Y, CY+X);// {point in octant 2}
	//	dbDot(CX-Y, CY+X);// {point in octant 3}
		dbDot(CX-Y, CY-X);// {point in octant 6}
		dbDot(CX+Y, CY-X);
		*/

		if (CX+X >= 0 && CX+X < w-1 && CY+Y >= 0 && CY+Y < h-1) {
			dmem_loc = ((CX+X) * 4) + ((CY +Y) * DestRect.Pitch);
			*(DWORD*)(DestAddr + dmem_loc) = c;
		}

		if (CX-X >= 0 && CX-X < w-1 && CY+Y >= 0 && CY+Y < h-1) {
			dmem_loc = ((CX-X) * 4) + ((CY +Y) * DestRect.Pitch);
			*(DWORD*)(DestAddr + dmem_loc) = c;
		}

		if (CX-X >= 0 && CX-X < w-1 && CY-Y >= 0 && CY-Y < h-1) {
			dmem_loc = ((CX-X) * 4) + ((CY -Y) * DestRect.Pitch);
			*(DWORD*)(DestAddr + dmem_loc) = c;
		}

		if (CX+X >= 0 && CX+X < w-1 && CY-Y >= 0 && CY-Y < h-1) {
			dmem_loc = ((CX+X) * 4) + ((CY -Y) * DestRect.Pitch);
			*(DWORD*)(DestAddr + dmem_loc) = c;
		}

		if (CX+Y >= 0 && CX+Y < w-1 && CY+X >= 0 && CY+X < h-1) {
			dmem_loc = ((CX+Y) * 4) + ((CY +X) * DestRect.Pitch);
			*(DWORD*)(DestAddr + dmem_loc) = c;
		}

		if (CX-Y >= 0 && CX-Y < w-1 && CY+X >= 0 && CY+X < h-1) {
			dmem_loc = ((CX-Y) * 4) + ((CY +X) * DestRect.Pitch);
			*(DWORD*)(DestAddr + dmem_loc) = c;
		}

		if (CX-Y >= 0 && CX-Y < w-1 && CY-X >= 0 && CY-X < h-1) {
			dmem_loc = ((CX-Y) * 4) + ((CY -X) * DestRect.Pitch);
			*(DWORD*)(DestAddr + dmem_loc) = c;
		}

		if (CX+Y >= 0 && CX+Y < w-1 && CY-X >= 0 && CY-X < h-1) {
			dmem_loc = ((CX+Y) * 4) + ((CY -X) * DestRect.Pitch);
			*(DWORD*)(DestAddr + dmem_loc) = c;
		}

		Y++;

		RadiusError += YChange;
		YChange += 2;

		if (2 * RadiusError + XChange > 0 ) {
			X--;
			RadiusError += XChange;
			XChange += 2;
		}
	}


	DestSurface->UnlockRect();
	DestSurface->Release();
	DestSurface = 0;
}


int GetMemOffset(int format) {

	//if (dbScreenDepth() == 16)
		//return 3;

	switch(format) {
			
		case D3DFMT_R8G8B8:
			return 3;
			break;

		case D3DFMT_A8R8G8B8:
			return 4;
			break;

		case D3DFMT_X8R8G8B8:
			return 4;
			break;

		case D3DFMT_R5G6B5:
			return 2;
			break;

		case D3DFMT_X1R5G5B5:
			return 2;
			break;

		case D3DFMT_A1R5G5B5:
			return 2;
			break;

		case D3DFMT_A4R4G4B4:
			return 2;
			break;

		case D3DFMT_R3G3B2:
			return 1;
			break;

		case D3DFMT_A8:
			return 1;
			break;

		case D3DFMT_A8R3G3B2:
			return 2;
			break;

		case D3DFMT_X4R4G4B4:
			return 2;
			break;

		case D3DFMT_A2B10G10R10:
			return 4;
			break;

		case D3DFMT_A8B8G8R8:
			return 4;
			break;

		case D3DFMT_X8B8G8R8:
			return 4;
			break;

		case D3DFMT_G16R16:
			return 4;
			break;

		case D3DFMT_A2R10G10B10:
			return 4;
			break;
	}
	return 0;
}

