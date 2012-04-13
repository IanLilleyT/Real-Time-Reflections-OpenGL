#pragma once

#include <vector>

#include "PhysicsWorld.h"
#include "../Utils/Singleton.h"
#include "../Utils/EventHandler.h"
#include "../Utils/Ray.h"
#include "../OpenGL/GLCamera.h"

class PhysicsIO
{
public:
	PhysicsIO(){}
	~PhysicsIO(){}
	void initialize()
	{
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

		PhysicsWorld* physicsWorld = Singleton<PhysicsSceneDefault>::Instance()->getScene();
		std::vector<PhysicsObject*>& objects = physicsWorld->getObjects();
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
			PhysicsObject* projectile = 0;
			Ray shootingRay = Singleton<GLCamera>::Instance()->getPickingRay(sfEvent.MouseButton.X,sfEvent.MouseButton.Y);
			glm::vec3 position = shootingRay.origin;
			
			if(sfEvent.MouseButton.Button == sf::Mouse::Left)
				projectile = Singleton<PhysicsSceneDefault>::Instance()->makeRandomSphere(position);
			else if(sfEvent.MouseButton.Button = sf::Mouse::Right)
				projectile = Singleton<PhysicsSceneDefault>::Instance()->makeRandomCube(position);

			glm::vec3 torque = Utils::getRandomVec3(0.0f,100.0f);
			float forceAmount = 1000.0f;
			projectile->getRigidBody()->applyTorque(PhysicsUtils::convertGLMVectorToBullet(torque));
			projectile->getRigidBody()->applyCentralForce(forceAmount*PhysicsUtils::convertGLMVectorToBullet(shootingRay.direction));

		}	
	}
};