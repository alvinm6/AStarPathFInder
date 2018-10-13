/*************************************************************************************
Title:			AStarPathFinder.h
Author:			Alvin Moonsamy
Date Written:	13/10/2018

Description:	- Performs A-Star path finding algorithm
				- Uses ReadMap tile map object to read tiles of user entered map
				- Algorithm:
					- Determine Start and Destination Positions in tile map
					- Set search direction depending of positional relation of 
					  start position to destination position
					- Start path search
					- Get next available tiles from current tile that are valid and 
					  calculate total cost (F) = Movement/Terrain cost (G) 
					  + Manhattan Distance Heuristic (H)
					  NB: Available tiles leads to possible open paths to destination
					- When next available tile ID equals destination tile ID then stop
					  searching
					- Display total cost of path, path co-ordinates and path on map
				- opened list = possible path tiles
				- visited list = tiles the algorithm checked will not be checked again
				- closed list = tiles in the path from start to destination
=======================================================================================*/

#pragma once

#include <vector>
#include "TileSearch.h"
#include "ReadMap.h"
#include "Vector3.h"

struct SEARCH_DIRECTION {
	short left_right;		//Left = -1 and Right = +1
	short CR_CCR;			//Counter-clockwise = -1and clockwise = +1
};

class AStarPathFinder {
public:

	AStarPathFinder(void);
	~AStarPathFinder(void);

	void FindPath(std::string fileName);
	Vector3 GetStartPos(std::map<int,char>* tMap);
	Vector3 GetDestPos(std::map<int,char>* tMap);
	void SetSearchDirection(Vector3* startPos, Vector3* destPos);
	void ClearOpenList() {
		m_openList.clear();
	}
	void ClearVisitedList() {
		m_visitedList.clear();
	}
	void ClearClosedList() {
		m_closedList.clear();
	}
	void InitializeMemberVariables(void);
	bool m_initializedStartDest;
	bool m_foundDest;
	bool m_startPosFound;
	bool m_destPosFound;

private:
	void SetStartAndDest(TileSearch start, TileSearch dest);
	void PathOpened(int x, int y, float newCost, TileSearch* parent);
	TileSearch* GetNextTile(); //Get next tile in list
	void ContinuePath();

	TileSearch* m_startTile;
	TileSearch* m_destTile;
	std::vector<TileSearch*> m_openList;
	std::vector<TileSearch*> m_visitedList;
	std::vector<Vector3*> m_closedList;

	ReadMap* tileMap;
	SEARCH_DIRECTION* srchDir;
	float totalPathCost;

};