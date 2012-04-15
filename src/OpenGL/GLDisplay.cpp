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
    //glPointSize(10);
	//glLineWidth(2);
	//glEnable (GL_BLEND); 
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonOffset(10.0f,10.0f);
}
void GLDisplay::initializeCamera()
{
	//Camera
	float fov = 45.0f;
	float nearPlane = 0.1f;
	float farPlane = 100.0f;

	this->camera = new Camera3rdPerson();
	this->camera->zoom(-1);
	this->camera->rotate(0,-.4f);

	Singleton<GLCamera>::Instance()->calcCameraToClipMatrix(fov,nearPlane,farPlane);
}
void GLDisplay::initializeFramebuffers()
{
	this->reflectionBufferFront = new GLFramebuffer_Reflection();
	this->reflectionBufferFront->initialize();

	this->reflectionBufferBack = new GLFramebuffer_Reflection();
	this->reflectionBufferBack->initialize();
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
		GLUniformBlockHelper* uniformBlockHelper = Singleton<GLUniformBlockHelper>::Instance();

		//physicsWorld->update();
		world->update();
		uniformBlockHelper->updateAll();

		//Texture types
		GLenum textureUnitsFront[2] = {GL_TEXTURE0, GL_TEXTURE1};
		GLenum textureUnitsBack[2] =  {GL_TEXTURE2, GL_TEXTURE3};

		//Diffuse render to texture (front and back faces)
		glState->setEffectType(GLUniformBlockHelper::DIFFUSE);
		uniformBlockHelper->update(GLUniformBlockHelper::TYPE_EFFECT_TYPE);

		//Front face diffuse render
		glCullFace(GL_BACK);
		this->reflectionBufferFront->bindForWriting();
		this->clearGL();
		world->render();
		
		//Back face diffuse render
		glCullFace(GL_FRONT);
		this->reflectionBufferBack->bindForWriting();
		this->clearGL();
		world->render();

		//Reflections render to texture (front faces)
		glCullFace(GL_BACK);
		glState->setEffectType(GLUniformBlockHelper::REFLECTION);
		uniformBlockHelper->update(GLUniformBlockHelper::TYPE_EFFECT_TYPE);
		
		this->reflectionBufferFront->bindForReading(textureUnitsFront);
		this->reflectionBufferBack->bindForReading(textureUnitsBack);
		this->clearGL();
		world->render();

		//Refractions render to screen (front faces)
		glCullFace(GL_BACK);
		glState->setEffectType(GLUniformBlockHelper::REFRACTION);
		uniformBlockHelper->update(GLUniformBlockHelper::TYPE_EFFECT_TYPE);
		GLenum textureUnitsFront[2] = {GL_TEXTURE0, GL_TEXTURE1};
		GLenum textureUnitsBack[2] =  {GL_TEXTURE2, GL_TEXTURE3};
		this->reflectionBufferFront->bindForReading(textureUnitsFront);
		this->reflectionBufferBack->bindForReading(textureUnitsBack);
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