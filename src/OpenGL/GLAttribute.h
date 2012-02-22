#pragma once

#include <gl3w/gl3w.h> //For OpenGL commands

class GLAttribute
{
public:
    GLAttribute();
    ~GLAttribute();

    //Setters
    void setAttributePos(GLuint attributePos);
    void setSize(GLushort size);
    void setType(GLenum type);

    //Getters
    GLuint getAttributePos();
    GLushort getSize();
    GLenum getType();

private:
    GLuint attributePos;
    GLushort size;
    GLenum type;
};
