#pragma once

class PLAYER
{
public:
	// players health
	//int		health_img;		// Image ID	*** UNUSED NOW
	//int		health_img_w;	// Width of the image
	//int		health_img_h;	// Height

	int		health;			// Actual health amount 0-100
	float	fuel;			// Fuel level 0-100

	int		fuel_count;		// how many fuel cans have been picked up
	int		health_count;	// how many healths have been picked up
	int		stars_count;

	int		hudimg;			// hud image
	int		hudtmp;			// blank image to use (same size as hud)
	int		hudobj;			// 3d object ID
	int		hudw, hudh;		// width and height of hud

	float	yang;			// used by the game over bit but don't work

	// players fuel

	PLAYER(void);
	~PLAYER(void);

	void	health_init(int sw, int sh);
	void	health_close();
	int		health_do(float sub_amount);	// Still the main function bah!

	void	copy_hud_img();	// copies image to image using DX
	void	do_hud();		// draws the hud and positions it
	void	game_over();	// runs if in game over mode

	void	draw_score();	// draws the score to the hud
	void	draw_count(int cnt, int yp);
};
