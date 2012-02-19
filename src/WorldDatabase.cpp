#include "WorldDatabase.h"

WorldDatabase::WorldDatabase(){}
WorldDatabase::~WorldDatabase(){}

World* WorldDatabase::loadWorld(std::string worldFileName)
{
	World* newWorld = 0;

	std::string extension = ".world";
	if(worldFileName.find(extension) == std::string::npos)
		worldFileName += extension;

	newWorld = this->findWorld(worldFileName);
	if(newWorld == 0) 
	{
		newWorld = this->openWorldFile(worldFileName);
		if(newWorld != 0)
			this->worldMap[worldFileName] = newWorld;
	}
	return newWorld;
}
World* WorldDatabase::findWorld(std::string worldFileName)
{
	std::map<std::string, World*>::iterator it;
	it = this->worldMap.find(worldFileName);
	if(it != this->worldMap.end())
		return it->second;
	else
		return 0;
}
World* WorldDatabase::openWorldFile(std::string worldFileName)
{
	std::string worldPath = Singleton<GlobalPaths>::Instance()->getWorldPath();
	std::string fullPath = worldPath + worldFileName;

	std::string line;
	std::ifstream file;
	std::vector<std::string> results;

	file.open(fullPath.c_str(), std::ifstream::in);
	if(file.is_open())
	{
		World* world = new World();
		while(file.good())
		{
			getline(file, line);
			if(line == "RenderObject")
				this->processRenderObject(file, world);
			else if(line == "Sphere")
				this->processSphere(file,world);
			else if(line == "Cylinder")
				this->processCylinder(file,world);
			else if(line == "Jello")
				this->processJello(file, world);
			else if(line == "Light")
				this->processLight(file, world);
		}
		file.close();
		return world;
	}
	else
	{
		std::cout << "Unable to open world file." << std::endl;
		return 0;
	}
}

void WorldDatabase::processRenderObject(std::ifstream& file, World* world)
{
	std::vector<std::string> results;		
	std::string name;
	std::string mesh;
	std::string material;
	std::string program;
	glm::vec3 translation;
	glm::vec3 axis;
	float angle;
	glm::vec3 scale;

	//Load name
	if(this->isFieldValid(file,"name",results))
		name = results.at(1);
	//Load mesh
	if(this->isFieldValid(file,"mesh",results))
		mesh = results.at(1);		
	//Load material
	if(this->isFieldValid(file,"material",results))
		material = results.at(1);		
	//Load program
	if(this->isFieldValid(file,"program",results))
		program = results.at(1);
	//Load translation
	if(this->isFieldValid(file,"translation",results))
		translation = Utils::parseIntoVec3(results);
	//Load rotation
	if(this->isFieldValid(file,"rotation",results))
	{
		glm::vec4 rotation = Utils::parseIntoVec4(results);
		axis = glm::vec3(rotation.x,rotation.y,rotation.z);
		angle = rotation.w;
	}		
	//Load scale
	if(this->isFieldValid(file,"scale",results))
		scale = Utils::parseIntoVec3(results);

	//Initialize
	RenderObject* renderObject = new RenderObject(name,mesh,material,program);
	renderObject->setTranslation(translation);
	renderObject->setRotation(axis,angle);
	renderObject->setScale(scale);
	world->addObject(renderObject);
}
void WorldDatabase::processSphere(std::ifstream& file, World* world)
{
	std::vector<std::string> results;		
	std::string name;
	std::string material;
	std::string program;
	glm::vec3 translation;
	glm::vec3 axis;
	float angle;
	glm::vec3 scale;

	//Load name
	if(this->isFieldValid(file,"name",results))
		name = results.at(1);		
	//Load material
	if(this->isFieldValid(file,"material",results))
		material = results.at(1);		
	//Load program
	if(this->isFieldValid(file,"program",results))
		program = results.at(1);
	//Load translation
	if(this->isFieldValid(file,"translation",results))
		translation = Utils::parseIntoVec3(results);
	//Load rotation
	if(this->isFieldValid(file,"rotation",results))
	{
		glm::vec4 rotation = Utils::parseIntoVec4(results);
		axis = glm::vec3(rotation.x,rotation.y,rotation.z);
		angle = rotation.w;
	}		
	//Load scale
	if(this->isFieldValid(file,"scale",results))
		scale = Utils::parseIntoVec3(results);

	//Initialize
	Sphere* sphere = new Sphere(name,material,program);
	sphere->setTranslation(translation);
	sphere->setRotation(axis,angle);
	sphere->setScale(scale);
	world->addObject(sphere);
}
void WorldDatabase::processCylinder(std::ifstream& file, World* world)
{
	std::vector<std::string> results;		
	std::string name;
	std::string material;
	std::string program;
	glm::vec3 translation;
	glm::vec3 axis;
	float angle;
	glm::vec3 scale;

	//Load name
	if(this->isFieldValid(file,"name",results))
		name = results.at(1);		
	//Load material
	if(this->isFieldValid(file,"material",results))
		material = results.at(1);		
	//Load program
	if(this->isFieldValid(file,"program",results))
		program = results.at(1);
	//Load translation
	if(this->isFieldValid(file,"translation",results))
		translation = Utils::parseIntoVec3(results);
	//Load rotation
	if(this->isFieldValid(file,"rotation",results))
	{
		glm::vec4 rotation = Utils::parseIntoVec4(results);
		axis = glm::vec3(rotation.x,rotation.y,rotation.z);
		angle = rotation.w;
	}		
	//Load scale
	if(this->isFieldValid(file,"scale",results))
		scale = Utils::parseIntoVec3(results);

	//Initialize
	Cylinder* cylinder = new Cylinder(name,material,program);
	cylinder->setTranslation(translation);
	cylinder->setRotation(axis,angle);
	cylinder->setScale(scale);
	world->addObject(cylinder);
}
void WorldDatabase::processJello(std::ifstream& file, World* world)
{
	std::vector<std::string> results;
	glm::vec3 origin;
	glm::vec3 size;
	glm::uvec3 divisions;
	std::string name;
	std::string material;
	std::string program;
				
	//Load name
	if(this->isFieldValid(file,"name",results))
		name = results.at(1);
	//Load translation
	if(this->isFieldValid(file,"origin",results))
		origin = Utils::parseIntoVec3(results);
	//Load scale
	if(this->isFieldValid(file,"size",results))
		size = Utils::parseIntoVec3(results);
	//Load divisions
	if(this->isFieldValid(file,"divisions",results))
		divisions = Utils::parseIntoUVec3(results);
	//Load material
	if(this->isFieldValid(file,"material",results))
		material = results.at(1);	
	//Load program
	if(this->isFieldValid(file,"program",results))
		program = results.at(1);

	//Initialize
	Jello* jello = new Jello(name,material,program,origin,size,divisions);
	world->addObject(jello);
}

void WorldDatabase::processLight(std::ifstream& file, World* world)
{
	std::vector<std::string> results;
	std::string name;
	glm::vec4 intensity;
	glm::vec3 translation;
		
	//Load name
	if(this->isFieldValid(file,"name",results))
		name = results.at(1);
	//Load intensity
	if(this->isFieldValid(file,"intensity",results))
		intensity = Utils::parseIntoVec4(results);
	//Load translation
	if(this->isFieldValid(file,"translation",results))
		translation = Utils::parseIntoVec3(results);

	//Initialize
	Light* light = new Light(name,intensity);
	light->setTranslation(translation);
	world->addObject(light);

	//Load physics representation
	//RenderObject* sphere = new RenderObject("helperLight","sphere","material1","Material");
	//sphere->setTranslation(light->getPosition());
	//sphere->setScale(.2f);
	//world->addObject(sphere);
}

bool WorldDatabase::isFieldValid(std::ifstream& file, std::string name, std::vector<std::string>& results)
{
	std::string line;
	getline(file, line);
	results = Utils::splitByCharacter(line, ' ');
	return results.at(0) == name;
}