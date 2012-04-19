#include "SFMLCore.h"

SFMLCore::SFMLCore(void)
{
	//Create the window
	int width = 1024;
	int height = 768;
	window = new sf::RenderWindow();
	title = "OpenGLApplication";
    window->Create(sf::VideoMode(width, height, 32), title);
    window->SetActive();
   // window->EnableVerticalSync(true);
   // window->SetFramerateLimit(60);

	//Load GLDisplay
	GLDisplay* glDisplay = Singleton<GLDisplay>::Instance();
	glDisplay->initialize();
	glDisplay->resize(width,height);

	//Set GLDisplay world
	World* world = Singleton<WorldDatabase>::Instance()->loadWorld("World1");
	glDisplay->setWorld(world);
	glDisplay->initializePhysics();

	//Handle events
	frameCount = 0;
    bool windowClosed = false;
    while (window->IsOpen())
    {
        sf::Event myEvent;
		while (window->PollEvent(myEvent))
        {
			Singleton<EventHandler>::Instance()->processEvent(myEvent);
            if (myEvent.Type == sf::Event::Closed)
            {
                window->Close();
                windowClosed = true;
            }
        }
        if(!windowClosed)
        {
			Singleton<EventHandler>::Instance()->enterFrame();
            window->Display();
			this->showFPS();
        }
    }
}

SFMLCore::~SFMLCore(void){}

void SFMLCore::showFPS()
{
	//Show fps
	frameCount++;
	float currentTime = clock.GetElapsedTime().AsSeconds();
	
	if(currentTime >= 1.0f)
	{	
		std::ostringstream ss;
		ss << title << " (fps: " << (frameCount/currentTime) << " )";
		window->SetTitle(ss.str());

		clock.Restart();
		frameCount = 0;
	}
}