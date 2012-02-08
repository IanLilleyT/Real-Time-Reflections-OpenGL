#include "RenderObject.h"

RenderObject::RenderObject(){}
RenderObject::~RenderObject(){}

void RenderObject::update()
{
	Object::update();
	Singleton<GLState>::Instance()->setModelToWorldMatrix(this->transformationMatrix);
	Singleton<GLState>::Instance()->setMaterial(this->material);
	this->render();
}
void RenderObject::render()
{
	this->mesh->getGLMesh()->Render();
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
void RenderObject::setMaterial(std::string material)
{
	Material* newMaterial = Singleton<MaterialDatabase>::Instance()->loadMaterial(material);
	this->setMaterial(newMaterial);
}
void RenderObject::setMaterial(Material* material)
{
	this->material = material;
}
Material* RenderObject::getMaterial()
{
	return this->material;
}

//Program Type
void RenderObject::setProgramType(std::string programType)
{
	this->mesh->setProgramType(programType);
}
std::string RenderObject::getProgramType()
{
	return this->mesh->getProgramType();
}