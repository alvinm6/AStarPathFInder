/******************************************************************************
Title:			TileSearch.h
Author:			Alvin Moonsamy
Date Written:	13/10/2018

Description:	- Structure for attributes of tiles in the map
				- Used in path finding algorithm
				- Attributes:
				  - X co-ordinate
				  - Y co-ordinate
				  - 1-Dimensional C++ map container of the map tiles
				  - G = Movement/terrain cost of the tile
				  - H = Manhattan distance of tile to destination
				  - F = total cost = G + H
				- Parent-Child relation that is current tile will be parent of 
				  next available tiles in possible paths
===============================================================================*/

#pragma once

#include <math.h>

struct TileSearch {
public:
	TileSearch* parent;
	int m_x_coord, m_y_coord, m_id;
	float G, H;

	TileSearch() : parent(0) {
	}

	TileSearch(int x, int y, int width, TileSearch* _parent = 0) : m_x_coord(x), m_y_coord(y), 
		parent(_parent), m_id(y * width + x), G(0), H(0) {
	}

	float GetF() {

		return G + H;
	}

	float ManhattanDistance(TileSearch* tileEnd) {
		float x = (float)(fabs((float)this->m_x_coord - tileEnd->m_x_coord));
		float y = (float)(fabs((float)this->m_y_coord - tileEnd->m_y_coord));

		return x + y;
	}

};