#include "Point.h"

void Point::move(Point::direction dir)
{
	dirX = NO_MOVE;
	dirY = MOVE_DOWN;
	switch (dir)
	{
	case Point::direction::KEY_LEFT:

		dirX = MOVE_LEFT;
		dirY = NO_MOVE;

		break;

	case Point::direction::KEY_RIGHT:

		dirX = MOVE_RIGHT;
		dirY = NO_MOVE;

		break;

	default:
		dirX = NO_MOVE;
		dirY = MOVE_DOWN;
	}

	x += dirX;
	y += dirY;
}

void Point::draw(char c)
{
	gotoxy(x, y);
	cout << c << endl;
}

void Point::setPoint(int x, int y)
{
	this->x = x;
	this->y = y;
}

int Point::getX() const
{
	return(this->x);
}

int Point::getY() const
{
	return(this->y);
}

Point Point::getPoint() const
{
	Point res = *this;
	return res;
}