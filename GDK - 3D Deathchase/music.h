#pragma once

#include "bassmod.h"

class MUSIC
{
public:
	int		loaded;		// 1 if loaded ( 0 if not )

			MUSIC(void);
			~MUSIC(void);

	void	load_and_play( int m );
	void	stop();
};
