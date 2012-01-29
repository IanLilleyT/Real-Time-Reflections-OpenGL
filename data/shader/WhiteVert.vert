#version 330

layout(location = 0) in vec4 position;
uniform mat4 modelToClipMatrix;
void main()
{
    gl_Position = modelToClipMatrix*position;
}
