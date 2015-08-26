#pragma once

#include <allegro.h>
#include "Sprite.h"
#include "Samus.h"
#include "Enemy.h"

class Game
{
protected:
	BITMAP *buffer;
	int delay;
	int start_time, end_time, game_time;
	int start, ticks, framerate;
	int current_map, current_map_width, current_map_height, tilesize;
	int xoffset, yoffset;
	int up,down,left,right,space,key_a;
	int start_game, gameover, end_game;
	PALETTE palette;
	BITMAP *samus_bmp, *shot_bmp, *item_bmp, *explosion_bmp; //player related sprites
	BITMAP *floater_bmp, *spike_bmp, *hopper_bmp, *diver_bmp; //enemy sprites
	BITMAP *menu_bmp, *gameover_bmp, *gui_bmp; //game sprites
	Samus samus;
	Enemy enemy[15];
	Sprite explosion;
	Sprite shot[3];
	Sprite item[3];

public:
	Game(void);
	Game(Samus setSamus, int setMap);
	~Game(void);

	int setupGame();
	void runGame();

	void checkInput();
	void gameLogic(int time);
	void drawGame();
	
	int load_map(int map_number);
	void map_hTransition(BITMAP *firstmap, BITMAP *secondmap, int type);
	void map_vTransition(BITMAP *firstmap, BITMAP *secondmap, int type);

	int isRunning();
	void reset();
	void end();
};

