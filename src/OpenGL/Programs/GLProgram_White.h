#include "../GLProgram.h"

class GLProgram_White: public GLProgram
{
public:
	GLProgram_White();
	virtual ~GLProgram_White();
protected:
	virtual GLVertexArrayObject* getVAO();
	virtual void fillUniforms();
};