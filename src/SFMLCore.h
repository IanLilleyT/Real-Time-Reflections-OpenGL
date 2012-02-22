#pragma once

#include <SFML/Graphics.hpp> //For displaying the screen

#include "Singleton.h" //For accessing global classes
#include "EventHandler.h" //For sending events
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
};

