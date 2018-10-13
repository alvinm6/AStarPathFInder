/******************************************************************************
Title:			Vector3.h
Author:			Alvin Moonsamy
Date Written:	24/04/2014

Description:	- 3-Dimensional Vector Class
				- Vector object with x, y and z co-ordinates
				- Used by the path finding algorithm
===============================================================================*/

#pragma once

class Vector3
{
public:
	int x;
	int y;
	int z;
 
	Vector3();
	Vector3(int a, int b, int c);
};