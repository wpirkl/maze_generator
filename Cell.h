#pragma once

#include <set>
#include <tuple>
#include <ostream>

struct Cell
{
	Cell() {}
	Cell(size_t x, size_t y) : position({x, y}) {}

	void initialize_neighbors()
	{
		unvisitedNeighbors.clear();
		walls.clear();

		for (Cell* cell : neighbors) {
			walls.insert(cell);
			unvisitedNeighbors.insert(cell);
		}
	}

	void visit()
	{
		for (Cell * neighbor : neighbors)
		{
			neighbor->unvisitedNeighbors.erase(this);
		}
	}

	void remove_wall(Cell* cell)
	{
		walls.erase(cell);
		cell->walls.erase(this);
	}

	friend std::ostream& operator<<(std::ostream& os, const Cell& dt)
	{
		os << "Cell(" << std::get<0>(dt.position) << ", " << std::get<1>(dt.position) << ", [";

		size_t i = 0;
		for (auto it = dt.unvisitedNeighbors.begin(); it != dt.unvisitedNeighbors.end(); ++it)
		{
			os << "Cell(" << std::get<0>((*it)->position) << ", " << std::get<1>((*it)->position) << ")";
			if (++i < dt.unvisitedNeighbors.size())
			{
				os << ", ";
			}
		}
		os << "])";

		return os;
	}

	std::set<Cell*, bool (*) (Cell*, Cell*)> walls = std::set<Cell*, bool (*) (Cell*, Cell*)>([](Cell* a, Cell* b) -> bool { return a->position < b->position; });
	std::set<Cell*, bool (*) (Cell*, Cell*)> unvisitedNeighbors = std::set<Cell*, bool (*) (Cell*, Cell*)>([](Cell* a, Cell* b) -> bool { return a->position < b->position; });
	std::set<Cell*, bool (*) (Cell*, Cell*)> neighbors = std::set<Cell*, bool (*) (Cell*, Cell*)>([](Cell* a, Cell* b) -> bool { return a->position < b->position; });
	std::tuple<size_t, size_t> position;
};
