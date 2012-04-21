#include "PhysicsObject.h"

//Static vars
std::string PhysicsObject::className = "PhysicsObject";

PhysicsObject::PhysicsObject(){}
PhysicsObject::~PhysicsObject(){}

void PhysicsObject::initialize(PRIMITIVE_TYPE type, RenderObject* attachedRenderObject,
	float mass, float restitution, float friction)
{
	this->attachedRenderObject = attachedRenderObject;
	if(type == PRIMITIVE_BOX)
		this->collisionShape = new btBoxShape(btVector3(0.5f,0.5f,0.5f));
	else if(type == PRIMITIVE_SPHERE)
		this->collisionShape = new btSphereShape(0.5f);
	this->createRigidBody(mass,friction,restitution);
}

//RigidBody
void PhysicsObject::createRigidBody(float mass, float friction, float restitution)
{
	bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0,0,0);
	if (isDynamic)
		this->collisionShape->calculateLocalInertia(mass,localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(btTransform::getIdentity());
	btRigidBody::btRigidBodyConstructionInfo cInfo(mass,myMotionState,this->collisionShape,localInertia);
	this->rigidBody = new btRigidBody(cInfo);
	this->rigidBody->setContactProcessingThreshold(BT_LARGE_FLOAT);
	this->rigidBody->setFriction(friction);
	this->rigidBody->setRestitution(restitution);
	this->rigidBody->setCcdMotionThreshold(.1f);
	this->rigidBody->setCcdSweptSphereRadius(.1f);
}

//Type
std::string PhysicsObject::getClassname()
{
	return PhysicsObject::className;
}

btRigidBody* PhysicsObject::getRigidBody()
{
	return this->rigidBody;
}

//Update
void PhysicsObject::update()
{
	Object::update();
	this->updateTransformationMatrix();
	this->attachedRenderObject->setTransformationMatrix(this->getTransformationMatrix());
}
//Mesh
RenderObject* PhysicsObject::getAttachedRenderObject()
{
	return this->attachedRenderObject;
}

//Mass
void PhysicsObject::setMass(float mass)
{
	this->rigidBody->setMassProps(mass,btVector3(0,0,0));
}
float PhysicsObject::getMass()
{
	return this->rigidBody->getInvMass();
}

//Restitution
void PhysicsObject::setRestitution(float restitution)
{
	this->rigidBody->setRestitution(restitution);
}
float PhysicsObject::getRestitution()
{
	return this->rigidBody->getRestitution();
}

//Friction
void PhysicsObject::setFriction(float friction)
{
	this->rigidBody->setFriction(friction);
}	
float PhysicsObject::getFriction()
{
	return this->rigidBody->getFriction();
}

/*----------------------------------------
			Transformations
----------------------------------------*/
void PhysicsObject::updateTransformationMatrix()
{
	//Scaling is not part of the world transform, so we need to add it manually
	btVector3 btScaleAmount = this->rigidBody->getCollisionShape()->getLocalScaling();
	glm::mat4 scaleMat = glm::mat4();
	scaleMat[0][0] = btScaleAmount.getX();
	scaleMat[1][1] = btScaleAmount.getY();
	scaleMat[2][2] = btScaleAmount.getZ();

	btTransform bulletTransform = this->rigidBody->getWorldTransform();
	this->transformationMatrix = PhysicsUtils::convertBulletTransformToGLM(bulletTransform) * scaleMat;
}
void PhysicsObject::setTransformationMatrix(glm::mat4 transformationMatrix)
{
	assert(false); //this method should not be called
}

//Translation
glm::mat4 PhysicsObject::getTranslationMatrix()
{
	assert(false); //this method should not be called
	return glm::mat4();
}
void PhysicsObject::setTranslationMatrix(glm::mat4 translationMatrix)
{
	assert(false); //this method should not be called
}
void PhysicsObject::translateX(float amount)
{
	btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(amount,0,0);
	bulletTransform.setOrigin(position);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::translateY(float amount)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(0,amount,0);
	bulletTransform.setOrigin(position);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::translateZ(float amount)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(0,0,amount);
	bulletTransform.setOrigin(position);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::translate(glm::vec3 vector)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(vector.x,vector.y,vector.z);
	bulletTransform.setOrigin(position);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::setTranslationX(float amount)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(amount,0,0);
	bulletTransform.setOrigin(position);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::setTranslationY(float amount)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(0,amount,0);
	bulletTransform.setOrigin(position);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::setTranslationZ(float amount)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(0,0,amount);
	bulletTransform.setOrigin(position);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::setTranslation(glm::vec3 vector)
{
	btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btVector3 position = btVector3(vector.x,vector.y,vector.z);
	bulletTransform.setOrigin(position);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}

//Scale
glm::mat4 PhysicsObject::getScaleMatrix()
{
	assert(false); //this method should not be called
	return glm::mat4();
}
void PhysicsObject::setScaleMatrix(glm::mat4 scaleMatrix)
{
	assert(false); //this method should not be called
}
void PhysicsObject::scaleX(float amount)
{
    btVector3 scaleAmount = this->rigidBody->getCollisionShape()->getLocalScaling();
	scaleAmount += btVector3(amount,0,0);
	this->rigidBody->getCollisionShape()->setLocalScaling(scaleAmount);
	this->updateTransformationMatrix();
}
void PhysicsObject::scaleY(float amount)
{
    btVector3 scaleAmount = this->rigidBody->getCollisionShape()->getLocalScaling();
	scaleAmount += btVector3(0,amount,0);
	this->rigidBody->getCollisionShape()->setLocalScaling(scaleAmount);
	this->updateTransformationMatrix();
}
void PhysicsObject::scaleZ(float amount)
{
    btVector3 scaleAmount = this->rigidBody->getCollisionShape()->getLocalScaling();
	scaleAmount += btVector3(0,0,amount);
	this->rigidBody->getCollisionShape()->setLocalScaling(scaleAmount);
	this->updateTransformationMatrix();
}
void PhysicsObject::scale(float amount)
{
    btVector3 scaleAmount = this->rigidBody->getCollisionShape()->getLocalScaling();
	scaleAmount += btVector3(amount,amount,amount);
	this->rigidBody->getCollisionShape()->setLocalScaling(scaleAmount);
	this->updateTransformationMatrix();
}
void PhysicsObject::scale(glm::vec3 vector)
{
	btVector3 scaleAmount = this->rigidBody->getCollisionShape()->getLocalScaling();
	scaleAmount += btVector3(vector.x,vector.y,vector.z);
	this->rigidBody->getCollisionShape()->setLocalScaling(scaleAmount);
	this->updateTransformationMatrix();
}
void PhysicsObject::setScale(float amount)
{
	btVector3 scaleAmount = btVector3(amount,amount,amount);
    this->rigidBody->getCollisionShape()->setLocalScaling(scaleAmount);
	this->updateTransformationMatrix();
}
void PhysicsObject::setScale(glm::vec3 vector)
{
	btVector3 scaleAmount = btVector3(vector.x,vector.y,vector.z);
    this->rigidBody->getCollisionShape()->setLocalScaling(scaleAmount);
	this->updateTransformationMatrix();
}

//Rotation
glm::mat4 PhysicsObject::getRotationMatrix()
{
	assert(false); //this method should not be called
	return glm::mat4();
}
void PhysicsObject::setRotationMatrix(glm::mat4 rotationMatrix)
{
	assert(false); //this method should not be called
}
void PhysicsObject::yaw(float angle)
{
	btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btQuaternion oldOrientation = bulletTransform.getRotation();
	
	btQuaternion newOrientation(btVector3(0,1,0),angle);
	newOrientation = oldOrientation + newOrientation;

	bulletTransform.setRotation(newOrientation);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->transformationMatrix = PhysicsUtils::convertBulletTransformToGLM(bulletTransform);
}
void PhysicsObject::pitch(float angle)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btQuaternion oldOrientation = bulletTransform.getRotation();
	
	btQuaternion newOrientation(btVector3(1,0,0),angle);
	newOrientation = oldOrientation + newOrientation;

	bulletTransform.setRotation(newOrientation);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->transformationMatrix = PhysicsUtils::convertBulletTransformToGLM(bulletTransform);
}
void PhysicsObject::roll(float angle)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btQuaternion oldOrientation = bulletTransform.getRotation();
	
	btQuaternion newOrientation(btVector3(0,0,1),angle);
	newOrientation = oldOrientation + newOrientation;

	bulletTransform.setRotation(newOrientation);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->transformationMatrix = PhysicsUtils::convertBulletTransformToGLM(bulletTransform);
}
void PhysicsObject::rotate(glm::vec3 axis, float angle)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btQuaternion oldOrientation = bulletTransform.getRotation();
	
	axis = glm::normalize(axis);
	btQuaternion newOrientation(btVector3(axis.x,axis.y,axis.z),angle);
	newOrientation = oldOrientation + newOrientation;

	bulletTransform.setRotation(newOrientation);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->transformationMatrix = PhysicsUtils::convertBulletTransformToGLM(bulletTransform);
}
void PhysicsObject::setRotation(glm::vec3 axis, float angle)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	axis = glm::normalize(axis);
	btQuaternion newOrientation(btVector3(axis.x,axis.y,axis.z),angle);
	bulletTransform.setRotation(newOrientation);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->transformationMatrix = PhysicsUtils::convertBulletTransformToGLM(bulletTransform);
}