#pragma once

#include "PhysicsWorld.h"
#include "PhysicsObject.h"
#include "../World.h"
#include "../Objects/RenderObject.h"

class PhysicsSceneDefault
{
public:
	static PhysicsWorld* getDefaultScene(World* world)
	{
		PhysicsWorld* physicsWorld = new PhysicsWorld();

		//ground plane
		PhysicsSceneDefault::makeWall(physicsWorld, world, glm::vec3(10,1,10),glm::vec3(0,0,0),glm::vec4(0,1,0,1));
		PhysicsSceneDefault::makeWall(physicsWorld, world, glm::vec3(12,5,1),glm::vec3(0,2,-5.5),glm::vec4(0,0,1,1));
		PhysicsSceneDefault::makeWall(physicsWorld, world, glm::vec3(1,5,10),glm::vec3(5.5,2,0),glm::vec4(1,0,1,1));
		PhysicsSceneDefault::makeWall(physicsWorld, world, glm::vec3(1,5,10),glm::vec3(-5.5,2,0),glm::vec4(1,0,0,1));

		//Bullet
		for(int i = 0; i < 50; i++)
		{
			RenderObject* fallingObject = new RenderObject();
			PhysicsObject* projectile = new PhysicsObject();

			int type = i%2;
			if(type == 0)
			{
				fallingObject->initialize("fallingObject","cube","reflective","Material");
				projectile->initialize(
					PRIMITIVE_BOX,
					fallingObject,
					1.0f,0.1f,0.7f);
				float min = .5f;
				float max = 2.0f;
				projectile->setScale(glm::vec3(Utils::getRandom(min,max),Utils::getRandom(min,max),Utils::getRandom(min,max)));
			}
			else
			{
				fallingObject->initialize("fallingObject","sphere","reflective","Material");
				projectile->initialize(
					PRIMITIVE_SPHERE,
					fallingObject,
					1.0f,0.1f,0.7f);
				float min = .5f;
				float max = 2.0f;
				projectile->setScale(Utils::getRandom(min,max));
			}
			
			projectile->setTranslationY(3.0f*i+5.0f);
			physicsWorld->addObject(projectile);
			world->addObject(fallingObject);
			world->addObject(projectile);
		}

		//standing cow
		RenderObject* cow = new RenderObject();
		cow->initialize("floor","cow_soft","refractive","Material");
		cow->getMaterial()->diffuseColor = glm::vec4(1,0,0,1);
		cow->setScale(.5f);
		cow->setTranslationY(5);
		world->addObject(cow);

		return physicsWorld;
	}
	static void makeWall(PhysicsWorld* physicsWorld, World* world, glm::vec3 dimensions, glm::vec3 position, glm::vec4 color)
	{
		RenderObject* floorObject = new RenderObject();
		floorObject->initialize("floor","cube","reflective","Material");
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
};