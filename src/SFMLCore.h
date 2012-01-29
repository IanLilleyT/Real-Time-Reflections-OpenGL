#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "Singleton.h"
#include "EventHandler.h"
#include "OpenGL/GLDisplay.h"

/*---------------------------------------------
SFMLCore: Controls how often the screen redraws
---------------------------------------------*/

class SFMLCore
{
public:
    SFMLCore(void);
    virtual ~SFMLCore(void);
};

