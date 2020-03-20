#pragma warning(disable:4996)
#ifndef __POINT_H
#define __POINT_H

#include <iostream>	
#include <conio.h>
#include "Gotoxy.h"
#include <vector>
#include <typeinfo>


using namespace std;

const int DEFAULT_BLOCK_SIZE = 4;
const int SPECIAL_BLOCK_SIZE = 1;
const int MOVE_LEFT = -1;
const int MOVE_RIGHT = 1;
const int NO_MOVE = 0;
const int MOVE_DOWN = 1;

class Point
{
private:
	int x = 9, y = 3; //default setting for point- the origin
	int dirX = 0, dirY = 1; //the block will "fall" from the top so we want it to 

public:
	enum direction { KEY_UP = 119, KEY_DOWN = 120, KEY_LEFT = 97, KEY_RIGHT = 100 };

	void setPoint(int x = 9, int y = 3);
	void draw(char c = '#');
	void move(direction dir);
	int getX() const;
	int getY() const;
	Point getPoint() const;
};

#endif //__POINT_H