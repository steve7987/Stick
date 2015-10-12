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