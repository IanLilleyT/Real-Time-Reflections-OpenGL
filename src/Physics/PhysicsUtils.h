#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <bullet/btBulletDynamicsCommon.h>

class PhysicsUtils
{
public:
	static glm::mat4 convertBulletTransformToGLM(btTransform& transform);
	static btTransform convertGLMTransformToBullet(glm::mat4 transform);
	
	static btVector3 convertGLMVectorToBullet(glm::vec3 vector);
	static glm::vec3 convertBulletVectorToGLM(btVector3& vector);
};