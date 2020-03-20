#ifndef __Joker_H
#define __Joker_H
#include "Block.h"

const int JOKER_SIZE = 1;

class Joker : public Block
{
private:

public:

	Joker();

	//Moving the Joker
	virtual void move(Point::direction dir, char(&bucket)[10][15], bool &shouldBlockExplode);

	//Checking if the Joker movement is valid
	virtual bool isBlockMovementValid(vector <Point> &temp, const char(&bucket)[10][15], bool &shouldBlockExplode) const;
	virtual string getBlockType();
};

#endif // !__Joker_H


