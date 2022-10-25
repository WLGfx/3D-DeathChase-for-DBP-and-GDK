#include "DarkGDK.h"
#include "globstruct.h"
//#include "SC_Collision.h"	// Haven't had any need for this at all yet!!!

// my extra includes
#include "windows.h"		// Will be using these for display mode selection
#include "commctrl.h"

#include <vector>			// Trying to avoid STL (I prefer low level speed)
#include <fstream>
#include <string>

#include "perlin_noise.h"	// generates terrain and clouds
#include "unzip.h"			// might eventually use this
#include "resource.h"		// in case of embedding files

#include "_global.h"		// All the globals and embedded classes
#include "bike.h"			// Bike controls
#include "trees.h"			// tree handling
#include "pickups.h"		// pickups handling
#include "player.h"			// player handling
#include "matrix.h"			// terrain handling
#include "sounds.h"			// in game sound effects
#include "music.h"			// music handler

extern _gglobal *gg;

// external references to global vars I'm using
//
// Anything declared here is 95% being used in other code somewhere

extern GlobStruct* g_pGlob;
extern std::string gstring;

// find free resources
int free_object();
int free_bitmap();
int free_image();

void menu_init();
void menu_close();
int menu_run();



//
// direct image access stuff (Thanks to the TGC forums for the code)
//
void		PutImagePixel(int Dest, int x, int y, DWORD c);
void		DrawImageLine(int Dest, int x1, int y1, int x2, int y2, DWORD c2);
void		DrawImageCircle(int Dest, int CX, int CY, int R, DWORD c);
void		DrawBox(int Dest, int nLeft, int nTop, int nRight, int nBottom, DWORD c2);
void		DrawFilledTriangle(int Dest, int x1, int y1, int x2, int y2, int x3, int y3, DWORD c);

void		CopyHorizontalImageLine(int Dest, int ySource, int yDest);

void		StartNewFilledTriangleBatch();
void		DrawFilledTriangleBatch(int Dest);
void		AddFilledTriangleToBatch(int x1, int y1, int x2, int y2, int x3, int y3, DWORD c);

void		StartNew2dLineBatch();
void		Draw2dLineBatch(int Dest);
void		Add2dLineToBatch(int x, int y, int x2, int y2, DWORD dwCol);

//Internal use function
int			GetMemOffset(int format);
void		DrawImageLine(int Dest, int x1, int y1, int x2, int y2, DWORD c2, int nMemOffset, unsigned char *DestAddr, int w, int h, int nPitch);
void		HorizontalImageLine(int Dest, int x1, int y1, int x2, DWORD c2, int nMemOffset, unsigned char *DestAddr, int w, int h, int nPitch);
void		VerticalImageLine(int Dest, int x1, int y1, int y2, DWORD c2, int nMemOffset, unsigned char *DestAddr, int w, int h, int nPitch);

//
// testing code (OBSELETE)
//
void init_test();
void run_test();
void close_test();

//
// Various Fast sqrt() functions
//

inline float fsqrt(const float x)  // inlined
{
  union
  {
    int i;
    float x;
  } u;
  u.x = x;
  u.i = (1<<29) + (u.i >> 1) - (1<<22); 
  return u.x;
}