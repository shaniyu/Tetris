#include "TetrisGame.h"
#include <typeinfo>

char TetrisGame::runGame()
{
	int randRes;
	bool createNewBlock = true;
	bool isFirstMovement;
	Point overwrittenByJoker;//saving the point overwritten by joker in order to print it again after joker continues
	char overwrittenByJokerC = ' ';//saving the char overwritten by joker in order to print it again after joker continues

	drawBoard();

	while (true)
	{
		displayData();

		if (createNewBlock == true)
		{
			updateNumOfBlocks();
			clearKeyboardBuffer();//cleaning the buffer from previous keys that were pressed
			randRes = (rand() % NUM_OF_BLOCK_TYPES);

			if (randRes == 1)
			{
				//type = blockType::BOMB;
				block = new Bomb;
			}
			else if (randRes == 2)
			{
				//type = blockType::JOKER;
				block = new Joker;
			}
			else
			{
				//type = blockType::SHAPE;
				block = new Shape;
			}

			isFirstMovement = true;
			isGameOver(); //checks if there isn't any room for the new block (=game over)
			if (gameOver == true)
			{
				clrscr();
				cout << "GAME OVER" << endl; //printGameOver
				Sleep(4000);
				return(EOD);
			}

			//initializing the point (x and y values) and character of the overwrittenByJoker point
			//which will help us redraw the blocks that were overwritten by the joker 
			overwrittenByJoker.setPoint();
			overwrittenByJokerC = ' ';
		}

		char keyPressed = 0;

		Sleep(currentSpeed);

		if (_kbhit()) // checks if there is anything in the buffer
		{
			keyPressed = _getch(); // take the head of the buffer
			if (keyPressed == '1')
			{
				delete block;
				return(keyPressed);
			}

			else if (keyPressed - '0' == TetrisGame::menu::PAUSE_CONTINUE)
			{
				keyPressed = this->pauseGame();
			}
			else if (keyPressed - '0' == TetrisGame::menu::DECREASE_SPEED)
			{
				this->updateSpeed(SLOWER);
			}
			else if (keyPressed - '0' == TetrisGame::menu::INCREASE_SPEED)
			{
				this->updateSpeed(FASTER);
			}
			else if (keyPressed == TetrisGame::menu::STOP_JOKER1 || keyPressed == TetrisGame::menu::STOP_JOKER2)
			{
				if (block->getBlockType() == "Joker")
				{
					//updating the bucket matrix with the current location of the joker (since we stop it with 's')
					updateBucket();
					//after joker stopped- we need to create a new block
					createNewBlock = true;
				}
			}

			//only Shape can rotate
			else if (keyPressed == Point::direction::KEY_UP && (block->getBlockType() != "Joker" && block->getBlockType() != "Bomb"))
			{
				//we rotate a temporary block to see if the rotate is valid (it doesn't cross the borders or current standing blocks)
				const vector <Point> temp = block->rotate();	

				//if the rotate is valid, then we can update the original block with the new point locations and redraw it
				if (isRotateValid(temp) == true)
				{
					for (int i = 0; i < SHAPE_SIZE; i++)
					{
						(block->getBlockBody())[i].draw(' ');
						(block->getBlockBody())[i] = temp[i];
					}
					block->drawBlock();
				}
			}


			else if (keyPressed == Point::direction::KEY_DOWN)
			{
				hardDrop();
			}
		}

		if (keyPressed - '0' == TetrisGame::menu::EXIT)
		{
			return(EOD);
		}

		

		if (block->getBlockType() != "Joker" || (block->getBlockType() == "Joker" && (keyPressed != TetrisGame::menu::STOP_JOKER1 && keyPressed != TetrisGame::menu::STOP_JOKER2)))
		{
			//checking if the block should stop before its first movement
			if (isFirstMovement)
			{
				checkBucket(createNewBlock);
				isFirstMovement = false;
			}
			//the block wasn't deleted because it has enough space in the row below
			if (createNewBlock == false)
			{
				bool shouldBlockExplode = false;
				
				block->move((Point::direction)keyPressed, bucket, shouldBlockExplode);

				if (shouldBlockExplode == true)
				{
					updateBucket();
					delete block;
					createNewBlock = true;
				}
				else
				{
					//redrawing the characters that were overwritten by joker
					if (block->getBlockType() == "Joker")
					{
						overwrittenByJoker.draw(overwrittenByJokerC);
						overwrittenByJoker = ((block->getBlockBody())[0]).getPoint();
						overwrittenByJokerC = getCInSpecificPoint(overwrittenByJoker);
					}

					//checking whether the current block needs to stop according to its location and the other standing blocks.
					//if it needs to stop then the "createNewBlock" flag is updated to "true". 
					//also checks if need to delete full rows and updates the score accordingly
					checkBucket(createNewBlock);
				}
			}
		}

		//if the current block shape is the joker and it stopped already (pressed 's') then free memory of current block (in order to create a new one again later)
		else
		{
			delete(block);
		}
	}
}

//This funtion draws the game borders to the console
void TetrisGame::drawBoard() const
{
	gotoxy(0, 3); //start the game board in the 4th row (after displaying data in the first 3 rows)

	for (int i = 0; i < ROWS; i++) //drawing the boarders of the game for only the first 9 rows 
								   //because in the last row (10th row) we want to draw the bottom boarder, we'll do it in a seperate for loop)
	{
		gotoxy(0, i + FIRST_ROW);
		cout << "|";
		gotoxy(COLS + FIRST_COL, i + FIRST_ROW); //displaying "|" on both sides of the board (distance of 15 columns between them in each row)
		cout << "|";
	}

	//drawing the bottom boarder 
	cout << endl;
	for (int i = 0; i < COLS + 2; i++)
	{
		cout << "^";
	}
}

//This funtion displayes the menu, current score and current number of blocks that fell down. 
void TetrisGame::displayData() const
{
	gotoxy(0, 0); //present the menu in the first line of the CLI (=command line interface)
	cout << "Please choose one of the following: "
		<< "(1) Start a game " << "(2) Pause/continue the Game " << "(3) Increase speed of blocks " << endl
		<< "(4) Decrease speed of blocks " << "(9) Exit " << endl;

	gotoxy(0, 2); //present the score in the second line of the CLI (=command line interface)
	cout << "\33[2KYour current score is: " << this->score << "	" << "Current number of blocks: " << this->numOfBlocks;
}

//This function updates the current speed of blocks according to the keys that were pressed
void TetrisGame::updateSpeed(const int rate)
{
	if (this->currentSpeed + rate >= 0)
	{
		this->currentSpeed += rate;
	}
}

//This function pauses/continues the game.
char TetrisGame::pauseGame() const
{
	char keyPressed = 0;

	while (keyPressed - '0' != TetrisGame::menu::PAUSE_CONTINUE && keyPressed - '0' != TetrisGame::menu::EXIT)
	{
		if (_kbhit()) // checks if there is anything in the buffer
		{
			keyPressed = _getch();
		}
	}
	return(keyPressed);
}

//This funtion clears the keyboard buffer (after each new block is created)
void TetrisGame::clearKeyboardBuffer() const
{
	while (_kbhit())
	{
		_getch();
	}
}

TetrisGame::~TetrisGame()
{
	delete block;
}

//This function updates the current number of blocks that fell down
void TetrisGame::updateNumOfBlocks()
{
	numOfBlocks++;
}

//This function print the bucket matrix to the console (which is the game board)
void TetrisGame::printBucket() const
{
	gotoxy(FIRST_COL, FIRST_ROW);
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			cout << bucket[i][j];
		}
		gotoxy(FIRST_COL, FIRST_ROW + i + 1);
	}
}

//This function updates the current score
void TetrisGame::updateScore(const int change)
{
	score += change;

	if (score < 0)
	{
		score = 0;
	}
}

//This function goes to a specific cell in the bucket matrix and returns the character in it
char TetrisGame::getCInSpecificPoint(const Point &relevantPoint) const
{
	return(bucket[relevantPoint.getY() - FIRST_ROW][relevantPoint.getX() - FIRST_COL]);
}

//This is a wrraper function that decides if to start a game or exit according to the user's input
void TetrisGame::Start()
{
	char keyPressed = 0;
	char ch = 0;
	initializeGameData();
	displayData();
	while (true)
	{
		//checking if a key was pressed
		if (_kbhit())
		{
			ch = _getch();
		}
		//exit the game if 9 was pressed
		if (ch == '9')
		{
			return;
		}
		//checking if key pressed was 1 during a game or before starting the first game
		if (keyPressed == '1' || ch == '1')
		{
			//initalizing the game data and diplaying the updated data
			initializeGameData();
			displayData();
			keyPressed = runGame();
			//checking if the game is over or 9 was pressed during the game
			if (gameOver == true || keyPressed == EOD)
			{
				return;
			}
		}
	}
}

//This function intializes all of the game's attributes
void TetrisGame::initializeGameData()
{
	clrscr();
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			bucket[i][j] = ' ';
		}
	}
	score = 0;
	currentSpeed = 600;
	numOfBlocks = 0;
	gameOver = false;
	printBucket();
}

//This is a empty consrtuctor
TetrisGame::TetrisGame() {}

//This function gets an X value (which is adjusted to the bucket values) of a point (in the current block's body) and returns the minimal Y value in the bucket matrix
//which is the Y location of the highest standing block that's alligned with the current block 
int TetrisGame::getMaxYforX(const int x) const
{
	int max = ROWS;

	for (int i = ROWS - 1; i >= 0; i--)
	{
		if (bucket[i][x] != ' ')
		{
			max = i;
		}

	}

	return(max);
}

//This function gets a rotated block body by reference and checks if its location is valid (not crossing any borders or standing blocks)  
bool TetrisGame::isRotateValid(const vector <Point> &temp) const
{
	for (int i = 0; i < SHAPE_SIZE; i++)
	{
		// if one of the bricks of the rotated block lands on an existing block:
		if (bucket[temp[i].getY() - FIRST_ROW][temp[i].getX() - FIRST_COL] != ' ')
		{
			return(false);
		}

		// if one of the bricks of the rotated block overwrites the right border
		if (temp[i].getX() >= COLS + FIRST_COL)
		{
			return(false);
		}

		// if one of the bricks of the rotated block overwrites the left border
		if (temp[i].getX() < FIRST_COL)
		{
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

	//in case the rotation was valid (didn't return any "false" until now) we update the rotation position of the original block.
	block->setPos((block->getPos() + 1) % NUM_OF_SHAPE_TYPES);

	return(true);
}


//This function will drop a block from its current location straight down to lowest available location
void TetrisGame::hardDrop()
{
	//if the current block is a joker, we need it to pass all standing blocks and go straight down to the bottom border.

	if (block->getBlockType() == "Joker")
	{
		(block->getBlockBody())[0].draw(' ');
		(block->getBlockBody())[0].setPoint((block->getBlockBody())[0].getX(), ROWS + FIRST_ROW - 1);
		(block->getBlockBody())[0].draw(block->getC());
	}

	//if the current block isn't a joker
	else
	{
		int maxYInBlock = block->getMaxYForBlock();
		int maxYInBoard = getMaxYforX((block->getBlockBody())[0].getX() - FIRST_COL);
		int heightDifference;

		for (int i = 1; i < block->getBlockSize(); i++)
		{
			//in the console and the bucket matrix, the minimal Y for a block is displayed as the highest Y in the console. 
			//so in order to find which Y to drop the block on- we need to find the minimal Y. 

			if (maxYInBoard > getMaxYforX((block->getBlockBody())[i].getX() - FIRST_COL))
			{
				maxYInBoard = getMaxYforX((block->getBlockBody())[i].getX() - FIRST_COL);
			}
		}

		heightDifference = maxYInBoard - maxYInBlock;

		for (int i = 0; i < block->getBlockSize(); i++)
		{
			(block->getBlockBody())[i].draw(' ');
			(block->getBlockBody())[i].setPoint((block->getBlockBody())[i].getX(), (block->getBlockBody())[i].getY() + heightDifference + FIRST_ROW-1);
		}

		block->drawBlock();
	}
}

//This function updates the current block's point locations in the bucket matrix (after the block stops or the bomb should explode) 
void TetrisGame::updateBucket()
{
	//if the block isn't a bomb then we just want to update the block's point locations in the bucket matrix
	if (block->getBlockType() != "Bomb")
	{
		for (int j = 0; j < block->getBlockSize(); j++)
		{
			bucket[(block->getBlockBody())[j].getY() - FIRST_ROW][(block->getBlockBody())[j].getX() - FIRST_COL] = block->getC();
		}
	}

	//if the current block is a bomb then we want to "erase" all bricks in the 3*3 range of the bomb, and erase the bomb as well.  
	else
	{
		//the score will be calculated by summing the ascii values of space and '#' (adding all the relevant cells in the 3*3 range of the bomb)
		//the ascii value of space is 32 and '#' is 35, there for we will substract the 32*(number of calculated cells) from the total sum
		//and the number that's left will be 3*(the number of cells that had '#'), because the difference between 32(space) and 35('#') is 3.
		//in the end we will devide the scoreToAdd by 3
		int scoreToAdd = 0;

		int bombX = (block->getBlockBody())[0].getX();
		int bombY = (block->getBlockBody())[0].getY();

		if (bombX == COLS)
		{
			//if the block is in the right bottom corner
			if (bombY == ROWS + FIRST_ROW - 1)
			{
				//updating the bucket with ' ' in the available 3*3 range of the bomb 

				scoreToAdd = bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL] +
					bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL - 1] +
					bucket[bombY - FIRST_ROW][bombX - FIRST_COL - 1] - (32 * 3);

				bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL] = ' ';
				bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL - 1] = ' ';
				bucket[bombY - FIRST_ROW][bombX - FIRST_COL - 1] = ' ';
			}

			//if the block is not in the right bottom corner but only next to the right border
			else
			{
				scoreToAdd = bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL] +
					bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL] +
					bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL - 1] +
					bucket[bombY - FIRST_ROW][bombX - FIRST_COL - 1] +
					bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL - 1] +
					bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL] - (32 * 6);

				bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL] = ' ';
				bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL] = ' ';
				bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL - 1] = ' ';
				bucket[bombY - FIRST_ROW][bombX - FIRST_COL - 1] = ' ';
				bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL - 1] = ' ';
				bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL] = ' ';
			}
		}
		else if (bombX == FIRST_COL)
		{
			//if the block is in the left bottom corner
			if (bombY == ROWS + FIRST_ROW - 1)
			{
				scoreToAdd = bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL] +
					bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL + 1] +
					bucket[bombY - FIRST_ROW][bombX - FIRST_COL + 1] - (32 * 3);

				//updating the bucket with ' ' in the available 3*3 range of the bomb 
				bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL] = ' ';
				bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL + 1] = ' ';
				bucket[bombY - FIRST_ROW][bombX - FIRST_COL + 1] = ' ';
			}

			//if the block is not in the left bottom corner but only next to the left border
			else
			{
				scoreToAdd = bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL] +
					bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL + 1] +
					bucket[bombY - FIRST_ROW][bombX - FIRST_COL + 1] +
					bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL + 1] +
					bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL] - (32 * 5);

				bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL] = ' ';
				bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL + 1] = ' ';
				bucket[bombY - FIRST_ROW][bombX - FIRST_COL + 1] = ' ';
				bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL + 1] = ' ';
				bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL] = ' ';
			}
		}

		//if the bomb is next to the bottom border but isn't next to anyone of the side borders
		else if (bombY == ROWS + FIRST_ROW - 1)
		{
			scoreToAdd = bucket[bombY - FIRST_ROW][bombX - FIRST_COL - 1] +
				bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL - 1] +
				bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL] +
				bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL + 1] +
				bucket[bombY - FIRST_ROW][bombX - FIRST_COL + 1] - (32 * 5);

			bucket[bombY - FIRST_ROW][bombX - FIRST_COL - 1] = ' ';
			bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL - 1] = ' ';
			bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL] = ' ';
			bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL + 1] = ' ';
			bucket[bombY - FIRST_ROW][bombX - FIRST_COL + 1] = ' ';
		}

		//The bomb hits a block right after it appears, near the ceiling
		else if (bombY == FIRST_ROW)
		{
			scoreToAdd = bucket[bombY - FIRST_ROW][bombX - FIRST_COL - 1] +
				bucket[bombY - FIRST_ROW][bombX - FIRST_COL + 1] +
				bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL - 1] +
				bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL] +
				bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL + 1] - (32 * 5);

			bucket[bombY - FIRST_ROW][bombX - FIRST_COL - 1] = ' ';
			bucket[bombY - FIRST_ROW][bombX - FIRST_COL + 1] = ' ';
			bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL - 1] = ' ';
			bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL] = ' ';
			bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL + 1] = ' ';
		}

		//if the bomb stopped somewhere in the middle of the board (not next to any of the borders)
		else
		{
			scoreToAdd = bucket[bombY - FIRST_ROW][bombX - FIRST_COL - 1] +
				bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL - 1] +
				bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL] +
				bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL + 1] +
				bucket[bombY - FIRST_ROW][bombX - FIRST_COL + 1] +
				bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL - 1] +
				bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL] +
				bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL + 1] - (32 * 8);

			bucket[bombY - FIRST_ROW][bombX - FIRST_COL - 1] = ' ';
			bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL - 1] = ' ';
			bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL] = ' ';
			bucket[bombY - FIRST_ROW - 1][bombX - FIRST_COL + 1] = ' ';
			bucket[bombY - FIRST_ROW][bombX - FIRST_COL + 1] = ' ';
			bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL - 1] = ' ';
			bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL] = ' ';
			bucket[bombY - FIRST_ROW + 1][bombX - FIRST_COL + 1] = ' ';
		}

		updateScore((-50)*scoreToAdd / 3);
		printBucket();
	}

}

//This function checks whether the current block should stop
bool TetrisGame::shouldBlockStop()
{
	int maxY = FIRST_ROW;
	int currentX;
	bool stopped = false;

	if (block->getBlockType() != "Joker")
	{
		for (int i = 0; i < block->getBlockSize() && stopped == false; i++)
		{
			currentX = (block->getBlockBody())[i].getX();

			//calculating the maximum Y value for the current X value of the point- body[i]
			maxY = getMaxYforX(currentX - FIRST_COL) + FIRST_ROW - 1;

			//if the current point's Y value alligns with the maxY value then we need to stop the block 
			if (maxY == (block->getBlockBody())[i].getY())// + 1)
			{
				stopped = true;
				//update the bucket with the stopped block's point locations
				updateBucket();
			}
		}
	}

	else //the joker will stop only if it got to the bottom border or when we click 's' (which we check in "runGame")
	{
		if ((block->getBlockBody())[0].getY() == FIRST_ROW + ROWS - 1)
		{
			stopped = true;
			updateBucket();
		}
	}
	return(stopped);
}

//This function checks whether the current block needs to stop according to its location and the other standing blocks.
//if it needs to stop then the "createNewBlock" flag is updated to "true". 
//also checks if need to delete full rows and updates the score accordingly
void TetrisGame::checkBucket(bool &createNewBlock)
{
	bool stopped;
	int numOfDeletedRows;
	stopped = shouldBlockStop();
	if (stopped == true)
	{
		createNewBlock = true;
		numOfDeletedRows = checkHowManyFullRowsInBucket();

		//after deleting any rows we need to print the bucket (=game board) to the console again.
		if (block->getBlockType() == "Joker" && numOfDeletedRows == 1)
		{
			updateScore(50);
			printBucket();
		}
		else if (numOfDeletedRows == 1)
		{
			updateScore(ONE_DELETED_ROW);
			printBucket();
		}
		else if (numOfDeletedRows == 2)
		{
			updateScore(TWO_DELETED_ROWS);
			printBucket();
		}
		else if (numOfDeletedRows == 3)
		{
			updateScore(THREE_DELETED_ROWS);
			printBucket();
		}
		else if (numOfDeletedRows == 4)
		{
			updateScore(FOUR_DELETED_ROWS);
			printBucket();
		}

		delete block;
	}
	else
	{
		createNewBlock = false;
	}
}

//This function checks the bucket matrix and return the number of full rows.
int TetrisGame::checkHowManyFullRowsInBucket()
{
	int write = ROWS - 1, read = ROWS - 1;
	int countFullRows = 0;
	bool isRowFull;
	for (int i = ROWS - 1; i >= 0; i--)
	{
		isRowFull = true;

		//if one of the cells in the current row is empty then the row isn't full
		for (int j = 0; j < COLS && isRowFull; j++)
		{
			if (bucket[i][j] == ' ')
			{
				isRowFull = false;
			}
		}

		if (isRowFull == true)
		{
			countFullRows++;

			//if we reached the top row of the bucket (which is the first row of the bucket) and it's full
			if (read == 0)
			{
				//there's no row above we can shift down to the first row so we initialize it with spaces
				for (int j = 0; j < COLS; j++)
				{
					bucket[read][j] = ' ';
				}
			}
		}

		//if the current row isn't full
		else
		{
			//and there's a row below the current row that's full
			if (read != write)
			{
				//then we shift down the current row to the full row below.
				for (int j = 0; j < COLS; j++)
				{
					bucket[write][j] = bucket[read][j];
				}
			}
			write--;
		}

		read--;
	}
	return(countFullRows);
}


//This funtion checks if the game is over by checking if the first row of the bucket is full from where the blocks drop
void TetrisGame::isGameOver()
{
	if (block->getBlockType() != "Joker")
	{
		for (int i = 0; i < block->getBlockSize() && !gameOver; i++)
		{
			if (bucket[(block->getBlockBody())[i].getY() - FIRST_ROW][(block->getBlockBody())[i].getX() - FIRST_COL] != ' ')
			{
				gameOver = true;
			}
		}
	}
}
