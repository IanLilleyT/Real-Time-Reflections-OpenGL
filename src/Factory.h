#include <map>
#include <string>

#include "Objects/Object.h"
#include "Objects/RenderObject.h"
#include "Objects/Jello.h"
#include "Objects/Light.h"

template <class T> void* constructor() { return (void*)new T(); }

class Factory
{
public:
	Factory()
	{
		this->register_class<Object>("Object");
		this->register_class<RenderObject>("RenderObject");
		this->register_class<Jello>("Jello");
		this->register_class<Light>("Light");
	}
	typedef void*(*constructor_t)();
	typedef std::map<std::string, constructor_t> map_type;
	map_type m_classes;

	template <class T>
	void register_class(std::string const& n)
	{ 
		m_classes[n] = &constructor<T>; 
	}

	void* construct(std::string const& n)
	{
		map_type::iterator i = m_classes.find(n);
		if (i == m_classes.end()) return 0;
		return i->second();
	}
};