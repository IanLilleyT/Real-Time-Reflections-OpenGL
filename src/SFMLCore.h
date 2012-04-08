#pragma once

#include <string> //for window title

#include <SFML/Graphics.hpp> //For displaying the screen

#include "Utils/Utils.h" //For conversions
#include "Utils/Singleton.h" //For accessing global classes
#include "Utils/EventHandler.h" //For sending events
#include "OpenGL/GLDisplay.h" //For initializing GLDisplay
#include "WorldDatabase.h" //For loading the initial world
#include "World.h" //For loading the initial world

/*---------------------------------------------
  SFMLCore: Controls the RenderWindow, SFML event
  flow, and display
---------------------------------------------*/

class SFMLCore
{
public:
    SFMLCore(void);
    virtual ~SFMLCore(void);

private:
	sf::Window* window;
	std::string title;

	void showFPS();
	sf::Clock clock;
	int frameCount;
};

