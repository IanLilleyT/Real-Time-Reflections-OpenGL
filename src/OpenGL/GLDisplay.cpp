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
	Singleton<EventHandler>::Instance()->addInputEventListener(sf::Event::MouseButtonPressed,InputReceiver::from_method<GLDisplay,&GLDisplay::mouseButtonPressed>(this));
	Singleton<EventHandler>::Instance()->addInputEventListener(sf::Event::MouseMoved,InputReceiver::from_method<GLDisplay,&GLDisplay::mouseMoved>(this));
	Singleton<EventHandler>::Instance()->addInputEventListener(sf::Event::MouseWheelMoved,InputReceiver::from_method<GLDisplay,&GLDisplay::mouseWheelMoved>(this));
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
    //glPointSize(10);
	//glLineWidth(2);
	//glEnable (GL_BLEND); 
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonOffset(10.0f,10.0f);
}
void GLDisplay::initializeCamera()
{
	//Camera to Clip
	float fov = 45.0f;
	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
	Singleton<GLCamera>::Instance()->calcCameraToClipMatrix(fov,nearPlane,farPlane);
}
void GLDisplay::initializeFramebuffers()
{
	this->reflectionBufferFront = new GLFramebuffer_Reflection();
	this->reflectionBufferFront->initialize();

	this->reflectionBufferBack = new GLFramebuffer_Reflection();
	this->reflectionBufferBack->initialize();

	this->shadowMapBuffer = new GLFramebuffer_ShadowMap();
	this->shadowMapBuffer->initialize();
}
void GLDisplay::initializePhysics()
{
	//Singleton<PhysicsSceneDefault>::Instance()->makeDefaultScene(this->world);
	//this->physicsWorld = Singleton<PhysicsSceneDefault>::Instance()->getScene();
	//Singleton<PhysicsIO>::Instance()->initialize();
}
void GLDisplay::update()
{ 
	if(this->world != 0)
	{
		GLState* glState = Singleton<GLState>::Instance();
		GLCamera* glCamera = Singleton<GLCamera>::Instance();
		GLUniformBlockHelper* uniformBlockHelper = Singleton<GLUniformBlockHelper>::Instance();

		//Texture stuff
		glState->setReflectionTextures(0,1,2,3);
		glState->depthTextureShadow = 4;
		int textureGroup0 = GLFramebuffer_Reflection::TEXTURE_GROUP0;
		int textureGroup1 = GLFramebuffer_Reflection::TEXTURE_GROUP1;
		
		//Update everything
		//physicsWorld->update();
		world->update();
		glCamera->setWorldToCameraMatrix(this->camera->getWorldToCameraMatrix());
		uniformBlockHelper->updateAll();

		//Diffuse render to texture (front and back faces)
		glState->effectType = GLUniformBlockHelper::DIFFUSE;
		uniformBlockHelper->update(GLUniformBlockHelper::TYPE_EFFECT_TYPE);
		//Front face diffuse render
		this->reflectionBufferFront->bindForWriting(textureGroup0);
		glCullFace(GL_BACK);
		this->clearGL();
		world->render();
		//Back face diffuse render
		//this->reflectionBufferBack->bindForWriting(textureGroup1);
		//glCullFace(GL_FRONT);
		//this->clearGL();
		//world->render();

		//Reflections render to texture (front faces)
		glState->effectType = GLUniformBlockHelper::REFLECTION;
		uniformBlockHelper->update(GLUniformBlockHelper::TYPE_EFFECT_TYPE);
		glCullFace(GL_BACK);
		this->reflectionBufferFront->bindForReadingAndWriting(GL_TEXTURE0,GL_TEXTURE1,textureGroup0,textureGroup1);
		//this->reflectionBufferBack->bindForReadingAndWriting(GL_TEXTURE2,GL_TEXTURE3,textureGroup0,textureGroup1);
		//this->reflectionBufferFront->bindForReading(GL_TEXTURE0,GL_TEXTURE1,textureGroup1);
		//this->reflectionBufferBack->bindForReading(GL_TEXTURE2,GL_TEXTURE3,textureGroup1);
		this->clearGL();
		world->render();

		//Refractions render to screen (front faces)
		glState->effectType = GLUniformBlockHelper::REFRACTION;
		uniformBlockHelper->update(GLUniformBlockHelper::TYPE_EFFECT_TYPE);
		glCullFace(GL_BACK);
		this->reflectionBufferFront->bindForReading(GL_TEXTURE0,GL_TEXTURE1,textureGroup1);
		//this->reflectionBufferBack->bindForReading(GL_TEXTURE2,GL_TEXTURE3,textureGroup1);
		this->clearGL();
		world->render();
	}
}
void GLDisplay::clearGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void GLDisplay::resize(sf::Event sfEvent)
{
	int width = sfEvent.Size.Width;
	int height = sfEvent.Size.Height;
	this->resize(width,height);
}

//IO Events
void GLDisplay::resize(int width, int height)
{
	Singleton<GLCamera>::Instance()->setWindowDimensions(width,height);
}
void GLDisplay::mouseButtonPressed(sf::Event sfEvent)
{
	//Nothing yet
}
void GLDisplay::mouseMoved(sf::Event sfEvent)
{
	EventHandler* eventHandler = Singleton<EventHandler>::Instance();
	glm::ivec2 mousePos = eventHandler->getMousePos();
	glm::ivec2 prevMousePos = eventHandler->getPrevMousePos();
	int mouseXDiff = (mousePos.x - prevMousePos.x);
	int mouseYDiff = (mousePos.y - prevMousePos.y);

	bool altIsDown = eventHandler->isAltDown();
	if(eventHandler->isLeftMouseDown() && altIsDown)
	{	
		float scaleFactor = .008f;
		float mouseXDifference = -(float)mouseXDiff * scaleFactor;
		float mouseYDifference = (float)mouseYDiff * scaleFactor;
		this->camera->rotateRad(mouseXDifference,mouseYDifference);
	}
	else if(eventHandler->isMiddleMouseDown() && altIsDown)
	{
		float scaleFactor = .01f;
		float mouseXDifference = -(float)mouseXDiff * scaleFactor;
		float mouseYDifference = (float)mouseYDiff * scaleFactor;
		this->camera->pan(mouseXDifference,mouseYDifference);
	}
	else if(eventHandler->isRightMouseDown() && altIsDown)
	{
		float scaleFactor = .05f;
		float mouseYDifference = -(float)mouseYDiff * scaleFactor;
		this->camera->zoom(mouseYDifference);
	}
}
void GLDisplay::mouseWheelMoved(sf::Event sfEvent)
{
	int delta = sfEvent.MouseWheel.Delta;
	float scaleFactor = 1.0f;
	this->camera->zoom(delta*scaleFactor);
}

//World
void GLDisplay::setWorld(World* world)
{
	this->world = world;
	this->camera = (Camera3rdPerson*)this->world->getObjectsByType("Camera3rdPerson",true).at(0);
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