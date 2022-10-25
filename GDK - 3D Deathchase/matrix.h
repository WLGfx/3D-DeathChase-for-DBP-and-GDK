#pragma once

class MATRIX
{
public:
	int		mtext;				// matrix texture image ID

	int		matcw, match;		// cell width and height in vertex points
	int		matwid, mathgt;		// overall width and height
	int		matxs, matys;		// cell size actual

	int		mathalfx, mathalfy;	// halfway points

	int		mat_skydome;		// this is repositioned in the bike function
	int		sd_text;			// skydome texture

	int		water_obj;			// water plane ID

			MATRIX(void);
			~MATRIX(void);

	void	load();
	void	unload();
	void	init(int w, int h, int ow, int oh, int trees, int oct);
	void	paste_mtext();
	void	calc_normals();
	void	close();
	void	run();				// OBSELETE
	void	make_texture_grad();// OBSELETE
	void	make_texture2();
	void	make_texture();		// OBSELETE
	void	make_skydome_texture(int size);
	void	make_water();
	void	unmake_water();
	void	position_water();
};
