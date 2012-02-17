#include "Cube.h"

std::string Cube::className = "Cube";
Cube::Cube() : RenderObject()
{
	this->type = Cube::className;
}
Cube::Cube(std::string name, std::string material, std::string program)
	: RenderObject(name,"cube",material,program)
{
	this->type = Cube::className;
}
Cube::~Cube(){}