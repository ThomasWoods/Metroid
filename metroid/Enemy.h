#pragma once
#include "sprite.h"
class Enemy : public Sprite
{
protected:
	int type;
	int rotate_direction; //0 for clockwise, 1 for counterclockwise
public:
	Enemy(void);
	Enemy(BITMAP *image, int cellWidth,int cellHeight, int animDelay, int animDirection,
		float locationX, float locationY, int initType, int initFacing, int initRotateDirection, int initAlive);
	~Enemy(void);

	void runAI(int time);
	int move_type1_helper(int first, int second, int third, int fourth);

	void damage(int damage);
	int death_check();

	int getType();
	void setType(int t);
	int getFacing();
	void setFacing(int f);
	int getRotateDirection();
	void setRotateDirection(int r);
	void reverse();
};

