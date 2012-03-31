#include "GLDisplay.h"

GLDisplay::GLDisplay(){}
GLDisplay::~GLDisplay(){}

void GLDisplay::initialize()
{
	//Initializers
	this->initializeGL();
	this->initializeCamera();
	this->initializeFramebuffers();

	//Event handlers
	Singleton<EventHandler>::Instance()->addEnterFrameEventListener(EnterFrameReceiver::from_method<GLDisplay,&GLDisplay::update>(this));
	Singleton<EventHandler>::Instance()->addInputEventListener(sf::Event::Resized,InputReceiver::from_method<GLDisplay,&GLDisplay::resize>(this));
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
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonOffset(10.0f,10.0f);
}
void GLDisplay::initializeCamera()
{
	//Camera
	float fov = 45.0f;
	float nearPlane = 0.1f;
	float farPlane = 1000.0f;

	this->camera = new Camera3rdPerson();
	this->camera->zoom(-1);
	this->camera->rotate(0,-.4f);

	Singleton<GLCamera>::Instance()->calcCameraToClipMatrix(fov,nearPlane,farPlane);
}
void GLDisplay::initializeFramebuffers()
{
	this->reflectionBuffer = new GLFramebuffer();
	this->reflectionBuffer->initialize();
}
void GLDisplay::update()
{
	
	
	
	
	if(this->world != 0)
	{
		GLState* glState = Singleton<GLState>::Instance();
		GLUniformBlockHelper* uniformBlockHelper = Singleton<GLUniformBlockHelper>::Instance();

		world->update();

		//Render to framebuffer
		this->clearGL();
		this->reflectionBuffer->bindForWriting();
		glState->setReflectionToggle(0);
		uniformBlockHelper->updateAll();
		world->render();

		//Render for real
		this->clearGL();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		this->reflectionBuffer->bindForReading();
		glState->setReflectionToggle(1);
		uniformBlockHelper->update(GLUniformBlockHelper::TYPE_REFLECTION_TOGGLE);
		world->render();
	}
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
	Singleton<GLCamera>::Instance()->setWindowDimensions(width,height);
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