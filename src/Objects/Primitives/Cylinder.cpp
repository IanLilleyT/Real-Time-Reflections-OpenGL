#include "Cylinder.h"

std::string Cylinder::className = "Cylinder";
Cylinder::Cylinder() : RenderObject()
{
	this->type = Cylinder::className;
}
Cylinder::Cylinder(std::string name, std::string material, std::string program)
	: RenderObject(name,"cylinder",material,program)
{
	this->type = Cylinder::className;
}
Cylinder::~Cylinder(){}