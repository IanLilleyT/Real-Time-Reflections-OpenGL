#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "Singleton.h"
#include "EventHandler.h"
#include "OpenGL/GLDisplay.h"

/*---------------------------------------------
  SFMLCore: Controls the RenderWindow, SFML event
  flow, window actions, etc
---------------------------------------------*/

class SFMLCore
{
public:
    SFMLCore(void);
    virtual ~SFMLCore(void);
};

