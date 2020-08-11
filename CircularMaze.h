#pragma once


#include <string>
#include <cmath>
#include <tuple>
#include <iostream>

#include "Maze.h"

constexpr double PI = 3.1415926535897932384626433832795;

static std::tuple<double, double> polarToCartesian(size_t r, size_t n)
{
	size_t segments = 1u << r;

	double x = cos(2.0 * n * PI / segments);
	double y = sin(2.0 * n * PI / segments);

	return { r * x, r * y };
}


struct CircularMaze : public Maze
{
	CircularMaze(size_t radius = 8) : radius(radius)
	{
		// we have 1 cell for radius 1
		// 1 + 2 = 3 cells for radius 2
		// 1 + 2 + 4 = 7 cells for radius 3
		cells.reserve((2u << radius) - 1u);

		// add cells
		for (size_t r = 0; r < radius; ++r)
		{
			// for r:
			// 0: 1
			// 1: 2
			// 2: 4
			// ...
			// 4: 16
			for (size_t n = 0; n < (1u << r); ++n)
			{
				cells.push_back(Cell(r, n));
			}
		}

		// add neighbors
		for (auto& cell : cells) {
			size_t r = std::get<0>(cell.position);
			size_t n = std::get<1>(cell.position);

			size_t max_n = 1u << r;

			if (r > 0) {
				// r = 2 and n = 0: prev_n = 0
				// r = 2 and n = 1: prev_n = 0
				// r = 2 and n = 2: prev_n = 1
				// ...

				cell.neighbors.insert(&cells[get_index(r - 1u, n >> 1u)]);

				// we also have a left neighbor and a right neighbor if we're not in the center
				cell.neighbors.insert(&cells[get_index(r, (n + max_n - 1u) & (max_n - 1u))]);
				cell.neighbors.insert(&cells[get_index(r, (n + max_n + 1u) & (max_n - 1u))]);
			}

			if (r < radius - 1) {
				// we have to insert two:
				// n*2 and n*2+1
				cell.neighbors.insert(&cells[get_index(r + 1u, n << 1u)]);
				cell.neighbors.insert(&cells[get_index(r + 1u, (n << 1u) + 1u)]);
			}
			cell.initialize_neighbors();
		}
	}

	size_t get_index(size_t r, size_t n)
	{
		return (1u << r) - 1u + n;
	}

	virtual std::tuple<int, int, int, int> get_bounds()
	{
		return { 0, 0, radius << 1u, radius << 1u };
	}

	virtual std::string get_walls(const Cell& cell)
	{
		size_t r = std::get<0>(cell.position);
		size_t n = std::get<1>(cell.position);

		size_t max_n = 1u << r;

		std::string borders;

		if (r > 0) {
			if (cell.walls.find(&Cell(r - 1, n >> 1u)) != cell.walls.end()) {
				borders += draw_circular_wall(r, n, n + 1);
			}

			if (cell.walls.find(&Cell(r, (n + max_n - 1) & (max_n - 1))) != cell.walls.end()) {
				borders += draw_linear_wall(r, r + 1, n, n << 1);
			}
		}

		if (r == radius - 1) {	// outside wall
			borders += draw_circular_wall(r + 1, (n << 1), (n + 1) << 1);
		}

		// borders += draw_circular_wall(r, n, n + 1);

		return borders;
	}

	std::string draw_linear_wall(size_t start_r, size_t end_r, size_t start_n, size_t end_n)
	{
		double center_x = radius * 30.0;
		double center_y = radius * 30.0;

		double start_x, start_y;
		double end_x, end_y;

		std::tie(start_x, start_y) = polarToCartesian(start_r, start_n);
		std::tie(end_x, end_y) = polarToCartesian(end_r, end_n);

		size_t sx = static_cast<size_t>(std::round(start_x * 30.0 + center_x));
		size_t sy = static_cast<size_t>(std::round(start_y * 30.0 + center_y));
		size_t ex = static_cast<size_t>(std::round(end_x * 30.0 + center_x));
		size_t ey = static_cast<size_t>(std::round(end_y * 30.0 + center_y));

		return "<line x1=\"" + std::to_string(sx) +
			"\" x2=\"" + std::to_string(ex) +
			"\" y1=\"" + std::to_string(sy) +
			"\" y2=\"" + std::to_string(ey) +
			"\" stroke=\"black\" stroke-linecap=\"round\" stroke-width=\"3\"/>\n";
	}

	std::string draw_circular_wall(size_t r, size_t start_n, size_t end_n)
	{
		double center_x = radius * 30.0;
		double center_y = radius * 30.0;

		double start_x, start_y;
		double end_x, end_y;
		std::tie(start_x, start_y) = polarToCartesian(r, start_n);
		std::tie(end_x, end_y) = polarToCartesian(r, end_n);

		size_t sx = static_cast<size_t>(std::round(start_x * 30.0 + center_x));
		size_t sy = static_cast<size_t>(std::round(start_y * 30.0 + center_y));
		size_t ex = static_cast<size_t>(std::round(end_x * 30.0 + center_x));
		size_t ey = static_cast<size_t>(std::round(end_y * 30.0 + center_y));

		return "<path d=\"M " +
			std::to_string(sx) +					// start x
			" " +
			std::to_string(sy) +					// start y
			" A " +
			std::to_string(r * 30) +					// radius x
			" " +
			std::to_string(r * 30) +					// radius y
			", 0" +										// rotation x
			", 0" +										// short (0) or long (1) path
			", 1, " +									// cw or ccw
			std::to_string(ex) +					// end x
			" " +
			std::to_string(ey) +					// end y
			"\" stroke=\"black\" stroke-linecap=\"round\" stroke-width=\"3\" fill=\"transparent\"/>\n";
	}

	size_t radius;
};
