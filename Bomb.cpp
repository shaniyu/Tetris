#include "Bomb.h"

Bomb::Bomb() :Block(BOMB_SIZE, '@')
{
	vector <Point> temp(BOMB_SIZE);
	temp[0].setPoint(9, 3);

	setBody(temp);

	drawBlock();
}

void Bomb::move(Point::direction dir, char(&bucket)[10][15], bool &shouldBlockExplode)
{
	vector <Point> temp(BOMB_SIZE);
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

		//if the temp block movement was valid then we update the original block
		if (isBlockMovementValid(temp, bucket, shouldBlockExplode))
		{
			currBody[0].draw(' ');
			setBody(temp);
			drawBlock();
		}

		//if the temp block movement isn't valid then we move the original block down (default option)
		else
		{
			continueDown();
		}

		break;

	default:
		continueDown();
		break;
	}
}

string Bomb::getBlockType()
{
	return("Bomb");
}
