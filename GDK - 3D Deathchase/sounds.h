#pragma once

#define SOUNDS_BIKEHIT		0
#define SOUNDS_HEALTHHIT	1
#define SOUNDS_STARHIT		2
#define SOUNDS_FUELHIT		3

#define SOUNDS_MAX			4

class SOUNDS
{
public:
	vector<int> soundID;

			SOUNDS(void);
			~SOUNDS(void);
	void	load();
	void	unload();
	int		free_sound();
	void	play(int s);
};
