#pragma once

#include <glm/glm.hpp>
#include <gl3w/gl3w.h>

#include "GLView.h" //For handling camera
#include "GLUniformBlockHelper.h" //For calling updates to unform blocks
#include "Framebuffers/GLFramebuffer_GBuffer.h"
#include "Framebuffers/GLFramebuffer_ColorBuffer.h"
#include "Framebuffers/GLFramebuffer_DepthBuffer.h"
#include "Programs/GLProgramDatabase.h"
#include "../Utils/EventHandler.h" //For enter frame events
#include "../Utils/Singleton.h" //For accessing several globals
#include "../Utils/IntersectionAlgorithms.h"
#include "../World.h" //For storing world
#include "../Objects/Cameras/Camera.h" //For using different camera types
#include "../Objects/Cameras/Camera3rdPerson.h" //For using different camera types
#include "../Objects/Cameras/Camera1stPerson.h" //For using different camera types
#include "../Objects/RenderObject.h"
#include "../Physics/PhysicsWorld.h"
#include "../Physics/PhysicsObject.h"
#include "../Physics/PhysicsSceneDefault.h"
#include "../Physics/PhysicsIO.h"
#include "../Objects/Lights/ShadowLight.h"
#include "../MaterialDatabase.h"

/*---------------------------------------------
GLDisplay: Essentially a bridge between
OpenGL and the World. Updates objects on the screen, 
draws them, contains the camera, world, and more, 
and takes events like resize and key presses.
---------------------------------------------*/

class GLDisplay
{
public:
    GLDisplay();
    ~GLDisplay();

	void initialize();
	void initializePhysics();
	void update();
	void resize(int width, int height);

	//World
    World* getWorld();
	void setWorld(World* world);

	//Camera
	Camera* getCamera();
	void setCamera(Camera* camera);

	//Physics World
	PhysicsWorld* getPhysicsWorld();

private:
	Camera* camera;
	World* world;
	PhysicsWorld* physicsWorld;
	GLFramebuffer_GBuffer* gbufferFBO;
	GLFramebuffer_ColorBuffer* colorBufferFBO;
	GLFramebuffer_DepthBuffer* shadowMapFBO;
	GLMesh* fullScreenQuadMesh;
	RenderObject* lightSphere;
	std::vector<RenderObject*> refractiveObjects;
	std::vector<RenderObject*> nonRefractiveObjects;

	void initializeGL();
	void initializeCamera();
	void initializeFramebuffers();
	void clearGL();

	//IO Events
	void resize(sf::Event sfEvent);
	void mouseButtonPressed(sf::Event sfEvent);
	void mouseMoved(sf::Event sfEvent);
	void mouseWheelMoved(sf::Event sfEvent);
	void checkKeyPress();

	//THERE SHOULD BE A BETTER PLACE TO DO THIS
	RenderObject* selectedObject;
};
