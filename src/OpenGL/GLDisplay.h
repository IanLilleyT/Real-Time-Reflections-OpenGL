#pragma once

#include <glm/glm.hpp>
#include <gl3w/gl3w.h>

#include "GLCamera.h" //For handling camera
#include "GLUniformBlockHelper.h" //For calling updates to unform blocks
#include "Framebuffers/GLFramebuffer_Reflection.h" //For color buffer drawing
#include "Framebuffers/GLFramebuffer_ShadowMap.h"
#include "../Utils/EventHandler.h" //For enter frame events
#include "../Utils/Singleton.h" //For accessing several globals
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

private:
	Camera* camera;
	World* world;
	GLFramebuffer_Reflection* reflectionBufferFront;
	GLFramebuffer_Reflection* reflectionBufferBack;
	GLFramebuffer_ShadowMap* shadowMapBuffer;
	PhysicsWorld* physicsWorld;

	void initializeGL();
	void initializeCamera();
	void initializeFramebuffers();
	
	
	void clearGL();
	void updateFramebuffers();

	//IO Events
	void resize(sf::Event sfEvent);
	void mouseButtonPressed(sf::Event sfEvent);
	void mouseMoved(sf::Event sfEvent);
	void mouseWheelMoved(sf::Event sfEvent);
};
