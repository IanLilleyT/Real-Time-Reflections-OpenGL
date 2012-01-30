#include "SFMLCore.h"

SFMLCore::SFMLCore(void)
{
	//Create the window
	int width = 800;
	int height = 800;
	sf::RenderWindow* window = new sf::RenderWindow();
    window->Create(sf::VideoMode(width, height, 32), "OpenGLApplication");
    window->SetActive();
    window->EnableVerticalSync(true);
    window->SetFramerateLimit(60);

	//Handle events
	Singleton<GLDisplay>::Instance()->initialize();
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