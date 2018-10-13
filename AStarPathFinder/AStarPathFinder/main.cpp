/******************************************************************************
Title:			main.cpp
Author:			Alvin Moonsamy
Date Written:	13/10/2018

Description:	- Performs A-Star path finding algorithm
				- Displays Tile Map files in '../Maps/' folder
				- Repeatedly ssks user for tile map file saved in '.txt' format
				  until user selects to exit application. This is to ease 
				  testing of the application with different maps without user
				  having to restart the application
				- When user enters the tile map file, The A-Star path finding 
				  algorithm is performed.
===============================================================================*/


#include "dirent.h"
#include "AStarPathFinder.h"

void read_directory(const std::string& name, std::vector<std::string>& v) {
	DIR* dirp = opendir(name.c_str());
	struct dirent* dp;
	while((dp = readdir(dirp)) != NULL) {
		v.push_back(dp->d_name);
	}
	closedir(dirp);
}

int main(int argc, char* argv[]) {

	AStarPathFinder m_pathFinder;
	std::string fileName = "";
	std::string mapFilesPath = "../Maps/";
	std::vector<std::string> allFileNames;
	unsigned char choice = '0';
	read_directory(mapFilesPath, allFileNames);

	do {
		std::cout<<"Map Files: "<<std::endl<<"-----------"<<std::endl;
		for(std::vector<std::string>::iterator it = allFileNames.begin(); it != allFileNames.end(); it++) {
			std::cout<<*it<<std::endl;
		}

		std::cout<<std::endl;

		std::cout<<std::endl<<"Select Option:"<<std::endl<<"--------------------"<<std::endl;
		std::cout<<"1 - Enter Map File"<<std::endl<<"Q - Exit"<<std::endl<<std::endl;
		std::cout<<"Enter Option: ";
		std::cin>>choice;
		std::cout<<std::endl;

		switch(choice) {
		case '1': std::cout<<"Enter map text file name: ";
			std::cin>>fileName;
			m_pathFinder.FindPath(mapFilesPath+fileName);
			break;
		case 'Q': exit(0);
			break;
		case 'q': exit(0);
			break;
		default:
			std::cout<<std::endl<<"Invalid Option!"<<std::endl<<std::endl;
			break;
		}

	} while (choice != 'Q' || choice != 'q');
	
	system("PAUSE");
	return 0;

} // main