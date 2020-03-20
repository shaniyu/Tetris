#ifndef __BOMB_H
#define __BOMB_H
#include "Block.h"

const int BOMB_SIZE = 1;

class Bomb : public Block
{
private:

public:
	Bomb();

	//Moving the Bomb
	virtual void move(Point::direction dir, char(&bucket)[10][15], bool &shouldBlockExplode);
	virtual string getBlockType();

};

#endif // !__BOMB_H

