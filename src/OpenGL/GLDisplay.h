#pragma once

#include <glm/glm.hpp>
#include <gl3w/gl3w.h>

#include "../World.h"
#include "../Cameras/Camera.h"
#include "../Cameras/Camera3rdPerson.h"
#include "../Cameras/Camera1stPerson.h"

/*---------------------------------------------
GLDisplay: Updates objects on the screen, draws them,
contains the camera, and takes events like resize 
and key presses
---------------------------------------------*/
class GLDisplay
{
public:
    GLDisplay();
    ~GLDisplay();
	void initialize();

	//Primary GL methods
	void update();
	
	void resizeGL(glm::uvec2 dimensions);

    Camera* camera;
	World* world;

private:
	void initializeGL();
	void paintGL();
};
