#pragma warning(disable:4996)
#include <iostream>
#include <conio.h>
using namespace std;

#include "Block.h"
#include "Gotoxy.h"
#include "Point.h"
#include "TetrisGame.h"


void main()
{
	TetrisGame game;
	game.Start();
	clrscr();
	system("pause");
}
