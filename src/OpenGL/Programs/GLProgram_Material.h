#include "../GLProgram.h"

class GLProgram_Material: public GLProgram
{
public:
	GLProgram_Material();
	virtual ~GLProgram_Material();
protected:
	virtual GLVertexArrayObject* getVAO();
	virtual void bindUniformBlocks();
	virtual void fillUniforms();
};