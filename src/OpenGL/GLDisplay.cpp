#include "GLDisplay.h"

GLDisplay::GLDisplay(){}
GLDisplay::~GLDisplay(){}
void GLDisplay::initialize()
{
	this->camera = new Camera3rdPerson();
	this->camera->zoom(-10);
	this->world = new World();
	this->initializeGL();
}
void GLDisplay::initializeGL()
{
    //Init the gl methods
    gl3wInit();
}
void GLDisplay::update()
{
	world->update();
	this->paintGL();
}
void GLDisplay::paintGL()
{
	//paint stuff - or something
}
void GLDisplay::resizeGL(glm::uvec2 dimensions)
{
	glViewport(0, 0, (GLsizei) dimensions.x, (GLsizei) dimensions.y);
	//fix cam to clip
}