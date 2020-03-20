#ifndef __TETRIS_GAME_H
#define __TETRIS_GAME_H
#include <iostream>
#include <Windows.h>
#include <conio.h>
using namespace std;

#include "Block.h"
#include "Shape.h"
#include "Joker.h"
#include "Bomb.h"

const int SLOWER = 100;
const int FASTER = -100;
const int ONE_DELETED_ROW = 100;
const int TWO_DELETED_ROWS = 300;
const int THREE_DELETED_ROWS = 500;
const int FOUR_DELETED_ROWS = 800;
const char EOD = '0';
const int NUM_OF_BLOCK_TYPES = 7;
const int NUM_OF_SHAPE_TYPES = 4;


class TetrisGame {

public:

	enum blockType { SHAPE, JOKER, BOMB };

private:

	Block *block;
	int score;
	int currentSpeed = 600; //milliseconds for sleep (200 = initial speed of blocks)
	char bucket[10][15];
	int numOfBlocks = 0;
	bool gameOver = false;
	//blockType type;

public:

	enum menu { START_GAME = 1, PAUSE_CONTINUE = 2, INCREASE_SPEED = 3, DECREASE_SPEED = 4, EXIT = 9, STOP_JOKER1 = 83, STOP_JOKER2 = 115 };
	

	char runGame();

	//This funtion displayes the menu, current score and current number of blocks that fell down.
	void displayData() const;

	//This funtion draws the game borders to the console
	void drawBoard() const;

	//This function pauses/continues the game.
	char pauseGame() const;

	//This function updates the current speed of blocks according to the keys that were pressed
	void updateSpeed(const int rate);

	//This funtion clears the keyboard buffer (after each new block is created)
	void clearKeyboardBuffer() const;

	~TetrisGame();

	//This function updates the current number of blocks that fell down
	void updateNumOfBlocks();

	//This function print the bucket matrix to the console (which is the game board)
	void printBucket() const;

	//This function updates the current score
	void updateScore(const int change);

	//This function goes to a specific cell in the bucket matrix and returns the character in it
	char getCInSpecificPoint(const Point &relevantPoint) const;

	//This function intializes all of the game's attributes
	void initializeGameData();

	//This is a empty consrtuctor
	TetrisGame();

	//This is a wrraper function that decides if to start a game or exit according to the user's input
	void Start();

	//This function gets an X value (which is adjusted to the bucket values) of a point (in the current block's body) and returns the minimal Y value in the bucket matrix
	//which is the Y location of the highest standing block that's alligned with the current block 
	int getMaxYforX(const int x) const;

	//This function gets a rotated block body by reference and checks if its location is valid (not crossing any borders or standing blocks)  
	bool isRotateValid(const vector <Point> &temp) const;

	//This function will drop a block from its current location straight down to lowest available location
	void hardDrop();

	//This function updates the current block's point locations in the bucket matrix (after the block stops or the bomb should explode) 
	void updateBucket();

	//This function checks whether the current block should stop
	bool shouldBlockStop();

	//This function checks whether the current block needs to stop according to its location and the other standing blocks.
	//if it needs to stop then the "createNewBlock" flag is updated to "true". 
	//also checks if need to delete full rows and updates the score accordingly
	void checkBucket(bool &createNewBlock);

	//This function checks the bucket matrix and return the number of full rows.
	int checkHowManyFullRowsInBucket();

	//This funtion checks if the game is over by checking if the first row of the bucket is full from where the blocks drop
	void isGameOver();
};
#endif // !__TETRIS_GAME_H