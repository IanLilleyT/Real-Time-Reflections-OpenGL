#include "ExampleObject.h"

//Static vars
std::string ExampleObject::className = "ExampleObject";

ExampleObject::ExampleObject() : Object() 
{
	this->value = 0;
}
ExampleObject::~ExampleObject(){}

//Initialize
void ExampleObject::initialize(TiXmlElement* element)
{
	Object::initialize(element);

	int value;
	TiXmlElement* valueElement = element->FirstChildElement("value");
	if(valueElement) value = Utils::parseIntoInt(valueElement->FirstChild()->Value());

	this->initialize(value);
}
void ExampleObject::initialize(std::string name, int value)
{
	Object::initialize(name);
	this->initialize(value);
}
void ExampleObject::initialize(int value)
{
	this->value = value;
}

//Type
std::string ExampleObject::getType()
{
	return ExampleObject::className;
}