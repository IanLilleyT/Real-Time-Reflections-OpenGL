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

	file.open(fullPath.c_str(), std::ifstream::in);
	if(file.is_open())
	{
		World* world = new World();
		while(file.good())
		{
			getline(file, line);
			if(line == "RenderObject")
			{
				RenderObject* renderObject = new RenderObject();
				world->addRenderObject(renderObject);
				std::vector<std::string> results;
				
				//Load mesh
				getline(file, line);
				results = Utils::splitByCharacter(line,' ');
				std::string meshFileName = results.at(1);
				Mesh* mesh = Singleton<MeshDatabase>::Instance()->loadMesh(meshFileName);
				renderObject->setMesh(mesh);

				//Load name
				getline(file, line);
				results = Utils::splitByCharacter(line,' ');
				std::string name = results.at(1);
				renderObject->setName(name);

				//Load material
				getline(file, line);
				results = Utils::splitByCharacter(line,' ');
				std::string materialFileName = results.at(1);
				renderObject->setMaterial(materialFileName);

				//Load program
				getline(file, line);
				results = Utils::splitByCharacter(line, ' ');
				std::string programType = results.at(1);
				renderObject->setProgramType(programType);

				//Load transformations
				getline(file, line);
				glm::vec3 translation = Utils::parseIntoVec3(line);
				getline(file, line);
				glm::vec4 rotation = Utils::parseIntoVec4(line);
				glm::vec3 axis = glm::vec3(rotation.x,rotation.y,rotation.z);
				float angle = rotation.w;
				getline(file, line);
				glm::vec3 scale = Utils::parseIntoVec3(line);
				renderObject->setTranslation(translation);
				renderObject->setRotation(axis,angle);
				renderObject->setScale(scale);	
			}
			else if(line == "Light")
			{
				Light* light = new Light();
				world->addLight(light);
				std::vector<std::string> results;
				
				//Load name
				getline(file, line);
				results = Utils::splitByCharacter(line,' ');
				std::string lightName = results.at(1);
				light->setName(lightName);

				//Load intensity
				getline(file, line);
				glm::vec4 intensity = Utils::parseIntoVec4(line);
				light->setIntensity(intensity);

				//Load transformations
				getline(file, line);
				glm::vec3 translation = Utils::parseIntoVec3(line);
				light->setTranslation(translation);
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