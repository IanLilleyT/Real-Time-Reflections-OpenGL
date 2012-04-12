#pragma once

#include <vector>

#include "PhysicsWorld.h"
#include "PhysicsUtils.h"
#include "../Utils/Singleton.h"
#include "../Utils/EventHandler.h"
#include "../Utils/Ray.h"
#include "../OpenGL/GLCamera.h"

class PhysicsIO
{
public:
	PhysicsIO(){}
	~PhysicsIO(){}
	void initialize(PhysicsWorld* physicsWorld, World* world, Camera* camera)
	{
		this->physicsWorld = physicsWorld;
		this->camera = camera;
		this->world = world;

		Singleton<EventHandler>::Instance()->addEnterFrameEventListener(EnterFrameReceiver::from_method<PhysicsIO,&PhysicsIO::update>(this));
		Singleton<EventHandler>::Instance()->addInputEventListener(sf::Event::MouseButtonPressed, InputReceiver::from_method<PhysicsIO,&PhysicsIO::shoot>(this));
	}
	void update(void)
	{
		EventHandler* eventHandler = Singleton<EventHandler>::Instance();

		float forceMagnitude = 12.0f;
		glm::vec3 force = glm::vec3(0,0,0);
		if(eventHandler->isKeyDown(sf::Keyboard::Left))
			force += glm::vec3(-forceMagnitude,0,0);
		if(eventHandler->isKeyDown(sf::Keyboard::Right))
			force += glm::vec3(forceMagnitude,0,0);
		if(eventHandler->isKeyDown(sf::Keyboard::Up))
			force += glm::vec3(0,forceMagnitude,0);
		if(eventHandler->isKeyDown(sf::Keyboard::Down))
			force += glm::vec3(0,-forceMagnitude,0);

		std::vector<PhysicsObject*>& objects = this->physicsWorld->getObjects();
		for(unsigned int i = 0; i < objects.size(); i++)
		{
			PhysicsObject* object = objects.at(i);
			object->getRigidBody()->activate(true);
			object->getRigidBody()->applyCentralForce(PhysicsUtils::convertGLMVectorToBullet(force));
		}
	}
	void shoot(sf::Event sfEvent)
	{
		EventHandler* eventHandler = Singleton<EventHandler>::Instance();

		if(!eventHandler->isAltDown())
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
			glm::vec4 randomColor = glm::vec4(Utils::getRandom(0.0f,1.0f),Utils::getRandom(0,1),Utils::getRandom(0,1),Utils::getRandom(0,1));
			float randomReflectivity = Utils::getRandom(.3f,1.0f);
			fallingObject->getMaterial()->diffuseColor = randomColor;
			fallingObject->getMaterial()->reflectivity = randomReflectivity;

			float forceAmount = 1000.0f;
			Ray shootingRay = Singleton<GLCamera>::Instance()->getPickingRay(sfEvent.MouseButton.X,sfEvent.MouseButton.Y);
			projectile->setTranslation(shootingRay.origin);
			projectile->getRigidBody()->applyCentralForce(forceAmount*PhysicsUtils::convertGLMVectorToBullet(shootingRay.direction));
		
			physicsWorld->addObject(projectile);
			this->world->addObject(fallingObject);
			this->world->addObject(projectile);
		}	
	}

	PhysicsWorld* physicsWorld;
	Camera* camera;
	World* world;
};