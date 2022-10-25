========================================================================
    Dark GDK - 3D Game : GDK - 3D Deathchase Project Overview
========================================================================

Dark GDK - 3D Game has created this GDK - 3D Deathchase project for you as 
a starting point.

This file contains a summary of what you will find in each of the files 
that make up your project.

Dark GDK - 3D Game.vcproj
	This is the main project file for projects generated using an 
	Application Wizard. It contains information about the version of the 
	product that generated the file, and information about the platforms, 
	configurations, and project features selected with the Application 
	Wizard.

Dark GDK - The Game Creators - www.thegamecreators.com

/////////////////////////////////////////////////////////////////////////////
Other notes:

1. Clean up the default GDK setup (DONE)
2. Include windows stuff and comctl32.lib (DONE)


/////////////////////////////////////////////////////////////////////////////

Well, if you have gotten this far then you should be running VS2008 with
all the Dark GDK installed correctly.

I make no promises that this code is very easy to read but I have commented
it as much as possible.

Throughout writing this game, which I might add, was quite easy to do, I
have actually not planned but just threw things in as I went along so you
will no doubt find (if you study it) that code has been thrown into other
parts of the game. For example, the 'bike' functions will call the 'tree'
functions as well as the 'pickups' functions. So what! It all came together
in the end without any glitches. That's how the original programmers worked
from the old 8-bit days.

GDK and DBP seems to have a problem with PNG image files unless it is a
problem with ATI cards but at the moment I can't tell so I've added a
secondary project that converts PNG images to DDS images.

Originally all the code was not in classes and took over a day to convert
it all and with a lot of persistance I got it working without a rewrite.
From now on I will be using classes a lot more as they are handing for
storing variables within them without having a huge list of globals all
over the place, plus the intellisense within VS2008 picks them up when you
come to accessing them.

All over the internet you can find examples and tutorials for the STL
library. If you're writing a game, I'd suggest staying away from them but
make sure you code is tight and rock solid. In my code I've used standard
C type arrays instead of <vector> and <string> classes because when doing
umpteen thousands of calculations they slow your code down as compared to
standard C arrays.

/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////

IDEA:
~~~~

foreach ( tree_in_list )
{
	if ( dist < curr_dist )
	{
		curr_tree = tree_in_list;
		curr_dist = dist;
	}
}

do_collision_with_closest_tree();

// also make an array for the trees to store the x and z coords for faster access
// in assembler with a pointer to the beginning of the array.
// That way would speed up the collision check.
//

/////////////////////////////////////////////////////////////////////////////

distance = sqrt( ((x2-x1)*(x2-x1)) + ((y2-y1)*(y2-y1)) )

Take out the sqrt() part and you can use the same formula to find the closest
or furthest object much quicker. Then use the sqrt() to find the distance.

/////////////////////////////////////////////////////////////////////////////

90% of the programs time is only using 10% of the code. (90/10 law)

/////////////////////////////////////////////////////////////////////////////

Optimisations required for this game:

1. Load and store media in memory from the beginning.

	This is causing a bottleneck in performance when switching from the menu
	to the game as the media is getting dumped and then reloaded when the
	game starts and vice versa.
	
	When the game starts the only things that need processing is the setting
	up of the level. The media can be pre-loaded, trees, the bike, sounds
	(when they're added). Thus making the switch from menu to game faster.
	
	When the game ends the level can stay set up and only dumped when a new
	game starts.
	
	If at the beginning of the code all the references to media can be set
	to NULL and each time it may need loading then that reference can be
	double checked and loaded if it has been flushed from memory.
	
	ie. When it comes to loading the bike model, if the ID is NULL then load
		it in, if not then there's no need to re-load it.
		
		Only when the screen resolution has been changed then all these
		ID's will have to be reset to NULL.

2. Tree collision may (will) need optimising.

	Instead of calculating the distance for every tree in the scene, it
	would be much faster just to check the distances on the X and Z axis
	first. Currently only one distance check is actually using the sqrt()
	function.
	
	There was a slight frame drop when the timer based movement was
	implemented so this optimisation is most important.
	
	ie. Anything within the first rectangle to be checked can then be
		checked for distance collision. This will shave a lot of clock
		cycles on large batch collision checks.
		
		In assembler the 'imul' command uses a lot of cycles so if I can
		reduce that amount then the collision on 5000 or 15000 trees will
		speed up frame rates and give the oppurtunity to add more features.
		
		Currently the highest level has 12,000 trees.

3. Freeing up DirectX resources.

	In the TGC forums there's a subject on images not being loaded into
	memory when the game is run and I've also had the same problem whilst
	writing this.
	
	Hopefully some of the above methods may resolve this issue as media
	will be kept in memory. Still the issue with DBPro and GDK still seems
	to be with PNG's.

4. Add a lot more comments to code.

	In the future help with optimisation I will need to go through each
	night and add comments as the project is getting larger each day as
	more and more code is being added.
	
	This will be most unfortunate for others when the code is released as
	I'm not using easy coding techniques (ie C++ classes etc) for easier
	reading. Tons of my code is what us programmers call cheating, 
	especially when it comes to my collision techniques.

5. Timer base the menu screen.

	Yes, at last I come to think about this. It runs fine on my laptop but
	will probably go crazy on other faster computers so I will have to add
	this timer functionality to the menu even.

/////////////////////////////////////////////////////////////////////////////

Images converted to dds:

gascan.png
goldstar.png
heart.png

grad1.bmp

hudlayout.png

MainMenu.png

tree1.png to tree10.png

/////////////////////////////////////////////////////////////////////////////

TO DO:

1. Move the gradient texture generation into the load and unload functions.
	This will speed up the menu/game swapping...