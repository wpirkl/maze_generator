#pragma once

#include <iostream>
#include <string>

#include "Maze.h"


struct RectangularMaze : public Maze
{
	RectangularMaze(size_t width = 15, size_t height = 15) : width(width), height(height)
	{
		cells.reserve(width * height);

		// add cells
		for (size_t y = 0; y < height; ++y)
		{
			for (size_t x = 0; x < width; ++x)
			{
				cells.push_back(Cell(x, y));
			}
		}

		// add walls
		for (auto& cell : cells) {
			size_t x = std::get<0>(cell.position);
			size_t y = std::get<1>(cell.position);

			if (x > 0) {	// left wall
				cell.neighbors.insert(&cells[get_index(x - 1, y)]);
			}

			if (x < width - 1) { // right wall
				cell.neighbors.insert(&cells[get_index(x + 1, y)]);
			}

			if (y > 0) { // up wall
				cell.neighbors.insert(&cells[get_index(x, y - 1)]);
			}

			if (y < height - 1) { // down wall
				cell.neighbors.insert(&cells[get_index(x, y + 1)]);
			}

			cell.initialize_neighbors();
		}
	}

	size_t get_index(size_t x, size_t y)
	{
		return x + y * width;
	}

	virtual std::tuple<int, int, int, int> get_bounds()
	{
		return { 0, 0, width, height };
	}

	virtual std::string get_walls(const Cell& cell)
	{
		std::string borders;
		size_t x = std::get<0>(cell.position);
		size_t y = std::get<1>(cell.position);

		if (y == 0 || (y > 0  && cell.walls.find(&Cell(x, y - 1)) != cell.walls.end())) {

			// top
			borders += "<line x1=\"" + std::to_string((x + 0) * 30) +
				"\" x2=\"" + std::to_string((x + 1) * 30) +
				"\" y1=\"" + std::to_string((y + 0) * 30) +
				"\" y2=\"" + std::to_string((y + 0) * 30) +
				"\" stroke=\"black\" stroke-linecap=\"round\" stroke-width=\"3\"/>\n";
		}

		if (y == height - 1 ) //|| (y < height - 1 && cell.walls.find(&Cell(x, y + 1)) != cell.walls.end()))
		{
			// bottom
			borders += "<line x1=\"" + std::to_string((x + 0) * 30) +
				"\" x2=\"" + std::to_string((x + 1) * 30) +
				"\" y1=\"" + std::to_string((y + 1) * 30) +
				"\" y2=\"" + std::to_string((y + 1) * 30) +
				"\" stroke=\"black\" stroke-linecap=\"round\" stroke-width=\"3\"/>\n";
		}

		if (x == 0 || (x > 0 && cell.walls.find(&Cell(x - 1, y)) != cell.walls.end()))
		{
			// left
			borders += "<line x1=\"" + std::to_string((x + 0) * 30) +
				"\" x2=\"" + std::to_string((x + 0) * 30) +
				"\" y1=\"" + std::to_string((y + 0) * 30) +
				"\" y2=\"" + std::to_string((y + 1) * 30) +
				"\" stroke=\"black\" stroke-linecap=\"round\" stroke-width=\"3\"/>\n";
		}

		if (x == width - 1 )//|| (x < width - 1 && cell.walls.find(&Cell(x + 1, y)) != cell.walls.end()))
		{
			// right
			borders += "<line x1=\"" + std::to_string((x + 1) * 30) +
				"\" x2=\"" + std::to_string((x + 1) * 30) +
				"\" y1=\"" + std::to_string((y + 0) * 30) +
				"\" y2=\"" + std::to_string((y + 1) * 30) +
				"\" stroke=\"black\" stroke-linecap=\"round\" stroke-width=\"3\"/>\n";
		}

		return borders;
	}

	size_t width;
	size_t height;
};
