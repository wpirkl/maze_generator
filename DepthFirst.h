#pragma once

#include <vector>
#include <random>
#include <map>
#include <algorithm>
#include <iostream>
#include <chrono>

#include "Maze.h"
#include "Cell.h"


struct DepthFirst
{
	DepthFirst(Maze * maze) : maze(maze), gen(std::chrono::system_clock::now().time_since_epoch().count())
	{
		firstCell = &*(maze->cells.begin());
	}

	void generate()
	{
		Cell* cell = firstCell;

		// visit first cell
		cell->visit();

		// push cell to stack
		stack.push_back(cell);

		while (!stack.empty())
		{
			// select a random unvisited neighbor
			Cell* random_unvisited_neighbor = select_random(cell);

			// if there is one
			if (random_unvisited_neighbor) {

				// remove the wall between us and the neighbor
				cell->remove_wall(random_unvisited_neighbor);

				// visit the neighbor
				random_unvisited_neighbor->visit();

				// remove the neighbor from my unvisited list
				cell->unvisitedNeighbors.erase(random_unvisited_neighbor);

				// push it to the stack
				cell = random_unvisited_neighbor;
				stack.push_back(cell);
			}
			else
			{
				// get the last one
				cell = stack.back();
				stack.pop_back();
			}
		}
	}

	// get a random unvisited neighboring cell
	Cell* select_random(Cell* cell)
	{
		size_t num_neighbors = cell->unvisitedNeighbors.size();
		if (num_neighbors== 0) {
			return nullptr;
		}

		if (num_neighbors == 1) {
			return *cell->unvisitedNeighbors.begin();
		}

		std::uniform_int_distribution<> distrib(0, cell->unvisitedNeighbors.size() - 1);

		auto it = cell->unvisitedNeighbors.begin();

		std::advance(it, distrib(gen));

		return *it;
	}

	std::mt19937 gen;

	const Maze* maze;
	Cell* firstCell;
	std::vector<Cell*> stack;
};
