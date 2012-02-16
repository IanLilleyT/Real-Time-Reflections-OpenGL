#include "RenderObject.h"

std::string RenderObject::className = "RenderObject";
RenderObject::RenderObject() : Object() 
{
	this->type = Object::className;
}
RenderObject::RenderObject(std::string name) : Object(name)
{
	this->type = RenderObject::className;
}
RenderObject::RenderObject(std::string name, std::string mesh, std::string material, std::string program) : Object(name) 
{
	this->type = RenderObject::className;
	this->setMesh(mesh);
	this->setMaterial(material);
	this->setProgram(program);
}
RenderObject::~RenderObject(){}

void RenderObject::update()
{
	this->render();
}
void RenderObject::render()
{
	Singleton<GLState>::Instance()->setModelToWorldMatrix(this->transformationMatrix);
	Singleton<GLState>::Instance()->setMaterial(this->material);
	if(this->mesh->isVisible())
		this->mesh->Render();
}

//Mesh
void RenderObject::setMesh(std::string name)
{
	this->mesh = Singleton<MeshDatabase>::Instance()->loadMesh(name);
}
std::string RenderObject::getMesh()
{
	return this->mesh->getName();
}

//Material
void RenderObject::setMaterial(std::string material)
{
	this->material = Singleton<MaterialDatabase>::Instance()->loadMaterial(material);
}
std::string RenderObject::getMaterial()
{
	return this->material->getName();
}

//Program Type
void RenderObject::setProgram(std::string programName)
{
	this->mesh->setProgram(programName);
}
std::string RenderObject::getProgram()
{
	return this->mesh->getProgram();
}