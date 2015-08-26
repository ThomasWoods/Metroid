/*
Author: Thomas Woods
Date: January 30th 2012
Project: Metroid Clone
*/

#include <allegro.h>
#include "Game.h"

int main(void){
	allegro_init();
	install_keyboard();
	set_color_depth(16);
	set_gfx_mode(GFX_SAFE,480,448,0,0);
	install_timer();
	
	//start the game
	Game game=Game();
	game.setupGame();
	
	while(game.isRunning()) game.runGame();
	
	game.end();
	allegro_exit();
	return 0;
}
END_OF_MAIN()