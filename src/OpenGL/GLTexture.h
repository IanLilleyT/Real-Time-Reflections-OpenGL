#include <gl3w/gl3w.h> //For GL commands

class GLTexture
{
public:
	GLTexture();
	~GLTexture();
private:
	GLuint textureObject;
	GLenum textureUnit;

}