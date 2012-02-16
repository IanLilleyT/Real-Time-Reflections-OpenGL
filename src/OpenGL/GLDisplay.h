#pragma once

#include <glm/glm.hpp>
#include <gl3w/gl3w.h>

#include "GLCamera.h"
#include "GLUniformBlockHelper.h"
#include "GLProgramDatabase.h"
#include "../WorldDatabase.h"
#include "../EventHandler.h"
#include "../Singleton.h"
#include "../World.h"
#include "../Cameras/Camera.h"
#include "../Cameras/Camera3rdPerson.h"
#include "../Cameras/Camera1stPerson.h"
#include "../Objects/RenderObject.h"
#include "../Objects/Jello.h"
#include "../Material.h"
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
	void update();

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
	void resizeGL(sf::Event sfEvent);
	void keyDown(sf::Event sfEvent);
	void initializeGL();
	void clearGL();
};
