#include "helpers.h"
#include "vector.h"

void textDump(std::string output){
	std::ofstream myfile (LOGFILE, std::ios::out | std::ios::app); 
	if (myfile.is_open())
	{
		myfile << output;
		myfile << "\n";
		myfile.close();
	}
}

float randb(float a, float b){
	float ret = ((float) rand()) / (float) RAND_MAX;
	return a + ret*(b-a);
}

bool aabbCollide(Vector pos1, Vector dim1, Vector pos2, Vector dim2){
	//compute centers
	Vector c1 = pos1 + 0.5 * dim1;
	Vector c2 = pos2 + 0.5 * dim2;

	bool x = abs(c1.x - c2.x) < (dim1.x + dim2.x) / 2;
	bool y = abs(c1.y - c2.y) < (dim1.y + dim2.y) / 2;
	bool z = abs(c1.z - c2.z) < (dim1.z + dim2.z) / 2;

	return x && y && z;
}

bool sphereBoxCollide(Vector center, float radius, Vector position, Vector dimensions){
	float dmin = 0;
	Vector posMax = position + dimensions;
	
	if (center.x < position.x){
		dmin += (center.x - position.x) * (center.x - position.x);
	}
	else if (center.x > posMax.x){
		dmin += (center.x - posMax.x) * (center.x - posMax.x);
	}

	if (center.y < position.y){
		dmin += (center.y - position.y) * (center.y - position.y);
	}
	else if (center.y > posMax.y){
		dmin += (center.y - posMax.y) * (center.y - posMax.y);
	}

	if (center.z < position.z){
		dmin += (center.z - position.z) * (center.z - position.z);
	}
	else if (center.z > posMax.z){
		dmin += (center.z - posMax.z) * (center.z - posMax.z);
	}
	
	return dmin <= (radius * radius);
}

Vector sphereBoxSidesCollide(Vector center, float radius, Vector position, Vector dimensions){
	Vector pos, dim;
	Vector ret = Vector(0, 0, 0);

	//check near x wall
	pos = position;
	dim = dimensions;
	dim.x = 0;
	if (sphereBoxCollide(center, radius, pos, dim)){
		ret.x = -1;
	}
	//check near y wall
	pos = position;
	dim = dimensions;
	dim.y = 0;
	if (sphereBoxCollide(center, radius, pos, dim)){
		ret.y = -1;
	}
	//check near z wall
	pos = position;
	dim = dimensions;
	dim.z = 0;
	if (sphereBoxCollide(center, radius, pos, dim)){
		ret.z = -1;
	}
	//check far x wall
	pos = position;
	dim = dimensions;
	pos.x = pos.x + dim.x;
	dim.x = 0;
	if (sphereBoxCollide(center, radius, pos, dim)){
		if (ret.x == 0){
			ret.x = 1;
		}
		else {
			//compare what sphere center is closer to
			if (abs(center.x - position.x) > abs(center.x - position.x - dimensions.x)){
				ret.x = 1;
			}
		}
	}
	//check far y wall
	pos = position;
	dim = dimensions;
	pos.y = pos.y + dim.y;
	dim.y = 0;
	if (sphereBoxCollide(center, radius, pos, dim)){
		if (ret.y == 0){
			ret.y = 1;
		}
		else {
			//compare what sphere center is closer to
			if (abs(center.y - position.y) > abs(center.y - position.y - dimensions.y)){
				ret.y = 1;
			}
		}
	}
	//check far z wall
	pos = position;
	dim = dimensions;
	pos.z = pos.z + dim.z;
	dim.z = 0;
	if (sphereBoxCollide(center, radius, pos, dim)){
		if (ret.z == 0){
			ret.z = 1;
		}
		else {
			//compare what sphere center is closer to
			if (abs(center.z - position.z) > abs(center.z - position.z - dimensions.z)){
				ret.z = 1;
			}
		}
	}
	return ret;
}

bool sphereInsideBox(Vector center, float radius, Vector position, Vector dimensions){
	Vector sphereBot = center - Vector(radius, radius, radius);
	Vector sphereTop = center + Vector(radius, radius, radius);

	Vector boxTop = position + dimensions;

	return (sphereBot.x > position.x && sphereBot.y > position.y && sphereBot.z > position.z && 
			sphereTop.x < boxTop.x && sphereTop.y < boxTop.y && sphereTop.z < boxTop.z);
}

Vector closestPointOnBlock(Vector center, float radius, Vector position, Vector dimensions, Vector validSides){
	//adjust block size base on valid sides
	if (validSides.x > 0) {
		position.x += dimensions.x;
		dimensions.x = 0;
	}
	else if (validSides.x < 0) {
		dimensions.x = 0;
	}
	if (validSides.y > 0) {
		position.y += dimensions.y;
		dimensions.y = 0;
	}
	else if (validSides.y < 0) {
		dimensions.y = 0;
	}
	if (validSides.z > 0) {
		position.z += dimensions.z;
		dimensions.z = 0;
	}
	else if (validSides.z < 0) {
		dimensions.z = 0;
	}
	//find the nearest point on the modified block
	Vector result;
	//if the center isn't over the x range of the block take an endpoint of the xrange, else take the center as the x point
	if (center.x < position.x){  
		result.x = position.x;
	}
	else if (center.x > position.x + dimensions.x){
		result.x = position.x + dimensions.x;
	}
	else {
		result.x = center.x;
	}

	if (center.y < position.y){  
		result.y = position.y;
	}
	else if (center.y > position.y + dimensions.y){
		result.y = position.y + dimensions.y;
	}
	else {
		result.y = center.y;
	}

	if (center.z < position.z){  
		result.z = position.z;
	}
	else if (center.z > position.z + dimensions.z){
		result.z = position.z + dimensions.z;
	}
	else {
		result.z = center.z;
	}
	return result;
}

bool ellipsoidLineSegmentCollide(Vector center, Vector scale, Vector start, Vector end){
	//transform everything so center is at (0,0,0);
	start = start - center;
	end = end - center;
	//rescale so ellipsoid becomes sphere of radius 1
	start.x = start.x / scale.x;
	start.y = start.y / scale.y;
	start.z = start.z / scale.z;
	end.x = end.x / scale.x;
	end.y = end.y / scale.y;
	end.z = end.z / scale.z;
	//calculate distance from line to origin set to 2 since its larger than radius
	Vector hyp = -1 * start;
	Vector lineDir = end - start;
	float dist = 2;

	if ((lineDir * start) * (lineDir * end) < 0){  //if start and end on opposite sides of the sphere, need to use dist from line
		dist = (hyp.cross(lineDir) * hyp.cross(lineDir)) / (lineDir * lineDir);
	}
	return dist < 1 || start * start < 1 || end * end < 1;
}