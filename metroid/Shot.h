#pragma once
#include "sprite.h"
class Shot : public Sprite
{
private:
	int hit_time;
public:
	Shot(void);
	~Shot(void);
};

