#pragma once

#include <assert.h> //Ensures that only one instance is present

template <class T>
class Singleton
{
public:
    static T* Instance()
    {
        if(!m_pInstance) m_pInstance = new T;
        assert(m_pInstance !=0);
        return m_pInstance;
    }
protected:
    Singleton();
    ~Singleton();
private:
    Singleton(Singleton const&);
    Singleton& operator=(Singleton const&);
    static T* m_pInstance;
};

template <class T> T* Singleton<T>::m_pInstance=0;
