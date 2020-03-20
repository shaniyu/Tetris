#ifndef __BLOCK_H
#define __BLOCK_H

#include "Point.h"


const int ROWS = 10;
const int COLS = 15;
const int FIRST_ROW = 3;
const int FIRST_COL = 1;

class Block
{
private:
	vector <Point> body;
	int blockSize;
	char c = '#';
	

public:
	//This constructor creates a new block 
	Block(int blockSize, char ch);

	//will free memory of block (body)
	~Block();

	void setBody(vector <Point> temp);

	//Drawing the whole block
	void drawBlock();

	vector <Point> & getBlockBody();

	virtual void setPos(const int pos){	}
	virtual const int getPos()
	{
		return(0);
	}

	//if the function wasn't called by Shape then no rotate should be done
	virtual vector <Point> rotate()
	{
		return(body);
	}

	//moving the block
	virtual void move(Point::direction dir, char(&bucket)[10][15], bool &shouldBlockExplode) = 0;

	//checking if the movement is valid
	virtual bool isBlockMovementValid(vector <Point> &temp, const char(&bucket)[10][15], bool &shouldBlockExplode) const;

	//Moving the block one step down
	void continueDown();
	char getC() const;
	int getBlockSize() const;
	int getMaxYForBlock();
	virtual string getBlockType() = 0;

};
#endif // __BLOCK_H