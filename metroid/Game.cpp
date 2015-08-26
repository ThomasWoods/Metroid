#include "Game.h"
#include "mappyal.h"


Game::Game(void)
{
	setupGame();
}

Game::Game(Samus setSamus, int setMap)
{
	setupGame();
	current_map=setMap, current_map_width=71, current_map_height=15, tilesize=16;
	samus=setSamus;
}


Game::~Game(void)
{
}

int Game::setupGame(){
	buffer=create_bitmap(256, 240);
	delay=17;
	start_time=0; end_time=0; game_time=0;
	start=0; ticks=0; framerate=0;
	current_map=-1, current_map_width=0, current_map_height=0, tilesize=16; //multiple maps not yet supported
	xoffset = 384, yoffset = 0;
	up=0,down=0,left=0,right=0,space=0,key_a=0;
	start_game=0; gameover=0, end_game=0;
	srand ( time(NULL) );

	samus_bmp=load_bitmap("samus.bmp",palette);			if(!samus_bmp) { allegro_message("Could not create bitmap: samus_bmp"); return 1; }
	shot_bmp=load_bitmap("shot.bmp",palette);			if(!shot_bmp) { allegro_message("Could not create bitmap: shot_bmp"); return 1; }
	item_bmp=load_bitmap("items.bmp",palette);			if(!item_bmp) { allegro_message("Could not create bitmap: item_bmp"); return 1; }
	explosion_bmp=load_bitmap("explosion.bmp",palette);	if(!explosion_bmp) { allegro_message("Could not create bitmap: explosion_bmp"); return 1; }
	floater_bmp=load_bitmap("floaters.bmp",palette);	if(!floater_bmp) { allegro_message("Could not create bitmap: floater_bmp"); return 1; }
	spike_bmp=load_bitmap("spike.bmp",palette);			if(!spike_bmp) { allegro_message("Could not create bitmap: spike_bmp"); return 1; }
	menu_bmp=load_bitmap("metroid start.bmp",palette);	if(!menu_bmp) { allegro_message("Could not create bitmap: menu_bmp"); return 1; }
	gameover_bmp=load_bitmap("gameover.bmp",palette);	if(!gameover_bmp) { allegro_message("Could not create bitmap: gameover_bmp"); return 1; }
	gui_bmp=load_bitmap("gui.bmp",palette);				if(!gui_bmp) { allegro_message("Could not create bitmap: gui_bmp"); return 1; }

	samus=Samus(samus_bmp, 0, 0);
	for(int i=0;i<15;i++) enemy[i]=Enemy(floater_bmp, 16,8, 100, 0, 0,0, 0, 0, 0, -1);
	for(int i=0;i<3;i++){  shot[i]=Sprite(shot_bmp, 12, 12, 100, 0, 0, 0); shot[i].setAlive(0); }
	for(int i=0;i<3;i++){  item[i]=Sprite(item_bmp, 8, 12, 10, 0, 0, 0); item[i].setAlive(0); }
	explosion=Sprite(explosion_bmp,32,32,100,0,0,0);

	load_map(0);

	return 0;
}

void Game::runGame(){
	//start the game with the start screen
	while(start_game==0){
		stretch_blit(menu_bmp,screen,0,0,menu_bmp->w,menu_bmp->h,0,0,SCREEN_W,SCREEN_H);
		if(key[KEY_ENTER] || key[KEY_ESC]) start_game=1;
	}

	start_time=clock(); //time the game logic - start

	//main processes
	checkInput();
	gameLogic(start_time);
	drawGame();

	end_time=clock();
	game_time=end_time-start_time;

	//game rest
	if(game_time>=delay){ rest(game_time); } //rest for game time +1 if it game time is greater than delay
	else rest(delay-game_time); //otherwise rest for delay-game time

	//when the player dies show the game over screen
	if(gameover==1) load_map(-1); //reset
	while(gameover==1){ //wait
		stretch_blit(gameover_bmp,screen,0,0,menu_bmp->w,menu_bmp->h,0,0,SCREEN_W,SCREEN_H);
		if(key[KEY_ENTER] || key[KEY_ESC]) gameover=0;
	}
	
	//game end code here so it happens after all the code runs
	if(samus.check_gameover()==999)
		end_game=1;
	if(key[KEY_ESC])
		end_game=1;
	while(end_game && !key[KEY_ESC]){
		textout_centre_ex(buffer,font,"Thank you for playing!",120,120,makecol(255,255,255),-1);
		stretch_blit(buffer,screen,8,8,224,224,0,0,SCREEN_W,SCREEN_H); 
		clear_bitmap(buffer);
	}
}

void Game::checkInput(){
	up=0,down=0,left=0,right=0,space=0;key_a=0;
	if(key[KEY_UP]){ up=1;}
	if(key[KEY_DOWN]) { down=1; }
	if(key[KEY_LEFT]) { left=1; }
	if(key[KEY_RIGHT]) { right=1; } 
	if(key[KEY_SPACE]){ space=1; }
	if(key[KEY_A]){ key_a=1; }
}

void Game::gameLogic(int time){
	//samus updating
	samus.updateState(up,down,left,right,space,key_a,start_time);
	samus.updateAnim(start_time);
	samus.itemCheck();
	if(samus.deathCheck()){
		gameover=1;
		return;
	}
	int doorCheck=samus.doorCheck();
	if(doorCheck>=0) load_map(doorCheck);

	//Camera update
	xoffset=samus.getLocX()+(samus.getWidth()/2)-(buffer->w/2);
	if (xoffset < 0) xoffset = 0;
	if (xoffset > 16*current_map_width-256) xoffset = 16*current_map_width-256;
	yoffset=samus.getLocY()+(samus.getHeight()/2)-(buffer->h/2);
	if (yoffset < 0) yoffset = 0;
	if (yoffset > 16*current_map_height-240) yoffset = 16*current_map_height-240;

	//Enemy updates
	for(int i=0;i<15;i++){
		if(enemy[i].isAlive())
			if(enemy[i].death_check()==1){ 
				//create an explosion
				explosion.setLoc(enemy[i].getLocX()-(32-enemy[i].getWidth())/2,enemy[i].getLocY()-(32-enemy[i].getHeight())/2);
				explosion.setAlive(1); explosion.setColumn(0);
				//item creation
				if(rand()%2==0)
					for(int i2=0;i2<3;i2++) //find an empty item and fill it in
						if(item[i2].isAlive()==0) { 
							item[i2].setLoc(enemy[i].getLocX()-(8-enemy[i].getWidth())/2,enemy[i].getLocY()-(8-enemy[i].getHeight())/2);
							item[i2].setAlive(1); item[i2].setLife(time);
							i2=3;
						}
			}
			else{
				enemy[i].runAI(time);
				if(enemy[i].collide_box(samus)>=8){ samus.damage(4); }
			}
	}
	//update the explosion
	if(explosion.isAlive()){
		explosion.updateAnim(time);
		if(explosion.getColumn()==0) explosion.setAlive(0);
	}
	//update the items
	for(int inum=0;inum<3;inum++) {
		if(item[inum].isAlive()==1){
			item[inum].updateAnim(time);
			if(item[inum].collide_box(samus)){ 
				item[inum].setAlive(0);
				samus.heal(5);
			}
			if(time>item[inum].getLife()+3500) 
				item[inum].setAlive(0);
		}
	}

	//shot updates
	//setup a new shot
	int facing=samus.get_shot(); //get the qued shot
	if(facing>=0) //if it isn't -1 (hasn't fired)
		for(int i=0;i<3;i++) //find an empty shot and fill it in
			if(!shot[i].isAlive() && shot[i].getColumn()==0) { 
				shot[i].setAlive(1); shot[i].setLife(time);
				switch(facing){
				case 0: shot[i].setLoc(samus.getLocX()+5,  samus.getLocY()); break;
				case 1: shot[i].setLoc(samus.getLocX()+8,  samus.getLocY()); facing=0; break;
				case 2: shot[i].setLoc(samus.getLocX()-2,  samus.getLocY()+12); break;
				case 3: shot[i].setLoc(samus.getLocX()+16, samus.getLocY()+12); break;}
				shot[i].setFacing(facing); i=3; }
	//move shot, check for collisions
	for(int i=0;i<3;i++)
		if(shot[i].isAlive())
			for(int distance=0;distance<3;distance++){
				shot[i].move(shot[i].getFacing(),1,0);
				if(time>shot[i].getLife()+250) { shot[i].setAlive(0); }
				if(shot[i].isAlive() && shot[i].collide_with_map_peixelperfect()){
					shot[i].setAlive(0);
					shot[i].setColumn(1);
				}
				for(int e=0;e<15;e++)
					if(shot[i].isAlive() && enemy[e].isAlive() && shot[i].collide_box(enemy[e])>0){
						shot[i].setAlive(0);
						enemy[e].damage(1);
						shot[i].setColumn(1);
					}
			}
		else if(shot[i].getColumn()>0) shot[i].updateAnim(time);
}

void Game::drawGame(){
	//draw enemies
	for(int i=0;i<15;i++) 
		if(enemy[i].isAlive()) 
			enemy[i].draw_on_map(buffer,xoffset,yoffset);
	//draw shots
	for(int i=0;i<3;i++) 
		if(shot[i].isAlive() || shot[i].getColumn()>0) 
			shot[i].draw_on_map(buffer,xoffset,yoffset);
	//draw items
	for(int i=0;i<3;i++) 
		if(item[i].isAlive()) 
			item[i].draw_on_map(buffer,xoffset,yoffset);
	//draw player
	samus.draw_on_map(buffer,xoffset,yoffset);
	//draw the map
	MapDrawFG(buffer, xoffset, yoffset, 0, 0, 256, 240, 1);
	//draw the explosion
	if(explosion.isAlive()) explosion.draw_on_map(buffer,xoffset,yoffset);

	//draw the gui
	masked_blit(gui_bmp,buffer,0,0,32,34,24,8);
	textprintf_ex(buffer,font,56,34,makecol(255,255,255),-1,"%d", samus.getLife());

	//draw the buffer to the screen then empty the buffer- double buffering
	stretch_blit(buffer,screen,8,8,240,224,0,0,SCREEN_W,SCREEN_H);
	clear_bitmap(buffer);
}

int Game::load_map(int map_number){
	int scroll_direction=0;
	BITMAP *firstmap, *secondmap;
	firstmap=create_bitmap(256, 240); 
	secondmap=create_bitmap(256, 240);
	MapDrawFG(firstmap, xoffset, yoffset, 0, 0, 256, 240, 1);

	switch(map_number){ //should really reference a database of some sort...
		default: //to reset game enter anything less than 0
			if(MapLoad("metroid.fmp")) { allegro_message ("Can't find metroid.fmp"); return 1; } 
			for(int i=0; i<15; i++) enemy[i]=Enemy(floater_bmp, 16,8, 100, 0, 0,0, 0, 0, 0, -1);
			enemy[0]=Enemy(floater_bmp, 16,8, 100, 0, 17*16,12*16, 0, 3, 0, 1);
			enemy[1]=Enemy(floater_bmp, 16,8, 100, 0, 35*16, 8*16, 0, 3, 0, 1);
			enemy[2]=Enemy(spike_bmp,  16,16, 100, 0, 28*16, 2*16, 1, 3, 1, 1);
			enemy[3]=Enemy(spike_bmp,  16,16, 100, 0, 34*16, 2*16, 1, 3, 1, 1);
			samus.setLoc(31*16, 9*16); scroll_direction=0; map_number=1;
			current_map_width=71, current_map_height=15;
			break;
		case 0: 
			if(MapLoad("metroid.fmp")) { allegro_message ("Can't find metroid.fmp"); return 1; } 
			for(int i=0; i<15; i++) enemy[i]=Enemy(floater_bmp, 16,8, 100, 0, 0,0, 0, 0, 0, -1);
			enemy[0]=Enemy(floater_bmp, 16,8, 100, 0, 17*16,12*16, 0, 3, 0, 1);
			enemy[1]=Enemy(floater_bmp, 16,8, 100, 0, 35*16, 8*16, 0, 3, 0, 1);
			enemy[2]=Enemy(spike_bmp,  16,16, 100, 0, 28*16, 2*16, 1, 3, 1, 1);
			enemy[3]=Enemy(spike_bmp,  16,16, 100, 0, 34*16, 2*16, 1, 3, 1, 1);
			if(current_map!=1){ samus.setLoc(31*16, 9*16);  scroll_direction=0; }
			if(current_map==1){ samus.setLoc(68*16, 8*16-samus.getHeight()); scroll_direction=3; }
			current_map_width=71, current_map_height=15;
			break;
		case 1: 
			if(MapLoad("map1.fmp")) { allegro_message ("Can't find map1.fmp"); return 1; } 
			for(int i=0; i<15; i++) enemy[i]=Enemy(floater_bmp, 16,8, 100, 0, 0,0, 0, 0, 0, -1);
			enemy[0]=Enemy(spike_bmp, 16,16, 100, 0, 8*16,5*16, 1, 3, 1, 1);
			enemy[1]=Enemy(floater_bmp, 16,8, 100, 0, 8*16,13*16, 0, 3, 0, 1);
			if(current_map==0){ samus.setLoc(2*16, 8*16-samus.getHeight()); scroll_direction=2; }
			if(current_map==2){ samus.setLoc(13*16, 8*16-samus.getHeight()); scroll_direction=3; }
			current_map_width=16, current_map_height=15;
			break;
		case 2: 
			if(MapLoad("map2.fmp")) { allegro_message ("Can't find map2.fmp"); return 1; } 
			for(int i=0; i<15; i++) enemy[i]=Enemy(floater_bmp, 16,8, 100, 0, 0,0, 0, 0, 0, -1);
			enemy[0]=Enemy(spike_bmp, 16,16, 100, 0, 2*16,12*16, 1, 2, 0, 1);
			enemy[1]=Enemy(spike_bmp, 16,16, 100, 0, 42*16,12*16, 1, 2, 0, 1);
			enemy[2]=Enemy(spike_bmp, 16,16, 100, 0, 44*16,3*16, 1, 0, 0, 1);
			enemy[4]=Enemy(floater_bmp, 16,8, 100, 0, 61*16,12*16, 0, 2, 0, 1);
			if(current_map==1){ samus.setLoc(2*16, 8*16-samus.getHeight()); scroll_direction=2; }
			if(current_map==3){ samus.setLoc(13*16, 8*16-samus.getHeight()); scroll_direction=3; }
			current_map_width=64, current_map_height=15;
			break;
	}
	
	if(scroll_direction==2) MapDrawFG(secondmap, 0, 0, 0, 0, 256, 240, 1);
	if(scroll_direction==3) MapDrawFG(secondmap, current_map_width*16-256, 0, 0, 0, 256, 240, 1);
	map_hTransition(firstmap, secondmap, scroll_direction);

	destroy_bitmap(firstmap);destroy_bitmap(secondmap);

	current_map=map_number;
	return 0;
}

void Game::map_hTransition(BITMAP *firstmap, BITMAP *secondmap, int scroll_direction){
	if(scroll_direction!=2 && scroll_direction!=3) return;
	int transition_progress=0;
	while(transition_progress<256){
		transition_progress+=3;
		if(scroll_direction==2){//scroll to the left
			blit(firstmap,buffer,transition_progress,0,0,0,256-transition_progress,240);
			blit(secondmap,buffer,0,0,256-transition_progress,0,transition_progress,240);
		}
		if(scroll_direction==3){//scroll to the right
			blit(firstmap,buffer,0,0,transition_progress,0,256-transition_progress,240);
			blit(secondmap,buffer,256-transition_progress,0,0,0,transition_progress,240);
		}

		stretch_blit(buffer,screen,8,8,240,224,0,0,SCREEN_W,SCREEN_H);
		clear(buffer);
		rest(17);
	}
}


int Game::isRunning(){
	if(!end_game) return 1;
	else return 0;
}

void Game::end(){
	destroy_bitmap(samus_bmp);
	destroy_bitmap(floater_bmp);
	destroy_bitmap(buffer);
	MapFreeMem();
}