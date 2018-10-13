/******************************************************************************
Title:			ReadMap.h
Author:			Alvin Moonsamy
Date Written:	13/10/2018

Description:	- Read Map class to read map tiles saved in '.txt' format
				- Used in path finding algorithm
				- Open map file that user selects
				- Poputlate a C++ map container with map tiles mapped to equivalent
				  1-dimensional position
				- Display tiles of the map
				- Close map file when path search is done
				- Enum MAP KEY:
					-> '@', '.', 'X' = Flat Land, G = 1
					-> '*' = Forest, G = 2
					-> '^' = Mountain, G = 3
					-> '~' = Water, G = N/A
===============================================================================*/

#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

typedef enum {
	START_TILE = '@',
	DEST_TILE = 'X',
	WATER_TILE = '~',
	FLAT_LAND_TILE = '.',
	FOREST_TILE = '*',
	MOUNTAIN_TILE = '^',
	PATH_TILE = '#'
} MAPKEY;

class ReadMap {
public:
	ReadMap(void);
	ReadMap(std::string fileName);
	~ReadMap(void);

	int width, height;
	std::map<int, char> tMap;

	bool openMapFile(void);
	void closeMapFile(void);
	void populateMap(void);
	void printMap(void);

private:
	std::ifstream mapFile;
	std::string mapFileName;
};