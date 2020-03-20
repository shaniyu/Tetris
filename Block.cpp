#include "Block.h"

Block::Block(int blockSize, char ch) : blockSize(blockSize), c(ch)
{
	body.resize(blockSize);
	//body = new Point[blockSize];
}

void Block::drawBlock()
{
	for (int i = 0; i < blockSize; i++)
	{
		body[i].draw(c);
	}
}

void Block::setBody(vector <Point> temp)
{
	for (int i = 0; i < blockSize; i++)
	{
		body[i] = temp[i];
	}
}

Block::~Block()
{
	//delete[]body;
}

vector <Point> & Block::getBlockBody()
{
	return(body);
}

bool Block::isBlockMovementValid(vector <Point> &temp, const char(&bucket)[10][15], bool &shouldBlockExplode) const
{
	shouldBlockExplode = false;
	for (int i = 0; i < blockSize; i++)
	{
		// if one of the bricks of the rotated block lands on an existing block:
		if (bucket[temp[i].getY() - FIRST_ROW][temp[i].getX() - FIRST_COL] != ' ')
		{
			shouldBlockExplode = true;
			return(false);
		}

		// if one of the bricks of the rotated block overwrites the right border
		if (temp[i].getX() >= COLS + FIRST_COL)
		{
			shouldBlockExplode = true;
			return(false);
		}

		// if one of the bricks of the rotated block overwrites the left border
		if (temp[i].getX() < FIRST_COL)
		{
			shouldBlockExplode = true;
			return(false);
		}

		// if one of the bricks of the rotated block overwrites the floor border
		if (temp[i].getY() > FIRST_ROW + ROWS)
		{
			return(false);
		}

		// if one of the bricks of the rotated block overwrites the ceiling border
		if (temp[i].getY() < FIRST_ROW)
		{
			return(false);
		}
	}

	return(true);
}


void Block::continueDown()
{
	for (int i = 0; i < blockSize; i++)
	{
		body[i].draw(' ');
		body[i].move((Point::direction)0);
	}
	drawBlock();
}


char Block::getC() const
{
	return(c);
}

int Block::getBlockSize() const
{
	return(blockSize);
}

int Block::getMaxYForBlock()
{
	int max;
	max = body[0].getY();
	for (int i = 1; i < blockSize; i++)
	{
		if (max < body[i].getY())
		{
			max = body[i].getY();
		}
	}

	return(max);
}