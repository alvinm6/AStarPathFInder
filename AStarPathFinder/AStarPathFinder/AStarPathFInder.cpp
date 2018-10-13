#include "AStarPathFinder.h"

AStarPathFinder::AStarPathFinder(void) {
	InitializeMemberVariables();
}

AStarPathFinder::~AStarPathFinder() {
}

void AStarPathFinder::InitializeMemberVariables(void){
	m_initializedStartDest = false;
	m_foundDest = false;
	m_startPosFound = false;
	m_destPosFound = false;
	totalPathCost = 0;
}

void AStarPathFinder::FindPath(std::string fileName) {
	tileMap = new ReadMap(fileName.c_str());
	srchDir = new SEARCH_DIRECTION;
	bool openedFile = tileMap->openMapFile();
	if (openedFile) {
		tileMap->populateMap();
		tileMap->printMap();
		std::cout<<std::endl;
		Vector3 currentPos = GetStartPos(&tileMap->tMap);
		Vector3 targetPos = GetDestPos(&tileMap->tMap);
		SetSearchDirection(&currentPos, &targetPos);

		if(!m_initializedStartDest && m_startPosFound && m_destPosFound) {
			for (unsigned int i = 0; i< m_openList.size(); i++) {
				delete m_openList[i];
			}
			m_openList.clear();

			for (unsigned int i = 0 ; i<m_visitedList.size(); i++) {
				delete m_visitedList[i];
			}
			m_visitedList.clear();

			for (unsigned int i = 0; i<m_closedList.size(); i++) {
				delete m_closedList[i];
			}
			m_closedList.clear();

			//Initialize Start Tile
			TileSearch start;
			start.m_x_coord = currentPos.x;
			start.m_y_coord = currentPos.y;

			//Initialize Destination Tile
			TileSearch dest;
			dest.m_x_coord = targetPos.x;
			dest.m_y_coord = targetPos.y;

			SetStartAndDest(start, dest);
			m_initializedStartDest = true;
		}

		if(m_initializedStartDest) {
			while(!m_foundDest) {
				ContinuePath();
			}		
		}
	}
	InitializeMemberVariables();
	tileMap->closeMapFile();
	delete tileMap;
	delete srchDir;

} //AStarPathFinder

void AStarPathFinder::SetSearchDirection(Vector3* startPos, Vector3* destPos) {
	
	//Clockwise start right
	if (startPos->x<destPos->x && startPos->y<destPos->y) {
		srchDir->CR_CCR = 1;
		srchDir->left_right = 1;
	}

	//Counter Clockwise start left
	else if (startPos->x<destPos->x && startPos->y>destPos->y) {
		srchDir->CR_CCR = -1;
		srchDir->left_right = 1;
	}

	//Clockwise start left
	else if (startPos->x>destPos->x && startPos->y>destPos->y) {
		srchDir->CR_CCR = 1;
		srchDir->left_right = -1;
	}

	//Counter Clockwise start right
	else {
		srchDir->CR_CCR = -1;
		srchDir->left_right = -1;
	}
}

void AStarPathFinder::SetStartAndDest(TileSearch start, TileSearch dest) {

	m_startTile = new TileSearch(start.m_x_coord, start.m_y_coord, tileMap->width, NULL);
	m_destTile = new TileSearch(dest.m_x_coord, dest.m_y_coord, tileMap->width, &dest);

	m_startTile->G = 0;
	m_startTile->H = m_startTile->ManhattanDistance(m_destTile);
	m_startTile->parent = 0;

	m_openList.push_back(m_startTile);

} // SetStartAndDest

TileSearch* AStarPathFinder::GetNextTile() {

	float bestF = 999999.0f;		//arbritary high value since first iteration
	int tileIndex = -1;
	TileSearch* nextTile = NULL;
	
	for(unsigned int i = 0; i<m_openList.size(); i++) {
		if (m_openList[i]->GetF() < bestF) {
			bestF = m_openList[i]->GetF();
			tileIndex = i;
		}
	}

	if (tileIndex >= 0) {
		nextTile = m_openList[tileIndex];
		m_visitedList.push_back(nextTile);
		m_openList.erase(m_openList.begin() + tileIndex);
	}

	return nextTile;

} // GetNextTile

//Explore possible paths from current position
void AStarPathFinder::PathOpened(int x, int y, float newCost, TileSearch* parent) {

	//Boundary Check
	if (x<0 || y<0 || x>=tileMap->width || y>=tileMap->height) {
		return;
	}

	int id = y*tileMap->width + x;

	//Visited tile Check
	for (unsigned int i =0 ; i<m_visitedList.size(); i++) {
		if (id == m_visitedList[i]->m_id) {
			return;
		}
	}

	//Terrain Check
	switch(tileMap->tMap[y*tileMap->width+x]) {
	case WATER_TILE: return;
	case FLAT_LAND_TILE: newCost++;
		break;
	case START_TILE: newCost++;
		break;
	case DEST_TILE: newCost++;
		break;
	case FOREST_TILE: newCost = newCost+2;
		break;
	case MOUNTAIN_TILE: newCost = newCost+3;
		break;
	default:
		std::cout<<"Unknown Terrain"<<std::endl;
		return;
	}
	
	TileSearch* newChild = new TileSearch(x, y,  tileMap->width, parent);
	newChild->G = newCost;
	newChild->H = parent->ManhattanDistance(m_destTile);

	for (unsigned int i = 0; i<m_openList.size(); i++) {
		if (id == m_openList[i]->m_id) {
			float newF = newChild->G  + newCost + m_openList[i]->H;
			if (m_openList[i]->GetF() > newF) {
				m_openList[i]->G = newChild->G  + newCost;
				m_openList[i]->parent = newChild;
			} else {
				delete newChild;
				return;
			}

		}
	}

	m_openList.push_back(newChild);

} // PathOpened

//Search through map, expand position and find target position. Calculate G and H values
void AStarPathFinder::ContinuePath() {

	if(m_openList.empty()) {
		return;
	}

	TileSearch* currentTile = GetNextTile();

	if (currentTile->m_id == m_destTile->m_id) {
		m_destTile->parent = currentTile->parent;

		TileSearch* getPath;
		float totalG = 0;

		//Order path in closed list obtain total path G and H

		for (getPath = m_destTile; getPath != NULL; getPath = getPath->parent) {
			m_closedList.push_back(new Vector3(getPath->m_x_coord, getPath->m_y_coord, 0));
			totalPathCost += getPath->ManhattanDistance(m_destTile);
			if(getPath->G > totalG)
				totalG = getPath->G;
			if(getPath->parent == NULL)
				totalPathCost -= getPath->ManhattanDistance(m_destTile);
		}

		//Total path cost F = G + H + 1. Add one for final move to destination
		totalPathCost += totalG + 1;

		m_foundDest = true;
		std::cout<<"\nPath:"<<std::endl;
		for (int i = m_closedList.size()-1; i>=0; i--) {
			std::cout<<"("<<m_closedList[i]->x<<","<<m_closedList[i]->y<<")"<<std::endl;
			tileMap->tMap[(m_closedList[i]->y*tileMap->width)+m_closedList[i]->x] = PATH_TILE;
		}
		
		std::cout<<std::endl<<"Total Path Cost = "<<totalPathCost<<std::endl<<std::endl;
		tileMap->printMap();
		return;

	} else {

		PathOpened(currentTile->m_x_coord + srchDir->left_right, currentTile->m_y_coord, currentTile->G, currentTile);
		
		PathOpened(currentTile->m_x_coord + srchDir->left_right, currentTile->m_y_coord + (srchDir->CR_CCR*srchDir->left_right), currentTile->G, currentTile);
		
		PathOpened(currentTile->m_x_coord, currentTile->m_y_coord + (srchDir->CR_CCR*srchDir->left_right), currentTile->G, currentTile);
		
		PathOpened(currentTile->m_x_coord - srchDir->left_right, currentTile->m_y_coord + (srchDir->CR_CCR*srchDir->left_right), currentTile->G, currentTile);
		
		PathOpened(currentTile->m_x_coord - srchDir->left_right, currentTile->m_y_coord, currentTile->G, currentTile);
		
		PathOpened(currentTile->m_x_coord - srchDir->left_right, currentTile->m_y_coord - (srchDir->CR_CCR*srchDir->left_right), currentTile->G, currentTile);
		
		PathOpened(currentTile->m_x_coord, currentTile->m_y_coord - (srchDir->CR_CCR*srchDir->left_right), currentTile->G, currentTile);
		
		PathOpened(currentTile->m_x_coord + srchDir->left_right, currentTile->m_y_coord - (srchDir->CR_CCR*srchDir->left_right), currentTile->G, currentTile);

		for (unsigned int i = 0; i<m_openList.size(); i++ ) {
			if (currentTile->m_id == m_openList[i]->m_id) {
				m_openList.erase(m_openList.begin() + i);
			}
		}
	}
} // ContinuePath

Vector3 AStarPathFinder::GetStartPos(std::map<int,char>* tMap) {

	Vector3 start(0,0,0);
	
	for (std::map<int,char>::iterator it = tMap->begin(); it != tMap->end(); it++) {
		if(it->second == START_TILE) {
			start.y = it->first/tileMap->width;
			start.x = it->first%tileMap->width;
				m_startPosFound = true;
				break;
		}
	}

	if (m_startPosFound)
		std::cout<<"Start Position: ("<<start.x<<","<<start.y<<")"<<std::endl;
	else
		std::cout<<"Start not found!"<<std::endl;

	return start;

} // GetStartPos

Vector3 AStarPathFinder::GetDestPos(std::map<int,char>* tMap) {

Vector3 dest(0,0,0);
	
	for (std::map<int,char>::iterator it = tMap->begin(); it != tMap->end(); it++) {
		if(it->second == DEST_TILE) {
			dest.y = it->first/tileMap->width;
			dest.x = it->first%tileMap->width;
				m_destPosFound = true;
				break;
		}
	}

	if (m_startPosFound)
		std::cout<<"Destination Position: ("<<dest.x<<","<<dest.y<<")"<<std::endl;
	else
		std::cout<<"Destinaiton not found!"<<std::endl;

	return dest;

} // GetDestPos