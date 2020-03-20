#include "Joker.h"

Joker::Joker() :Block(JOKER_SIZE, '#')
{
	vector <Point> temp(JOKER_SIZE);
	temp[0].setPoint(9, 3);

	setBody(temp);

	drawBlock();
}

void Joker::move(Point::direction dir, char(&bucket)[10][15], bool &shouldBlockExplode)
{
	vector <Point> temp(JOKER_SIZE);
	vector <Point> currBody = getBlockBody();
	switch (dir)
	{
	case Point::direction::KEY_UP:
		//Rotation is handeled in the TetrisGame, but still when the "move" function is called- we don't want it to go to the default case which is "continue down"
		break;
	case Point::direction::KEY_DOWN:
		//hard drop is handeled in the TetrisGame, but still when the "move" function is called- we don't want it to go to the default case which is "continue down"
		break;
	case Point::direction::KEY_LEFT:
	case Point::direction::KEY_RIGHT:

		temp[0] = currBody[0];
		temp[0].move(dir);
		if (isBlockMovementValid(temp, bucket, shouldBlockExplode))
		{
			setBody(temp);
			drawBlock();
		}

		else
		{
			continueDown();
		}
		break;

	default:
		continueDown();
		break;
	}

	shouldBlockExplode = false;
}


bool Joker::isBlockMovementValid(vector <Point> &temp, const char(&bucket)[10][15], bool &shouldBlockExplode) const
{
	shouldBlockExplode = false;

	// if one of the bricks of the rotated block overwrites the right border
	if (temp[0].getX() >= COLS + FIRST_COL)
	{
		return(false);
	}

	// if one of the bricks of the rotated block overwrites the left border
	if (temp[0].getX() < FIRST_COL)
	{
		return(false);
	}

	// if one of the bricks of the rotated block overwrites the floor border
	if (temp[0].getY() > FIRST_ROW + ROWS)
	{
		return(false);
	}

	// if one of the bricks of the rotated block overwrites the ceiling border
	if (temp[0].getY() < FIRST_ROW)
	{
		return(false);
	}

	return(true);
}

string Joker::getBlockType()
{
	return("Joker");
}