#include "Enemy.h"


Enemy::Enemy(void)
{
}

Enemy::Enemy(BITMAP *image, int cellWidth,int cellHeight, int animDelay, int animDirection,
		float locationX, float locationY, int initType, int initFacing, int initRotateDirection, int initAlive)
{
	spritesheet=image;
	width=cellWidth;height=cellHeight;
	row=0;column=0;
	animTime=0; delay=animDelay; if(delay==0) delay=1;
	animDir=animDirection;
	locX=locationX; locY=locationY;
	type=initType;
	facing=initFacing; if(type==0) if(facing==2) changeAnim(1);
	rotate_direction=initRotateDirection;
	alive=initAlive; life=3;
}
Enemy::~Enemy(void)
{
}

void Enemy::runAI(int time){
	//movement - depends on type
	switch(type){
		//Basic move forward then flip at wall behavior
		case 0:
			move(facing,1,0);
			if(collide_with_map()!=0){
				if(facing==2) facing=3;
				else facing=2;
				move(facing,1,0);
				nextRow();
			}
			break;
		//move along walls
		case 1:
			if(rotate_direction==0) //if moving clockwise
				switch(facing){
				//Each direction has a move order which will keep the enemy on whatever surface they are stuck to
				case 0: move_type1_helper(2,0,3,1); break; //if facing up -  order: L-U-R-D
				case 1: move_type1_helper(3,1,2,0); break; //if facing down - order: R-D-L-U
				case 2: move_type1_helper(1,2,0,3); break; //if facing left - order: D-L-U-R
				case 3: move_type1_helper(0,3,1,2); break; //if facing right - order: U-R-D-L
				}
			else //going counter clockwise changes the order
				switch(facing){
				case 0: move_type1_helper(3,0,2,1); break; //if facing up -  order: R-U-L-D
				case 1: move_type1_helper(2,1,3,0); break; //if facing down - order: L-D-R-U
				case 2: move_type1_helper(0,2,1,3); break; //if facing left - order: U-L-D-R
				case 3: move_type1_helper(1,3,0,2); break; //if facing right - order: D-R-U-L
				}
				changeAnim(facing+(4*rotate_direction));
				updateAnim(time);
			break; //end of type 1 movement
	}

}
	
//helps with moving enemies along walls
//checks to see if they can move in all four direction and if so sets their facing in that direction
int Enemy::move_type1_helper(int first, int second, int third, int fourth){
	int direction;
	int reverse_direction;
	for(int i=0;i<4;i++){
		switch(i){
			case 0: direction=first; break;
			case 1: direction=second; break;
			case 2: direction=third; break;
			case 3: direction=fourth; break;}
		switch(direction){
			case 0: reverse_direction=1; break;
			case 1: reverse_direction=0; break;
			case 2: reverse_direction=3; break;
			case 3: reverse_direction=2; break;}
		move(direction,1,0);//move the enemy in the specified direction
		if(collide_with_map()!=0) move(reverse_direction,1,0); //if they collide with the map move them back and continue with the for loop
		else{
			facing=direction;  //if they did not collide with the map, set their facing to the direction
			//if(direction==fourth) reverse(); //if this was the fourth direction then reverse the enemy's rotation dirction
			return 0; //getting here means this method is done, so return 0
		}
	}
	return 1; //getting here means it did not find a solution, so return 1
}

void Enemy::damage(int damage){
	if(type!=0){
		life-=damage;
	}
}

int Enemy::death_check(){
	if(life<=0) { alive=0; return 1;}
	return 0;
}

int Enemy::getType() { return type; }
void Enemy::setType(int t) { type=t; }
int Enemy::getFacing() { return facing; }
void Enemy::setFacing(int f) { facing=f; }
int Enemy::getRotateDirection() { return rotate_direction; }
void Enemy::setRotateDirection(int r) { rotate_direction=r; }
void Enemy::reverse(){ if(rotate_direction=0) rotate_direction=1; else rotate_direction=0; }