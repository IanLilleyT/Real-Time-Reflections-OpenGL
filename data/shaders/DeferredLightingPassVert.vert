#version 330
layout(std140) uniform;

//Input
layout(location = 0) in vec3 position;

//Main
void main()
{
	gl_Position = vec4(position,1.0);
}
