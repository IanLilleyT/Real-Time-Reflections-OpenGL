#pragma once

#include "PhysicsWorld.h"
#include "PhysicsObject.h"
#include "../World.h"
#include "../Objects/RenderObject.h"
#include "../Utils/Utils.h"

class PhysicsSceneDefault
{
public:
	PhysicsSceneDefault(){}
	~PhysicsSceneDefault(){}
	PhysicsWorld* getScene()
	{
		return this->physicsWorld;
	}
	void makeDefaultScene(World* world)
	{
		this->physicsWorld = new PhysicsWorld();
		this->world = world;

		//walls
		this->makeWall(glm::vec3(10,1,10),glm::vec3(0,0,0),glm::vec4(0,1,0,1),"reflective");
		this->makeWall(glm::vec3(12,5,1),glm::vec3(0,2,5.5),glm::vec4(0,0,1,1),"reflective");
		this->makeWall(glm::vec3(1,5,10),glm::vec3(5.5,2,0),glm::vec4(1,0,1,1),"reflective");
		this->makeWall(glm::vec3(1,5,10),glm::vec3(-5.5,2,0),glm::vec4(1,0,0,1),"reflective");
		//this->makeWall(glm::vec3(5,1,5),glm::vec3(0,2.5,0),glm::vec4(0,1,0,1),"reflective");

		//Bullet
		int numObjects = 100;
		for(int i = 0; i < numObjects; i++)
		{
			glm::vec3 position = glm::vec3(0,3.0f*i+5.0f,0);
			int type = i%2;
			if(type == 0)
				this->makeRandomCube(position);
			else
				this->makeRandomSphere(position);
		}

		//standing cow
		/*RenderObject* cow = new RenderObject();
		cow->initialize("floor","cube","refractive","Material");
		cow->getMaterial()->diffuseColor = glm::vec4(1,0,0,1);
		cow->setScale(glm::vec3(5,1,5));
		cow->setTranslationY(5.5);
		world->addObject(cow);*/
	}
	PhysicsObject* makeRandomCube(glm::vec3 position)
	{
		RenderObject* fallingObject = new RenderObject();	
		PhysicsObject* projectile = new PhysicsObject();
		fallingObject->initialize("cube","cube","reflective","Material");
		projectile->initialize(
			PRIMITIVE_BOX,
			fallingObject,
			1.0f,0.1f,0.7f);
		projectile->setScale(Utils::getRandomVec3(.5f,2.0f));
			
		glm::vec4 randomColor = Utils::getRandomColor();
		float randomReflectivity = Utils::getRandom(.3f,0.5f);
		fallingObject->getMaterial()->diffuseColor = randomColor;
		fallingObject->getMaterial()->reflectivity = randomReflectivity;

		projectile->setTranslation(position);
		physicsWorld->addObject(projectile);
		world->addObject(fallingObject);
		world->addObject(projectile);

		return projectile;
	}
	PhysicsObject* makeRandomSphere(glm::vec3 position)
	{
		RenderObject* fallingObject = new RenderObject();	
		PhysicsObject* projectile = new PhysicsObject();

		fallingObject->initialize("fallingObject","sphere","reflective","Material");
		projectile->initialize(
			PRIMITIVE_SPHERE,
			fallingObject,
			1.0f,0.1f,0.7f);
		float min = .5f;
		float max = 2.0f;
		projectile->setScale(Utils::getRandom(min,max));
		
		glm::vec4 randomColor = Utils::getRandomColor();
		float randomReflectivity = Utils::getRandom(.3f,0.5f);
		fallingObject->getMaterial()->diffuseColor = randomColor;
		fallingObject->getMaterial()->reflectivity = randomReflectivity;

		projectile->setTranslation(position);
		physicsWorld->addObject(projectile);
		world->addObject(fallingObject);
		world->addObject(projectile);

		return projectile;
	}
	void makeWall(glm::vec3 dimensions, glm::vec3 position, glm::vec4 color, std::string material)
	{
		RenderObject* floorObject = new RenderObject();
		floorObject->initialize("floor","cube",material,"Material");
		floorObject->getMaterial()->diffuseColor = color;
		PhysicsObject* floorPhysics = new PhysicsObject();
		floorPhysics->initialize(
			PRIMITIVE_BOX,
			floorObject,
			0.0f,0.1f,0.8f);
		floorPhysics->setScale(dimensions);
		floorPhysics->setTranslation(position);
		physicsWorld->addObject(floorPhysics);
		world->addObject(floorObject);
		world->addObject(floorPhysics);
	}

	PhysicsWorld* physicsWorld;
	World* world;
};