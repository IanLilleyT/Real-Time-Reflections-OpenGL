#include "RenderObject.h"

RenderObject::RenderObject() : Object() {}
RenderObject::~RenderObject(){}


//Initialize
void RenderObject::initialize(TiXmlElement* element)
{
	Object::initialize(element);

	//Mesh
	std::string meshName;
	TiXmlElement* meshElement = element->FirstChildElement("mesh");
	if(meshElement != 0) meshName = meshElement->FirstChild()->Value();
	else meshName = MeshDatabase::NO_NAME;

	//Material
	std::string materialName;
	TiXmlElement* materialElement = element->FirstChildElement("material");
	if(materialElement != 0) materialName = materialElement->FirstChild()->Value();
	else materialName = MaterialDatabase::NO_NAME;

	//Program
	std::string programName;
	TiXmlElement* programElement = element->FirstChildElement("program");
	if(programElement != 0) programName = programElement->FirstChild()->Value();
	else programName = GLProgramDatabase::NO_NAME;

	this->initialize(meshName,materialName,programName);
}
void RenderObject::initialize(std::string type, std::string name, std::string mesh, std::string material, std::string program)
{
	Object::initialize(type,name);
	this->initialize(mesh,material,program);
}
void RenderObject::initialize(std::string mesh, std::string material, std::string program)
{
	this->setMesh(mesh);
	this->setMaterial(material);
	this->setProgram(program);
}

//Update
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
	//Only set a new mesh if it is valid
	GLMesh* newMesh = Singleton<MeshDatabase>::Instance()->loadMesh(name);
	if(newMesh != 0) this->mesh = newMesh;
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