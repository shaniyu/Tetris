#ifndef __SHAPE_H
#define __SHAPE_H

#include "Block.h"

const int SHAPE_SIZE = 4;

class Shape: public Block
{

public:
	enum shapes { LINE, SQUARE, PLUS, Z_SHAPE, L_SHAPE };
	enum rotate_position { INITIAL, ROTATE1, ROTATE2, ROTATE3 };

private:

	shapes shape;
	rotate_position pos = INITIAL;

public:

	Shape();

	//Rotating the Shape
	virtual vector <Point> rotate();
	virtual void setPos(const int pos);
	virtual const int getPos();

	//Moving the Sahpe
	virtual void move(Point::direction dir, char(&bucket)[10][15], bool &shouldBlockExplode);

	virtual string getBlockType();

};

#endif // !__SHAPE_H
