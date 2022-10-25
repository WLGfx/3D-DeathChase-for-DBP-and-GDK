#pragma once

class BIKE
{
public:
	int		obj;			// bike object ID

	float	xpos, ypos;		// Bikes current X and 'Z' coords
	float	angle;			// direction of bike
	float	turnspd;		// current turning speed (intertia controlled)
	float	speed;			// bikes speed
	float	height;			// current height on the ground

	int		game_timer;		// Timer based control (might be elsewhere now)

	BIKE(void);
	~BIKE(void);

	void	load();
	void	close();
	void	mat_pos();
	void	turn();
	void	set_pos(float x, float y);
	void	set_angle_and_speed(float ang, float spd);
	void	init_height();
	void	move();
	void	set_angle();
	int		do_all();
	void	init_all(int spd);
};
