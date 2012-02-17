#include "../GLProgram.h"

class GLProgram_Jello: public GLProgram
{
public:
	GLProgram_Jello();
	virtual ~GLProgram_Jello();
	virtual void createVAO();
protected:
	virtual void bindUniformBlocks();
	virtual void fillUniforms();
};