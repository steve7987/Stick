#ifndef _HELPERS_H_
#define _HELPERS_H_


#include <iostream>
#include <fstream>
#include <sstream>

#include "classlist.h"

#define LOGFILE "log.txt"
#define PI 3.14159265
#define ROOT2 1.41421356237f
#define ROOT3 1.73205080757f

void textDump(std::string output);
float randb(float a, float b);

//returns true if two axis alligned bounding boxes collide, and false if they do not
bool aabbCollide(Vector pos1, Vector dim1, Vector pos2, Vector dim2);

//returns true if the sphere and box collide, position is corner of the box, dim is the lengths of the edges
bool sphereBoxCollide(Vector center, float radius, Vector position, Vector dimensions);

//returns a vector in {-1,0,1}^3 that represents which sides of the box the sphere collides with
//eg <0,1,-1> would mean collision with far y wall and near z wall (near meaning lower coord value NOT nearer to sphere)
//in the case where both near and fall walls collide, the wall closer to the sphere's center is returned
Vector sphereBoxSidesCollide(Vector center, float radius, Vector position, Vector dimensions);

bool sphereInsideBox(Vector center, float radius, Vector position, Vector dimensions);


#endif