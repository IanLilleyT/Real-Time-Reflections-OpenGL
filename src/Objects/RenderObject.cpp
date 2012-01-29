#include "RenderObject.h"

RenderObject::RenderObject(){}
RenderObject::~RenderObject(){}

void RenderObject::update()
{
	this->render();
}
void RenderObject::render()
{
	//update GLState
}

//Mesh
void RenderObject::setMesh(Mesh* mesh)
{
	this->mesh = mesh;
}
Mesh* RenderObject::getMesh()
{
	return this->mesh;
}

//Material
void RenderObject::setMaterial(Material* material)
{
	this->material = material;
}
Material* RenderObject::getMaterial()
{
	return this->material;
}