#pragma once
#include "sprite.h"
#include <allegro.h>

class Samus : public Sprite
{
protected:
	int jump_timer, jump_time, fall_timer; 
	int pup, pdown, pleft, pright, pspace, pkey_a;// previous key states
	int invicibility_timer; //for temp invicibility after taking damage
	int fired;
	int morphball, armor;
public:
	Samus(void);
	Samus(BITMAP *image, float locationX, float locationY);
	~Samus(void);

	void updateState(int up, int down,int left, int right, int space, int key_a, int time);
	void changeState(int newState);
	void jump(int direction, int time, int right, int left, int space);
	int get_shot();

	void draw_on_map(BITMAP *bmp, int offsetX, int offsetY);
	int collide_with_map();
	int morphball_collide_with_map();

	void itemCheck();
	int doorCheck();
	void damage(int damage);
	void heal(int heal);
	int deathCheck();
	int check_gameover();
};

