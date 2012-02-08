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
	std::string worldPath = Singleton<GlobalValues>::Instance()->getWorldPath();
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
			{
				this->processRenderObject(file, line, world);
			}
			else if(line == "Jello")
			{
				this->processJello(file, line, world);
			}
			else if(line == "Light")
			{
				this->processLight(file, line, world);
			}
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

bool WorldDatabase::isFieldValid(std::ifstream& file, std::string name, std::vector<std::string>& results)
{
	std::string line;
	getline(file, line);
	results = Utils::splitByCharacter(line, ' ');
	return results.at(0) == name;
}

void WorldDatabase::processRenderObject(std::ifstream& file, std::string type, World* world)
{
	std::vector<std::string> results;

	RenderObject* renderObject = new RenderObject();
	renderObject->setType(type);
	world->addObject(renderObject);
				
	//Load name
	if(this->isFieldValid(file,"name",results))
	{
		std::string name = results.at(1);
		renderObject->setName(name);
	}

	//Load mesh
	if(this->isFieldValid(file,"mesh",results))
	{
		std::string meshFileName = results.at(1);
		GLMesh* mesh = Singleton<MeshDatabase>::Instance()->loadMesh(meshFileName);
		renderObject->setMesh(mesh);
	}
				
	//Load material
	if(this->isFieldValid(file,"material",results))
	{
		std::string materialFileName = results.at(1);
		renderObject->setMaterial(materialFileName);
	}
				
	//Load program
	if(this->isFieldValid(file,"program",results))
	{
		std::string programType = results.at(1);
		renderObject->setProgramType(programType);
	}
				
	//Load translation
	if(this->isFieldValid(file,"translation",results))
	{
		glm::vec3 translation = Utils::parseIntoVec3(results);
		renderObject->setTranslation(translation);
	}
				
	//Load rotation
	if(this->isFieldValid(file,"rotation",results))
	{
		glm::vec4 rotation = Utils::parseIntoVec4(results);
		glm::vec3 axis = glm::vec3(rotation.x,rotation.y,rotation.z);
		float angle = rotation.w;
		renderObject->setRotation(axis,angle);
	}
				
	//Load scale
	if(this->isFieldValid(file,"scale",results))
	{
		glm::vec3 scale = Utils::parseIntoVec3(results);
		renderObject->setScale(scale);	
	}
}
void WorldDatabase::processJello(std::ifstream& file, std::string type, World* world)
{
	/*
	std::vector<std::string> results;

	Jello* jello = new Jello();
	jello->setType(type);
	world->addObject(jello);
				
	//Load name
	if(this->isFieldValid(file,"name",results))
	{
		std::string name = results.at(1);
		renderObject->setName(name);
	}

	//Load mesh
	if(this->isFieldValid(file,"mesh",results))
	{
		std::string meshFileName = results.at(1);
		GLMesh* mesh = Singleton<MeshDatabase>::Instance()->loadMesh(meshFileName);
		renderObject->setMesh(mesh);
	}
				
	//Load material
	if(this->isFieldValid(file,"material",results))
	{
		std::string materialFileName = results.at(1);
		renderObject->setMaterial(materialFileName);
	}
				
	//Load program
	if(this->isFieldValid(file,"program",results))
	{
		std::string programType = results.at(1);
		renderObject->setProgramType(programType);
	}
				
	//Load translation
	if(this->isFieldValid(file,"translation",results))
	{
		glm::vec3 translation = Utils::parseIntoVec3(results);
		renderObject->setTranslation(translation);
	}
				
	//Load rotation
	if(this->isFieldValid(file,"rotation",results))
	{
		glm::vec4 rotation = Utils::parseIntoVec4(results);
		glm::vec3 axis = glm::vec3(rotation.x,rotation.y,rotation.z);
		float angle = rotation.w;
		renderObject->setRotation(axis,angle);
	}
				
	//Load scale
	if(this->isFieldValid(file,"scale",results))
	{
		glm::vec3 scale = Utils::parseIntoVec3(results);
		renderObject->setScale(scale);	
	}*/
}
void WorldDatabase::processLight(std::ifstream& file, std::string type, World* world)
{
	std::vector<std::string> results;

	Light* light = new Light();
	light->setType(type);
	world->addObject(light);
				
	//Load name
	if(this->isFieldValid(file,"name",results))
	{
		std::string lightName = results.at(1);
		light->setName(lightName);
	}

	//Load intensity
	if(this->isFieldValid(file,"intensity",results))
	{
		glm::vec4 intensity = Utils::parseIntoVec4(results);
		light->setIntensity(intensity);
	}

	//Load translation
	if(this->isFieldValid(file,"translation",results))
	{
		glm::vec3 translation = Utils::parseIntoVec3(results);
		light->setTranslation(translation);
	}
}