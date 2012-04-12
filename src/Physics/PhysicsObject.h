#pragma once

#include <iostream>

#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include "../Objects/Object.h"
#include "../Objects/RenderObject.h"
#include "../OpenGL/GLMesh.h"
#include "../Utils/Utils.h"
#include "PhysicsUtils.h"

/*--------------------------------------
PhysicsObject has physics properties for
integration with Bullet.
--------------------------------------*/

enum PRIMITIVE_TYPE {
	PRIMITIVE_BOX,       //box
	PRIMITIVE_SPHERE,    //sphere
	PRIMITIVE_CYLINDER,  //cylinder
	PRIMITIVE_CONE,      //cone
	PRIMITIVE_CAPSULE,   //capsule
	PRIMITIVE_MESH,      //mesh
};

class PhysicsObject : public Object
{
public:
	//Class
	static std::string className;

	//Constructors
	PhysicsObject();
	virtual ~PhysicsObject();

	//initialize
	void initialize(PRIMITIVE_TYPE type, RenderObject* attachedRenderObject,
		float mass, float restitution, float friction);

	//update
	virtual void update();

	//Type
	virtual std::string getType();

	//Rigidbody
	btRigidBody* getRigidBody();

	//Mesh
	RenderObject* getAttachedRenderObject();

	//Mass
	void setMass(float mass);
	float getMass();
	
	//Restitution
	void setRestitution(float restitution);
	float getRestitution();

	//Friction
	void setFriction(float friction);
	float getFriction();

	//Transformations
	virtual void setTransformationMatrix(glm::mat4 transformationMatrix);

	//Translation
	virtual glm::mat4 getTranslationMatrix();
	virtual void setTranslationMatrix(glm::mat4 translationMatrix);
	virtual void translateX(float amount);
    virtual void translateY(float amount);
    virtual void translateZ(float amount);
    virtual void translate(glm::vec3 vector);
    virtual void setTranslationX(float amount);
    virtual void setTranslationY(float amount);
    virtual void setTranslationZ(float amount);
    virtual void setTranslation(glm::vec3 vector);

	//Scale
	virtual glm::mat4 getScaleMatrix();
	virtual void setScaleMatrix(glm::mat4 scaleMatrix);
	virtual void scaleX(float amount);
    virtual void scaleY(float amount);
    virtual void scaleZ(float amount);
    virtual void scale(float amount);
    virtual void scale(glm::vec3 vector);
    virtual void setScale(float amount);
    virtual void setScale(glm::vec3 vector);

	//Rotation
	virtual glm::mat4 getRotationMatrix();
	virtual void setRotationMatrix(glm::mat4 rotationMatrix);
    virtual void yaw(float angle);
    virtual void pitch(float angle);
    virtual void roll(float angle);
    virtual void rotate(glm::vec3 axis, float angle);
    virtual void setRotation(glm::vec3 axis, float angle);

private:

	void createRigidBody(float mass, float friction, float restitution);
	virtual void updateTransformationMatrix();

	RenderObject* attachedRenderObject;
	btCollisionShape* collisionShape;
	btRigidBody* rigidBody;
};

