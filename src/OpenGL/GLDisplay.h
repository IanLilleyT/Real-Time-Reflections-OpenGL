#pragma once

#include <glm/glm.hpp>
#include <gl3w/gl3w.h>

#include "GLCamera.h" //For handling camera
#include "GLUniformBlockHelper.h" //For calling updates to unform blocks
#include "../EventHandler.h" //For enter frame events
#include "../Singleton.h" //For accessing several globals
#include "../World.h" //For storing world
#include "../Cameras/Camera.h" //For using different camera types
#include "../Cameras/Camera3rdPerson.h" //For using different camera types
#include "../Cameras/Camera1stPerson.h" //For using different camera types

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

	//GL methods
	void resize(sf::Event sfEvent);
	void initializeGL();
	void clearGL();
};
