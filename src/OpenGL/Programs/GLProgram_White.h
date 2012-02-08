#include "../GLProgram.h"

class GLProgram_White: public GLProgram
{
public:
	GLProgram_White();
	virtual ~GLProgram_White();
	virtual void createVAO();
protected:
	virtual void bindUniformBlocks();
	virtual void fillUniforms();
};