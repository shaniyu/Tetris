#include "Shape.h"

//This constructor creates a new shape 
Shape::Shape() :Block(SHAPE_SIZE, '#')
{
	//choosing a random shape for a new size 4 shape
	shape = (Shape::shapes)(rand() % 5);

	vector <Point> temp(SHAPE_SIZE);

	//for each shape we initialize the point locations from which the shape falls 
	switch (shape)
	{
	case LINE:
		
		temp[0].setPoint(9, 3);
		temp[1].setPoint(10, 3);
		temp[2].setPoint(11, 3);
		temp[3].setPoint(12, 3);
		break;

	case SQUARE:

		temp[0].setPoint(9, 3);
		temp[1].setPoint(10, 3);
		temp[2].setPoint(9, 4);
		temp[3].setPoint(10, 4);

		break;

	case PLUS:

		temp[0].setPoint(9, 3);
		temp[1].setPoint(9, 4);
		temp[2].setPoint(8, 4);
		temp[3].setPoint(10, 4);

		break;

	case L_SHAPE:

		temp[0].setPoint(9, 3);
		temp[1].setPoint(9, 4);
		temp[2].setPoint(10, 4);
		temp[3].setPoint(11, 4);

		break;

	case Z_SHAPE:

		temp[0].setPoint(9, 3);
		temp[1].setPoint(8, 3);
		temp[2].setPoint(9, 4);
		temp[3].setPoint(10, 4);

		break;

	default:
		break;
	}

	setBody(temp);

	drawBlock();

}


vector <Point> Shape::rotate()
{
	vector <Point> currBody = getBlockBody();
	vector <Point> temp(SHAPE_SIZE);
	int rotateRight[2][2] = { { 0, -1 },{ 1, 0 } }; //the matrix which represents a right rotation of pi/2
	Point rotationBasePoint; //the brick on which the rotation will be based.
	Point rotateChange; //the difference between the current point we want to rotate and the rotation base point
	int rotatedX, rotatedY;
	bool shouldRotate = true;


	//since we always shift the body cells (0-3) from right to left/ up to down, then the rotate point should change according to the rotate position of the block, because we want to rotate to be clockwise. 
	//if the line is in the first 2 positions, then the rotate point will be body[0]
	if (shape == Shape::shapes::SQUARE)
	{
		return(currBody);
	}
	if (pos == Shape::rotate_position::INITIAL || pos == Shape::rotate_position::ROTATE1)
	{
		rotationBasePoint = currBody[0];
		temp[0] = currBody[0];
		for (int i = 1; i < DEFAULT_BLOCK_SIZE && shouldRotate; i++)
		{
			rotateChange.setPoint((currBody[i].getX()) - (rotationBasePoint.getX()), (currBody[i].getY()) - (rotationBasePoint.getY()));

			rotatedX = rotateRight[0][0] * rotateChange.getX() + rotateRight[0][1] * rotateChange.getY();

			rotatedY = rotateRight[1][0] * rotateChange.getX() + rotateRight[1][1] * rotateChange.getY();

			rotateChange.setPoint(rotatedX, rotatedY);

			temp[i].setPoint(rotateChange.getX() + rotationBasePoint.getX(), rotateChange.getY() + rotationBasePoint.getY());
		}

		//shifting body cells
		if (pos == Shape::rotate_position::ROTATE1)
		{
			vector <Point> blockToCopy(SHAPE_SIZE);

			for (int i = 0; i < SHAPE_SIZE; i++)
			{
				blockToCopy[i] = temp[SHAPE_SIZE - i - 1];
			}
			for (int i = 0; i < SHAPE_SIZE; i++)
			{
				temp[i] = blockToCopy[i];
			}
			//delete[]blockToCopy;
		}

	}
	//if the line is in the last 2 positions, then the rotate point will be body[3]
	else
	{
		rotationBasePoint = currBody[3];
		temp[3] = currBody[3];
		for (int i = 0; i < DEFAULT_BLOCK_SIZE - 1 && shouldRotate; i++)
		{

			rotateChange.setPoint((currBody[i].getX()) - (rotationBasePoint.getX()), (currBody[i].getY()) - (rotationBasePoint.getY()));

			rotatedX = rotateRight[0][0] * rotateChange.getX() + rotateRight[0][1] * rotateChange.getY();

			rotatedY = rotateRight[1][0] * rotateChange.getX() + rotateRight[1][1] * rotateChange.getY();

			rotateChange.setPoint(rotatedX, rotatedY);

			temp[i].setPoint(rotateChange.getX() + rotationBasePoint.getX(), rotateChange.getY() + rotationBasePoint.getY());

		}

		//shifting body cells
		if (pos == Shape::rotate_position::ROTATE3)
		{
			vector <Point> blockToCopy(SHAPE_SIZE);

			for (int i = 0; i < SHAPE_SIZE; i++)
			{
				blockToCopy[i] = temp[SHAPE_SIZE - i - 1];
			}
			for (int i = 0; i < SHAPE_SIZE; i++)
			{
				temp[i] = blockToCopy[i];
			}

			//delete[]blockToCopy;
		}
	}

	return(temp);
}

void Shape::setPos(const int pos)
{
	this->pos = (Shape::rotate_position)pos;
}

const int Shape::getPos()
{
	return(pos);
}

void Shape::move(Point::direction dir, char(&bucket)[10][15], bool &shouldBlockExplode)
{
	vector <Point> temp(SHAPE_SIZE);
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

		for (int i = 0; i < SHAPE_SIZE; i++)
		{
			temp[i] = currBody[i];
		}
		for (int i = 0; i < SHAPE_SIZE; i++)
		{
			temp[i].move(dir);
		}

		//if the temp block movement was valid then we update the original block
		if (isBlockMovementValid(temp, bucket, shouldBlockExplode))
		{
			for (int i = 0; i < SHAPE_SIZE; i++)
			{
				currBody[i].draw(' ');
			}
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
	shouldBlockExplode = false;

}


string Shape::getBlockType()
{
	return("Shape");
}