#include "SFMLCore.h"

SFMLCore::SFMLCore(void)
{
	//Create the window
	int width = 1024;
	int height = 768;
	sf::RenderWindow* window = new sf::RenderWindow();
    window->Create(sf::VideoMode(width, height, 32), "OpenGLApplication");
    window->SetActive();
    window->EnableVerticalSync(true);
    window->SetFramerateLimit(60);

	//Load GLDisplay
	Singleton<GLDisplay>::Instance()->initialize();
	Singleton<GLDisplay>::Instance()->resize(width,height);

	//Set GLDisplay world
	World* world = Singleton<WorldDatabase>::Instance()->loadWorld("World1");
	Singleton<GLDisplay>::Instance()->setWorld(world);
	
	//Handle events
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
        }
    }
}

SFMLCore::~SFMLCore(void){}