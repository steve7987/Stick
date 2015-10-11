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