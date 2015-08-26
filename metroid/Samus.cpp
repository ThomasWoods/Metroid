/*Samus' Possible States, numbers also correspond to animation numbers: 
(0)Intro, (1)Standing Left, (2)Standing Right, (3)Standing Left Looking Up, (4)Standing Right Looking Up, 
(5)Running Left, (6)Running Left and Looking Up, (7)Running Left and Holding Gun Out, (8)Running Right, (9)Running Right and Looking Up, (10)Running Right and Holding Gun Out, 
(11)Jump A Right, (12)Jump A Right Shooting, (13)Jump A Right Looking Up, (14)Jump A Left, (15)Jump A Left Shooting, (16)Jump A Left Looking Up, (17)Jump B Right, (18)Jump B Left, 
(19)Morph Ball*/
#include "Samus.h"
#include "mappyal.h"


Samus::Samus(void)
{
}

Samus::Samus(BITMAP *image,	float locationX, float locationY){
	spritesheet=image;
	locX=locationX; locY=locationY;
	width=25;height=40;
	row=0;column=0;
	animTime=0; delay=50; animDir=0;
	jump_timer=0; jump_time=700; fall_timer=0; 
	state=0; life=30; invicibility_timer=0;
	morphball=0; armor=0;
}

Samus::~Samus(void)
{
}

void Samus::updateState(int up, int down, int left, int right, int space, int key_a, int time){
		//check for keyboard input
		if (left) {  switch(state){
				case 0: state=5; break;
				case 1: state=5; break;
				case 2: state=5; break;
				case 3: state=5; break;
				case 4: state=5; break;
				case 11: state=14; break;
				case 12: state=14; break;
				case 13: state=14; break;
			} }
		//if they release a key stop being in that state
		else { if(state>=5 && state<=7) state=1; }

		if (right) { switch(state){
				case 0: state=8; break;
				case 1: state=8; break;
				case 2: state=8; break;
				case 3: state=8; break;
				case 4: state=8; break;
				case 14: state=11; break;
				case 15: state=11; break;
				case 16: state=11; break;
			} } 
		else { if(state>=8 && state<=10) state=2; }

		if (up) { switch(state){
			case 1: state=3; break;
			case 2: state=4; break;
			case 5: state=6; break;
			case 8: state=9; break;
			case 11: state=13; break;
			case 12: state=13; break;
			case 14: state=16; break;
			case 15: state=16; break;
			case 19: state=1; break;
			} }
		else {  switch(state){
				case 3: state=1; break;
				case 4: state=2; break;
				case 6: state=5; break;
				case 9: state=8; break;
				case 13: state=11; break;
				case 16: state=14; break;
			} }

		if(space && !pspace){
			//if the player is standing left, standing left and looking up, or moving left
			if(state==1 || state==3 || (state>=5 && state<=7)){
				state=14;	//set their state to jumping left and set the jump timer
				jump_timer=time;
			}
			//if the player is standing right, standing right and looking up, or moving right
			if(state==2 || state==4 || (state>=8 && state<=10)){
				state=11;	//set the player state to jumping right
				jump_timer=time;
			}
			if(state==19) state=1;
		}

		if (down) { 
			if(state<10 && morphball==1){ 
				state=19;
			}
		}

		if(key_a && !pkey_a){
			if(state==3 || state==6 || state==16) fired=0; //up, left side
			else if(state==4 || state==9 || state==13) fired=1; //up, right side
			else if(state==1 || state==5 || state==7 || state==14 || state==16) fired=2; //left
			else if(state==2 || state==8 || state==10 || state==11 || state==13) fired=3; //right
			else fired=-1;
		}
		else fired=-1;

		//movement switch
		switch(state){
			case 0: break; //intro
			case 1: break; //Standing Left
			case 2: break; //Standing Right
			case 3: break; //Standing Left Looking Up
			case 4: break; //Standing Right Looking Up
				//doing a for loop rather than moving more than one pixel per call ensures proper collision - Could be redone through collision method, but this works
			case 5: for(int i=0;i<2;i++){ move(2,0.85,0); if(collide_with_map()!=0) move(3,0.85,0); } break; //Running Left
			case 6: for(int i=0;i<2;i++){ move(2,0.85,0); if(collide_with_map()!=0) move(3,0.85,0); } break; //Running Left and Looking Up
			case 7: for(int i=0;i<2;i++){ move(2,0.85,0); if(collide_with_map()!=0) move(3,0.85,0); } break; //Running Left and Holding Gun Out
			case 8: for(int i=0;i<2;i++){ move(3,0.85,0); if(collide_with_map()!=0) move(2,0.85,0); } break; //Running Right
			case 9: for(int i=0;i<2;i++){ move(3,0.85,0); if(collide_with_map()!=0) move(2,0.85,0); } break; //Running Right and Looking Up
			case 10: for(int i=0;i<2;i++){ move(3,0.85,0); if(collide_with_map()!=0) move(2,0.85,0); } break; //Running Right and Holding Gun Out
			case 11: jump(3,time,right,left,space);break; //Jump A Right
			case 12: jump(3,time,right,left,space);break; //Jump A Right Shooting
			case 13: jump(3,time,right,left,space);break; //Jump A Right Looking Up
			case 14: jump(2,time,right,left,space);break; //Jump A Left
			case 15: jump(2,time,right,left,space);break; //Jump A Left Shooting
			case 16: jump(2,time,right,left,space);break; //Jump A Left Looking Up
			case 17: break; //Jump B Right
			case 18: break; //Jump B Left
			case 19: //Morph Ball
				if(left==1) for(int i=0;i<2;i++){ move(2,0.85,0); if(morphball_collide_with_map()!=0) move(3,0.85,0); } 
				if(right==1) for(int i=0;i<2;i++){ move(3,0.85,0); if(morphball_collide_with_map()!=0) move(2,0.85,0); }
				break; 
		}
		
		//gravity
		move(1,1,0);//try to move the player down
		if(collide_with_map()!=0){ //if they collide then they are on the ground
			move(0,1,0); //put them back where they were
			if(state>=11 && state<=13 && time>jump_timer+10) //take them out of jumping state
				state=2;
			if(state>=14 && state<=16 && time>jump_timer+10)
				state=1;
			fall_timer=0; //reset the fall timer
		}
		else{ //if they didn't collide it means they are either jumping or falling
			if(state==1 || state==3 || (state>=5 && state<=7)) state=14; //set their state to jumping
			if(state==2 || state==4 || (state>=8 && state<=10)) state=11;
			if(fall_timer==0 && (time>jump_timer+jump_time || space==0)) //if the player is not already falling, and is either out of jump time or not holding jump
				fall_timer=clock(); //start the fall
			if(fall_timer>0){
				int distance=(time-fall_timer+10)/100; //increase how fast they fall over time
				//if(distance>10) distance=10; //up to a max of 10 pixels per frame
				for(int i=0;i<distance;i++){ 
					move(1,1,0); 
					if(collide_with_map()!=0) { //this is hwere they hit the ground
						move(0,1,0); 
						//morphball bounce code here
					}
				}
			}
		}

		//update samus' animation to reflect her new state
		changeAnim(state);
		//make sure it doesn't update to a blank frame
		if(!checkFrame(row, column)) nextFrame();
		//set the previous keystates for next update
		pup=up; pdown=down; pleft=left; pright=right; pspace=space; pkey_a=key_a;
		//invicibility timer counts down instead of is a set time so I can use it when I don't have the time. Probably should do this to all of my timers...
		if(invicibility_timer) invicibility_timer--;
}

void Samus::changeState(int newState){ state=newState; }

void Samus::jump(int direction, int time, int right, int left, int space){
	if(direction=2 && left==1) for(int i=0;i<2;i++){ move(2,0.75,0); if(collide_with_map()!=0) move(3,0.75,0); } //if the player is holding the right direction, move the player sprite to the right
	if(direction=3 && right==1) for(int i=0;i<2;i++){ move(3,0.75,0); if(collide_with_map()!=0) move(2,0.75,0); } //if the player is holding the right direction, move the player sprite to the right
	if(time<jump_timer+jump_time && space==1){ //if it is still within the jump timeframe and the player is holding jump
		int distance=(jump_time-(time-jump_timer))/100;
		for(int i=0;i<distance;i++){ 
			move(0,1,0);  //move the player sprite up
			if(collide_with_map()!=0) { move(1,1,0); jump_timer=0; } //if the player collides with the cieling, end their jump
		}
	}
}

int Samus::get_shot(){
	return fired;
}

void Samus::draw_on_map(BITMAP *bmp, int offsetX, int offsetY){
	if(invicibility_timer==0 || invicibility_timer%3!=0) 
		masked_blit(spritesheet, bmp, 
			column*width, row*height,
			locX-offsetX,locY-offsetY,width,height);
}

int Samus::collide_with_map(){
	BITMAP *sprite=getSpriteFrame();
	BLKSTR *blockdata;
	int flag=0;
	if(state==19) flag=morphball_collide_with_map();
	else
		for(int w=9;w<width-9;w++)
			for(int h=12;h<height-1;h++){
					blockdata=MapGetBlock((locX+w)/mapblockwidth,(locY+h)/mapblockheight);
					if(flag==0)flag=blockdata->tl;if(flag==0)flag=blockdata->tr;
					if(flag==0)flag=blockdata->bl;if(flag==0)flag=blockdata->br;
				}
	destroy_bitmap(sprite);
	return flag;
}

int Samus::morphball_collide_with_map(){
	BITMAP *sprite=getSpriteFrame();
	BLKSTR *blockdata;
	int flag=0;
	for(int w=9;w<width-9;w++)
		for(int h=28;h<height-1;h++)
		{
			blockdata=MapGetBlock((locX+w)/mapblockwidth,(locY+h)/mapblockheight);
			if(flag==0)flag=blockdata->tl;if(flag==0)flag=blockdata->tr;
			if(flag==0)flag=blockdata->bl;if(flag==0)flag=blockdata->br;
		}
	destroy_bitmap(sprite);
	return flag;
}

void Samus::itemCheck(){
	BITMAP *sprite=getSpriteFrame();
	BLKSTR *blockdata;
	for(int w=9;w<width-9;w++)
		for(int h=12;h<height-1;h++){
			blockdata=MapGetBlock((locX+w)/mapblockwidth,(locY+h)/mapblockheight);
			if(blockdata->user1==1){ 
				morphball=1;
				MapSetBlock((locX+w)/mapblockwidth,(locY+h)/mapblockheight,0);
			}
		}
	destroy_bitmap(sprite);
}

int Samus::doorCheck(){
	BITMAP *sprite=getSpriteFrame();
	BLKSTR *blockdata;
	for(int w=-1;w<width+1;w++)
		for(int h=-1;h<height+1;h++){
			blockdata=MapGetBlock((locX+w)/mapblockwidth,(locY+h)/mapblockheight);
			if(blockdata->user1==888){ 
				int ret=blockdata->user2;
				return ret;
			}
		}
	destroy_bitmap(sprite);
	return -1;
}

void Samus::damage(int damage){
	if(invicibility_timer==0){
		if(damage-armor>0)
			life-=damage-armor; //can add multipliers from armor from here
		else 
			life--;
		invicibility_timer=45;
	}
}
void Samus::heal(int heal){
	life+=heal;
}

int Samus::deathCheck(){
	if(life<=0){
		changeState(0);
		setLoc(31*16,9*16);
		life=30;
		return 1;
	}
	return 0;
}

int Samus::check_gameover(){
	BITMAP *sprite=getSpriteFrame();
	BLKSTR *blockdata;
	int flag=0;
	for(int w=-1;w<width;w++)
		for(int h=-1;h<height;h++){
			blockdata=MapGetBlock((locX+w)/mapblockwidth,(locY+h)/mapblockheight);
			if(blockdata->user1==999) flag=999;
		}
	return flag;
}