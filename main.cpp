// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "RectangularMaze.h"
#include "CircularMaze.h"
#include "DepthFirst.h"

int main()
{
	// RectangularMaze maze;
	CircularMaze maze;

	DepthFirst df(&maze);

	df.generate();

	maze.save_maze("test");
}
