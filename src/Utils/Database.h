//maybe save for later

/*
#pragma once

#include <string>
#include <map>

template <class T>

std::string Database<T>::NONE = "NONE"

class Database
{
public:
	T* get(std::string name)
	{
		T* object = 0;
		object = find(name);
		if(object == 0) 
		{
			object = load(name);
			if(object != 0)
				this->map[name] = object;
		}
		return object;
	}

	static std::string NONE;

private:
	T* find(std::string name)
	{
		std::map<std::string, T*>::iterator it;
		it = this->map.find(name);
		if(it != this->map.end())
			return it->second;
		else
			return 0;
	}
	T* load(std::string name)
	{
		std::string fullPath = materialPath + materialFileName + ".xml";

		Material* material = new Material();

		//XML loading
		TiXmlDocument doc;
		doc.LoadFile(fullPath.c_str());
		if(doc.Error()) throw std::runtime_error(doc.ErrorDesc());
		TiXmlHandle docHandle(&doc);
		TiXmlElement* materialElement = docHandle.FirstChild("material").ToElement();
		material->initialize(materialElement);
	
		return material;
	}

	std::string path;
	std::map<std::string, T*> map;
};
*/