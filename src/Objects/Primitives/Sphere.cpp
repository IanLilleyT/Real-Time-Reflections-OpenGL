#include "Sphere.h"

std::string Sphere::className = "Sphere";
Sphere::Sphere() : RenderObject()
{
	this->type = Sphere::className;
}
Sphere::Sphere(std::string name, std::string material, std::string program)
	: RenderObject(name,"sphere",material,program)
{
	this->type = Sphere::className;
}
Sphere::~Sphere(){}