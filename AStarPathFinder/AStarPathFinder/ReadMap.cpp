#include "ReadMap.h"

ReadMap::ReadMap(void) {
	width = 0;
	height = 0;
}

ReadMap::~ReadMap(void) {
}

ReadMap::ReadMap(std::string fileName) : mapFileName(fileName), width(0), height(0) {
}

bool ReadMap::openMapFile(void) {
	mapFile.open(mapFileName.c_str());
	if(mapFile.is_open()) {
		std::cout<<"\nOpened "<<mapFileName<<std::endl<<std::endl;
		return true;
	} else {
		std::cout<<"\nError opening file "<<mapFileName<<std::endl<<std::endl;
		return false;
	}
}

void ReadMap::closeMapFile(void) {
	mapFile.close();
}

void ReadMap::populateMap(void) {
	
	char temp[10];
	std::string s;
	mapFile.getline(temp, 256, ',');
	width = atoi(temp);
	mapFile.getline(temp, 256);
	height = atoi(temp);

	while(!mapFile.eof()) {
		for(int i = 0 ; i<height; i++) {
			getline(mapFile,s);
			for(int j = 0; j<width; j++) {
				tMap[i*width+j] = s.c_str()[j];
			}
		}
	}
}

void ReadMap::printMap(void) {
	int i = 0;
	for(std::map<int,char>::iterator it = tMap.begin(); it != tMap.end(); it++) {
		std::cout<<it->second<<"\t";
		i++;
		if(i%width == 0)
			std::cout<<std::endl<<std::endl;
	}
}