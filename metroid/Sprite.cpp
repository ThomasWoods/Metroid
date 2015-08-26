/*
Project Metroid - Reused from Space invaders and heavily modified
Sprite.cpp
Author Thomas Woods
Date 1-21-12
*/

#include "Sprite.h"
#include "mappyal.h"

Sprite::Sprite(void)
{
}

Sprite::Sprite(BITMAP *image, int cellWidth,int cellHeight, int animDelay, int animDirection,
		float locationX, float locationY){
	spritesheet=image;
	width=cellWidth;height=cellHeight;
	row=0;column=0;
	animTime=0; delay=animDelay; if(delay==0) delay=1;
	animDir=animDirection;
	locX=locationX; locY=locationY;
	alive=0; life=0;
	state=0; facing=0;
}

Sprite::~Sprite(void)
{
}


BITMAP* Sprite::getSpriteFrame(){
	BITMAP *sprite=create_bitmap(width,height);
	blit(spritesheet, sprite, 
		column*width, row*height,
		0,0,width,height);
	return sprite;
	destroy_bitmap(sprite);
}

BITMAP* Sprite::getSpriteFrame(int frameX, int frameY){
	BITMAP *sprite=create_bitmap(width,height);
	blit(spritesheet, sprite, 
		column*width, row*height,
		0,0,width,height);
	return sprite;
	destroy_bitmap(sprite);
}

BITMAP* Sprite::getSpriteSheet(){ return spritesheet; }
void Sprite::setSpriteSheet(BITMAP *bmp) {spritesheet=bmp; }

int Sprite::getWidth() { return width; }
int Sprite::getHeight() { return height; }
void Sprite::setWidth(int w) { width=w; }
void Sprite::setHeight(int h) { height=h; }


void Sprite::updateAnim(int time){ 
	if(time>animTime+delay){
		animTime=time;
		if(animDir==0) nextColumn();
		else nextRow();
	}
}

void Sprite::changeAnim(int animationNum){
	if(animDir==0){ if(getRow()!=animationNum) setRow(animationNum);}
	else{ if(getColumn()!=animationNum) setColumn(animationNum);}
}

void Sprite::changeFrame(int newRow, int newColumn){ row=newRow; column=newColumn; }

int Sprite::checkFrame(int chackRow, int checkColumn){
	BITMAP *sprite=getSpriteFrame(chackRow,checkColumn);
	int mask_color=makecol(255,0,255);
	int flag=0;
	for(int w=0;w<width;w++)
		for(int h=0;h<height;h++)
			if(getpixel(sprite,w,h)!=mask_color) flag=1;
	destroy_bitmap(sprite);
	return flag;
}

void Sprite::nextFrame(){ 
	if(animDir==0) nextColumn();
	else nextRow();
}
void Sprite::setDelay(int d){ delay=d; }
void Sprite::setTime(int t){ animTime=t; }
void Sprite::setAnimDir(int d){animDir=d; }

int Sprite::getRow() { return row; }
int Sprite::getColumn() { return column; }
void Sprite::setRow(int newRow) { row=newRow; }
void Sprite::setColumn(int newColumn) { column=newColumn; }

void Sprite::nextRow(){
	if((row+1)*height+height<=spritesheet->h) 
		row++;
	else row=0;
	if(checkFrame(row,column)==0) nextRow();
}
void Sprite::nextColumn(){
	if((column+1)*width+width<=spritesheet->w) 
		column++;
	else column=0;
	if(checkFrame(row,column)==0) nextColumn();
}

void Sprite::move(float x, float y){
	locX=x;locY=y;
}

void Sprite::move(int dir, float distance, int anim){
	switch(dir)
	{
		case 0: locY-=distance; break;
		case 1: locY+=distance; break;
		case 2: locX-=distance; break;
		case 3: locX+=distance; break;
	}
	if(anim)changeAnim(dir);
}

void Sprite::draw(BITMAP *bmp){
	masked_blit(spritesheet, bmp, 
		column*width, row*height,
		locX,locY,width,height);
}
void Sprite::draw(BITMAP *bmp, int x, int y){
	masked_blit(spritesheet, bmp, 
		column*width, row*height,
		x,y,width,height);
}

int Sprite::collide_box(Sprite sprite){
	int collided=0;
	if( ((locX>sprite.getLocX() && locX<sprite.getLocX()+sprite.getWidth()) || (sprite.getLocX()>locX && sprite.getLocX()<locX+width)) &&
		((locY>sprite.getLocY() && locY<sprite.getLocY()+sprite.getHeight()) || (sprite.getLocY()>locY && sprite.getLocY()<locY+height)) )
		collided=collide_pixelperfect(sprite);// then the two sprites have collided
	return collided;
}

int Sprite::collide_pixelperfect(Sprite sprite){
	int collided=0; //return value, set to 1 if a collision is detected
	int offsetX=sprite.getLocX()-locX; //the x distance between the two sprites
	int offsetY=sprite.getLocY()-locY; //the y distance between the two sprites
	int mask_color=makecol(255,0,255); //the mask color, purple

	BITMAP *thisSprite=create_bitmap(width,height); //create a bitmap to hold this sprite
	rectfill(thisSprite,0,0,width,height,mask_color); //fill it with the mask color
	draw(thisSprite,0, 0); //draw the sprite to the bitmap at the 0,0
	BITMAP *otherSprite=create_bitmap(sprite.getWidth(),sprite.getHeight()); //create a btimap to hold the other sprite
	rectfill(otherSprite,0,0,sprite.getWidth(),sprite.getHeight(),mask_color); //fill it with mask color
	sprite.draw(otherSprite, offsetX, offsetY); //draw the other sprite at the offsets

	int a=0,b=0; //a and b hold the colors of each bitmap at the same location
	for(int w=0;w<width;w++) //loop through all pixels of the two bitmap
		for(int h=0;h<height;h++){
			a=getpixel(thisSprite,w,h); //get the colors for the points
			b=getpixel(otherSprite,w,h);
			if(a!=mask_color && b!=mask_color) //check the colors against the mask color, if they are both colored it means they are touching and have collided
				collided++;
		}
	destroy_bitmap(thisSprite);
	destroy_bitmap(otherSprite);
	return collided; //return collided
}

int Sprite::isAlive() { return alive; }
void Sprite::setAlive(int a) { alive=a; }

int Sprite::getLocX() { return locX; }
int Sprite::getLocY() { return locY; }

void Sprite::setLoc(int x, int y){ locX=x; locY=y; }

void Sprite::setFacing(int f){ facing=f; }
int Sprite::getFacing(){ return facing; }
void Sprite::setState(int s) { state=s; }
int Sprite::getState(){ return state; }
void Sprite::setLife(int l) { life=l; }
int Sprite::getLife(){ return life; }

void Sprite::draw_on_map(BITMAP *bmp, int offsetX, int offsetY){
	masked_blit(spritesheet, bmp, 
		column*width, row*height,
		locX-offsetX,locY-offsetY,width,height);
}
int Sprite::collide_with_map(){
	BITMAP *sprite=getSpriteFrame();
	BLKSTR *blockdata;
	int mask_color=makecol(255,0,255);
	int flag=0;
	for(int w=0;w<width;w++)
		for(int h=0;h<height;h++)
			//if(getpixel(sprite,w,h)!=mask_color)
			{
				blockdata=MapGetBlock((locX+w)/mapblockwidth,(locY+h)/mapblockheight);
				if(flag==0)flag=blockdata->tl;
				if(flag==0)flag=blockdata->tr;
				if(flag==0)flag=blockdata->bl;
				if(flag==0)flag=blockdata->br;
			}
	destroy_bitmap(sprite);
	return flag;
}

int Sprite::collide_with_map_peixelperfect(){
	BITMAP *sprite=getSpriteFrame();
	BLKSTR *blockdata;
	int mask_color=makecol(255,0,255);
	int flag=0;
	for(int w=0;w<width;w++)
		for(int h=0;h<height;h++)
			if(getpixel(sprite,w,h)!=mask_color)
			{
				blockdata=MapGetBlock((locX+w)/mapblockwidth,(locY+h)/mapblockheight);
				if(flag==0)flag=blockdata->tl;
				if(flag==0)flag=blockdata->tr;
				if(flag==0)flag=blockdata->bl;
				if(flag==0)flag=blockdata->br;
			}
	destroy_bitmap(sprite);
	return flag;
}