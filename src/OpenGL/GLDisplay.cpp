#include "GLDisplay.h"

GLDisplay::GLDisplay(){}
GLDisplay::~GLDisplay(){}
void GLDisplay::initialize()
{
	//Init GL
	this->initializeGL();

	//Event handlers
	Singleton<EventHandler>::Instance()->addEnterFrameEventListener(EnterFrameReceiver::from_method<GLDisplay,&GLDisplay::update>(this));
	Singleton<EventHandler>::Instance()->addInputEventListener(sf::Event::Resized, InputReceiver::from_method<GLDisplay,&GLDisplay::resizeGL>(this));
	
	//Camera
	Singleton<GLCamera>::Instance()->calcCameraToClipMatrix(45.0f,1.0f,100.0f);
	this->camera = new Camera3rdPerson();
	this->camera->zoom(-10);
	
	//World
	this->world = Singleton<WorldDatabase>::Instance()->loadWorld("World1");
}
void GLDisplay::initializeGL()
{
	gl3wInit();

    //Set GL global vars
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glPointSize(10);
	glLineWidth(2);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Initialize GL helper classes
	Singleton<GLUniformBlockHelper>::Instance()->initialize();
}
void GLDisplay::update()
{
	this->paintGL();
	world->update();
}
void GLDisplay::paintGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void GLDisplay::resizeGL(sf::Event sfEvent)
{
	int width = sfEvent.Size.Width;
	int height = sfEvent.Size.Height;
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	Singleton<GLCamera>::Instance()->changeWindowDimensions(width,height);
}

//World
void GLDisplay::setWorld(World* world)
{
	this->world = world;
}
World* GLDisplay::getWorld()
{
	return this->world;
}

//Camera
void GLDisplay::setCamera(Camera* camera)
{
	this->camera = camera;
}
Camera* GLDisplay::getCamera()
{
	return this->camera;
}