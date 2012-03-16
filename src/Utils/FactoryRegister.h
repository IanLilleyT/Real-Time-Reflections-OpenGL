#pragma once

//Objects
#include "Objects/Object.h"
#include "Objects/RenderObject.h"
#include "Objects/Jello.h"
#include "Objects/Light.h"

//Programs
#include "OpenGL/Programs/GLProgram_Jello.h"
#include "OpenGL/Programs/GLProgram_Material.h"
#include "OpenGL/Programs/GLProgram_White.h"
#include "OpenGL/Programs/GLProgram_Reflection.h"

#include "Utils/Singleton.h" //For accessing Factory
#include "Utils/Factory.h" //For registration

class FactoryRegister
{
public:
	FactoryRegister()
	{
		Factory* factory = Singleton<Factory>::Instance();

		//Objects
		factory->register_class<Object>("Object");
		factory->register_class<RenderObject>("RenderObject");
		factory->register_class<Jello>("Jello");
		factory->register_class<Light>("Light");

		//Programs
		factory->register_class<GLProgram_Jello>("GLProgram_Jello");
		factory->register_class<GLProgram_Material>("GLProgram_Material");
		factory->register_class<GLProgram_White>("GLProgram_White");
		factory->register_class<GLProgram_Reflection>("GLProgram_Reflection");
	}
	~FactoryRegister(){}
};