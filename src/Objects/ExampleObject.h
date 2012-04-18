#pragma once

#include "Object.h" //Superclass

class ExampleObject: public Object
{
public:
	//Class
	static std::string className;

	//Construct/Destruct
	ExampleObject();
	virtual ~ExampleObject();

	//Initialize
	virtual void initialize(TiXmlElement* element);
	void initialize(std::string name, int value);
	
	//Type
	virtual std::string getType();

protected:
	void initialize(int value);

	int value;
};