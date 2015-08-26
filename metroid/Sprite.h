/*
Project Metroid - Reused from Space invaders and heavily modified
Sprite.h - A class for sprite and sprite animation handling
Author Thomas Woods
Date 1-21-12
*/

#pragma once
#include <allegro.h>

class Sprite
{
protected:
	BITMAP *spritesheet; //the bitmap which contains all the sprite frames
	PALETTE palette; //the palette of the spritesheet
	int width,height; //holds the dimensions of the cells
	int row,column; //holds the location for the current frame of the sprite
	int animTime, delay; //delay holds off on animating the sprite for a certain amount of time since the last update, held by animTime
	int animDir; //direction tells the sprite if the animation plays via columns(0) or rows(1)
	float locX, locY; //the location on the screen or buffer to draw the sprite frame to
	int alive, life; //determines whether to display the sprite or not, if life<=0 then set alive to 0
	int state, facing; //used for determining behavior

public:
	Sprite(void);
	Sprite(BITMAP *image, int cellWidth, int cellHeight, int animDelay, int animationDirection,
		float locationX, float locationY);
	~Sprite(void);

	BITMAP* getSpriteFrame(); //gets a blit of the current frame
	BITMAP* getSpriteFrame(int getRow, int getColumn); //get a blit of a specific frame
	BITMAP* getSpriteSheet(); //returns the sprite sheet
	void setSpriteSheet(BITMAP* bmp); //returns the sprite sheet

	int getWidth(); //get the width and height of this sprite's frame
	int getHeight(); 
	void setWidth(int w); //set the width and height of this sprite's frame
	void setHeight(int h); 

	void updateAnim(int time); //update the animation, moving to the next frame
	void changeAnim(int animationNum); //change the animation to a specific animation number
	void changeFrame(int newRow, int newColumn); //change the frame to a specific rox/column
	int checkFrame(int checkRow, int checkColumn); //returns 1 if frame does not contain sprite, 0 otherwise
	void nextFrame(); //instantly changes to the next frame
	void setDelay(int d);
	void setTime(int t);
	void setAnimDir(int d);

	int getRow(); //return the row or column
	int getColumn();
	void setRow(int newRow); //set the row or column
	void setColumn(int newColumn);
	void nextRow(); //go to the next row or column or loop back if at the end
	void nextColumn();
	
	void move(float x, float y); //move to a specific location on the screen, does not change animation number
	void move(int dir, float distance, int anim); //move in a specific direction
	void draw(BITMAP *bmp); //draw to the bitmap at the location specified by the sprite
	void draw(BITMAP *bmp, int x, int y); //draw to the bitmap at the location specified by the call

	int collide_box(Sprite sprite); //check for box collision
	int collide_pixelperfect(Sprite sprite); //pixel perfect collision testing
	int isAlive(); //return or set the alive flag
	void setAlive(int a);
	int getLocX(); //return the x or y location, where the sprite is on the screen
	int getLocY();
	void setLoc(int x, int y); //set the sprites location

	void setFacing(int f);
	int getFacing();
	void setState(int s);
	int getState();
	void setLife(int l);
	int getLife();

	//map functions
	void draw_on_map(BITMAP *bmp, int offsetX, int offsetY);
	int collide_with_map();
	int collide_with_map_peixelperfect();
};

