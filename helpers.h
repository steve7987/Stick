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
//returns true if the sphere and box collide, position is corner of the box, dim is the lengths of the edges
bool sphereBoxCollide(Vector center, float radius, Vector position, Vector dimensions);

#endif