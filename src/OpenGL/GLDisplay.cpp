#include "GLDisplay.h"

GLDisplay::GLDisplay(){}
GLDisplay::~GLDisplay(){}

void GLDisplay::initialize()
{
	//Init GL
	this->initializeGL();

	//Event handlers
	Singleton<EventHandler>::Instance()->addEnterFrameEventListener(EnterFrameReceiver::from_method<GLDisplay,&GLDisplay::update>(this));
	Singleton<EventHandler>::Instance()->addInputEventListener(sf::Event::Resized, InputReceiver::from_method<GLDisplay,&GLDisplay::resize>(this));
	Singleton<EventHandler>::Instance()->addInputEventListener(sf::Event::KeyPressed, InputReceiver::from_method<GLDisplay,&GLDisplay::keyDown>(this));

	//Camera
	Singleton<GLCamera>::Instance()->calcCameraToClipMatrix(45.0f,1.0f,100.0f);
	this->camera = new Camera3rdPerson();
	this->camera->zoom(-1);
	this->camera->rotate(0,-.4f);
}
void GLDisplay::initializeGL()
{
	gl3wInit();
	Singleton<GLUniformBlockHelper>::Instance()->initialize();

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
	glEnable (GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void GLDisplay::update()
{
	Singleton<GLUniformBlockHelper>::Instance()->update();
	this->clearGL();
	if(this->world != 0)
		world->update();
}
void GLDisplay::clearGL()
{
	glClearColor(0.1f, 0.1f, 0.3f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void GLDisplay::resize(sf::Event sfEvent)
{
	int width = sfEvent.Size.Width;
	int height = sfEvent.Size.Height;
	this->resize(width,height);
}
void GLDisplay::resize(int width, int height)
{
	Singleton<GLCamera>::Instance()->changeWindowDimensions(width,height);
}
void GLDisplay::keyDown(sf::Event sfEvent)
{
	sf::Keyboard::Key key = sfEvent.Key.Code;
	if(key == sf::Keyboard::S)
	{
		std::cout << "Pressed a key!" << std::endl;
	}
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