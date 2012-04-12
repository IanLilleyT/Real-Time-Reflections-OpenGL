#include "RenderObject.h"

//Static vars
std::string RenderObject::className = "RenderObject";

RenderObject::RenderObject() : Object(){}
RenderObject::~RenderObject(){}

//Initialize
void RenderObject::initialize(TiXmlElement* element)
{
	Object::initialize(element);

	//Mesh
	std::string meshName;
	TiXmlElement* meshElement = element->FirstChildElement("mesh");
	if(meshElement != 0) meshName = meshElement->FirstChild()->Value();
	else meshName = MeshDatabase::NONE;

	//Material
	std::string materialName;
	TiXmlElement* materialElement = element->FirstChildElement("material");
	if(materialElement != 0) materialName = materialElement->FirstChild()->Value();
	else materialName = MaterialDatabase::NONE;

	//Program
	std::string programName;
	TiXmlElement* programElement = element->FirstChildElement("program");
	if(programElement != 0) programName = programElement->FirstChild()->Value();
	else programName = GLProgramDatabase::NONE;

	this->initialize(meshName,materialName,programName);

	//Optional settings

	//Color
	TiXmlElement* diffuseColorElement = element->FirstChildElement("diffuseColor");
	if(diffuseColorElement != 0)
	{
		glm::vec4 color = Utils::parseIntoVec4(diffuseColorElement->FirstChild()->Value());
		this->material->diffuseColor = color;
	}
}
void RenderObject::initialize(std::string name, std::string mesh, std::string material, std::string program)
{
	Object::initialize(name);
	this->initialize(mesh,material,program);
}
void RenderObject::initialize(std::string mesh, std::string material, std::string program)
{
	this->setMesh(mesh);
	this->setMaterial(material);
	this->setProgram(program);
}

//Type
std::string RenderObject::getType()
{
	return RenderObject::className;
}

//Render
void RenderObject::render()
{
	Object::render();
	Singleton<GLState>::Instance()->setModelToWorldMatrix(this->transformationMatrix);
	Singleton<GLState>::Instance()->setMaterial(this->material);
	if(this->isVisible())
		this->mesh->Render();
}

//Mesh
void RenderObject::setMesh(std::string name)
{
	GLMesh* newMesh = Singleton<MeshDatabase>::Instance()->loadMesh(name);
	if(newMesh != 0) this->mesh = newMesh;
}
GLMesh* RenderObject::getMesh()
{
	return this->mesh;
}

//Material
void RenderObject::setMaterial(std::string material)
{
	Material* databaseMaterial = Singleton<MaterialDatabase>::Instance()->loadMaterial(material);
	this->material = new Material(*databaseMaterial);
}
Material* RenderObject::getMaterial()
{
	return this->material;
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

//Visible
void RenderObject::setVibisle(bool visible)
{
	this->mesh->setVisible(visible);
}
bool RenderObject::isVisible()
{
	return this->mesh->isVisible();
}