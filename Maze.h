#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <tuple>

#include "Cell.h"


struct Maze
{
	virtual std::tuple<int, int, int, int> get_bounds() = 0;

	virtual std::string get_walls(const Cell& cell) = 0;

	void save_maze(const std::string filename)
	{
		std::ofstream svgfile(filename + ".svg");
		if (!svgfile) {
			std::cerr << "Error opening " << filename << ".svg for writing.\n";
			std::cerr << "Terminating.";
			exit(1);
		}

		int xmin, ymin, xmax, ymax;
		std::tie(xmin, ymin, xmax, ymax) = get_bounds();

		int xresolution = (xmax - xmin + 2) * 30,
			yresolution = (ymax - ymin + 2) * 30;

		svgfile << "<svg width=\"" << xresolution << "\" height=\"" << yresolution
			<< "\" xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;

		svgfile << "<g transform=\"translate(" << (1 - xmin) * 30 << ","
			<< yresolution - (1 - ymin) * 30 << ") scale(1,-1)\">" << std::endl;

		svgfile << "<rect x=\"" << (xmin - 1) * 30 << "\" y=\"" << (ymin - 1) * 30
			<< "\" width=\"" << xresolution << "\" height=\"" << yresolution
			<< "\" fill=\"white\"/>" << std::endl;

		// draw cells here
		for (Cell & cell : cells)
		{
			svgfile << get_walls(cell);
		}

		svgfile << "</g>" << std::endl;
		svgfile << "</svg>" << std::endl;
	}

	std::vector<Cell> cells;
};
