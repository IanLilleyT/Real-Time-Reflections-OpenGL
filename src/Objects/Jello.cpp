#include "Jello.h"

std::string Jello::SHAPE_CUBE = "CUBE";
std::string Jello::SHAPE_SPHERE = "SPHERE";

Jello::Jello()
{
	this->externalAcceleration = glm::vec3(0,-1,0);
}
Jello::~Jello(){}

void Jello::update()
{
	this->checkForCollisions();
	this->computeForces();
	this->resolveContacts();
	this->resolveCollisions();
	this->updateExternalParticles();
	RenderObject::update();
}
void Jello::render()
{
	RenderObject::render();
	//this->normalMesh->Render();
}
void Jello::initialize(std::string shape, glm::vec3 origin, glm::vec3 size, glm::uvec3 divisions)
{
	this->shape = shape;
	this->origin = origin;
	this->size = size;
	this->dimensions = divisions + glm::uvec3(1,1,1);

	int numCols = dimensions.x;
	int numRows = dimensions.y;
	int numDeps = dimensions.z;
	int numParticles = numCols * numRows * numDeps;
	this->particles = std::vector<Particle*>(numParticles,0);

	float colSize = size.x / (numCols-1);
	float rowSize = size.y / (numRows-1);
	float depSize = size.z / (numDeps-1);

	for(int col = 0; col < numCols; col++)
	{
		for(int row = 0; row < numRows; row++)
		{
			for(int dep = 0; dep < numDeps; dep++)
			{
				float xPos = col*colSize + origin.x;
				float yPos = row*rowSize + origin.y;
				float zPos = dep*depSize + origin.z;
				glm::vec3 position = glm::vec3(xPos,yPos,zPos);

				Jello::Particle* particle = new Particle();
				particle->position = position;
				particle->index3D = glm::uvec3(col,row,dep);
				particle->index1D = this->convert3DTo1DIndex(col,row,dep);
				this->particles[particle->index1D] = particle;

				if((col % (numCols - 1)) == 0 || (row % (numRows - 1)) == 0 || (dep % (numDeps - 1)) == 0)
					this->exteriorParticlesMap[particle] = this->exteriorParticlesMap.size();
			}
		}
	}

	//Set up mesh
	this->mesh = new GLMesh();
	std::vector<GLfloat> vboData = std::vector<GLfloat>(this->exteriorParticlesMap.size()*3*2,(GLfloat)0); // x3 for components. x2 for normals
	int numTriangles = 4*((numCols-1)*(numRows-1) + (numRows-1)*(numDeps-1) + (numCols-1)*(numDeps-1));
	std::vector<GLushort> iboData = std::vector<GLushort>(numTriangles*3,(GLushort)0); // x3 for three points in triangle
	this->mesh->setVBOData(vboData,iboData,(GLuint)this->exteriorParticlesMap.size(),GL_TRIANGLES);
	this->initializeExternalParticlesIBO();
	
	//Set up normal mesh
	this->normalMesh = new GLMesh();
	std::vector<GLfloat> normalVBOData = std::vector<GLfloat>(this->exteriorParticlesMap.size()*3*2,(GLfloat)0); // x3 for components, two points per line
	std::vector<GLushort> normalIBOData = std::vector<GLushort>(normalVBOData.size(),(GLushort)0);
	for(unsigned int i = 0; i < normalIBOData.size(); i++)
		normalIBOData[i] = i;
	this->normalMesh->setVBOData(normalVBOData,normalIBOData,normalVBOData.size(),GL_LINES);
	this->normalMesh->setProgramType(GLProgramDatabase::TYPE_WHITE);
}
void Jello::initializeExternalParticlesIBO()
{
	int numCols = this->dimensions.x;
	int numRows = this->dimensions.y;
	int numDeps = this->dimensions.z;

	std::vector<GLushort>& iboData = this->mesh->getIBOData();
	int iboCounter = 0;
	std::map<Particle*,int>::iterator iter;
	for(iter = this->exteriorParticlesMap.begin(); iter != this->exteriorParticlesMap.end(); ++iter)
	{
		Particle* particle = iter->first;
		int col = particle->index3D.x;
		int row = particle->index3D.y;
		int dep = particle->index3D.z;

		if(dep == (numDeps-1) && col > 0 && row > 0) //front
		{
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[particle];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep)];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep)];
				
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep)];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep)];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row-1,dep)];	
		}
		if(dep == 0 && col > 0 && row > 0) //back
		{
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[particle];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep)];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep)];
			
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep)];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row-1,dep)];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep)];
		}
		if(col == (numCols-1) && dep > 0 && row > 0) //right
		{
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[particle];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep)];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row,dep-1)];

			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep)];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep-1)];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row,dep-1)];
		}
		if(col == 0 && dep > 0 && row > 0) //left
		{
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[particle];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row,dep-1)];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep)];
				
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep)];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row,dep-1)];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep-1)];
		}
		if(row == (numRows-1) && dep > 0 && col > 0) //top
		{
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[particle];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row,dep-1)];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep)];
				
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row,dep-1)];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep-1)];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep)];	
		}
		if(row == 0 && dep > 0 && col > 0) //bottom
		{
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[particle];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep)];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row,dep-1)];
				
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row,dep-1)];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep)];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep-1)];
		}
	}
}
void Jello::checkForCollisions()
{
	for(int i = 0; i < this->particles.size(); i++)
	{
		Particle* p = this->particles.at(i);
		p->force = p->mass*this->externalAcceleration;
	}
}
void Jello::computeForces()
{

}
void Jello::resolveContacts()
{

}
void Jello::resolveCollisions()
{

}
void Jello::updateExternalParticles()
{
	std::vector<GLfloat>& vboData = this->mesh->getVBOData();
	std::vector<GLfloat>& normalVBOData = this->normalMesh->getVBOData();
	std::map<Particle*,int>::iterator iter;
	for(iter = this->exteriorParticlesMap.begin(); iter != this->exteriorParticlesMap.end(); ++iter)
	{
		Particle* particle = iter->first;
		int mappedIndex = iter->second;

		//Vertex
		vboData[0 + mappedIndex*3 + 0] = (GLfloat)particle->position.x;
		vboData[0 + mappedIndex*3 + 1] = (GLfloat)particle->position.y;
		vboData[0 + mappedIndex*3 + 2] = (GLfloat)particle->position.z;

		//Normal
		glm::vec3 normal = this->getNormal(particle);
		vboData[vboData.size()/2 + mappedIndex*3 + 0] = (GLfloat)normal.x;
		vboData[vboData.size()/2 + mappedIndex*3 + 1] = (GLfloat)normal.y;
		vboData[vboData.size()/2 + mappedIndex*3 + 2] = (GLfloat)normal.z;

		//Normal vbo
		normalVBOData[mappedIndex*6 + 0] = (GLfloat)(particle->position.x);
		normalVBOData[mappedIndex*6 + 1] = (GLfloat)(particle->position.y);
		normalVBOData[mappedIndex*6 + 2] = (GLfloat)(particle->position.z);
		normalVBOData[mappedIndex*6 + 3] = (GLfloat)(particle->position.x+normal.x);
		normalVBOData[mappedIndex*6 + 4] = (GLfloat)(particle->position.y+normal.y);
		normalVBOData[mappedIndex*6 + 5] = (GLfloat)(particle->position.z+normal.z);
	}
}
glm::vec3 Jello::getNormal(Particle* particle)
{
	int numCols = this->dimensions.x;
	int numRows = this->dimensions.y;
	int numDeps = this->dimensions.z;

	int col = particle->index3D.x;
	int row = particle->index3D.y;
	int dep = particle->index3D.z;

	glm::vec3 suggestedNormal;
	std::vector<Particle*> neighbors;

	if((col % (numCols-1)) == 0  && (row % (numRows-1)) == 0 && (dep % (numDeps-1)) == 0) //corners
	{
		neighbors.push_back(this->getParticle(col+(col == 0 ? 1:-1),row,dep));
		neighbors.push_back(this->getParticle(col,row+(row == 0 ? 1:-1),dep));
		neighbors.push_back(this->getParticle(col,row,dep+(dep == 0 ? 1:-1)));
	}
	else if((col % (numCols-1)) == 0 && (dep % (numDeps-1)) == 0) //col edges
	{
		neighbors.push_back(this->getParticle(col+(col == 0 ? 1:-1),row,dep));
		neighbors.push_back(this->getParticle(col,row,dep+(dep == 0 ? 1:-1)));
		neighbors.push_back(this->getParticle(col,row+1,dep));
		neighbors.push_back(this->getParticle(col,row-1,dep));
	}
	else if((row % (numRows-1)) == 0 && (dep % (numDeps-1)) == 0) //row edges
	{
		neighbors.push_back(this->getParticle(col,row+(row == 0 ? 1:-1),dep));
		neighbors.push_back(this->getParticle(col,row,dep+(dep == 0 ? 1:-1)));
		neighbors.push_back(this->getParticle(col+1,row,dep));
		neighbors.push_back(this->getParticle(col-1,row,dep));
	}
	else if((col % (numCols-1)) == 0 && (row % (numRows-1)) == 0) //dep edges
	{
		neighbors.push_back(this->getParticle(col+(col == 0 ? 1:-1),row,dep));
		neighbors.push_back(this->getParticle(col,row+(row == 0 ? 1:-1),dep));
		neighbors.push_back(this->getParticle(col,row,dep+1));
		neighbors.push_back(this->getParticle(col,row,dep-1));
	}
	else if((col % (numCols-1)) == 0) //left and right face
	{
		neighbors.push_back(this->getParticle(col,row-1,dep));
		neighbors.push_back(this->getParticle(col,row+1,dep));
		neighbors.push_back(this->getParticle(col,row,dep-1));
		neighbors.push_back(this->getParticle(col,row,dep+1));
		suggestedNormal = col == 0 ? glm::vec3(-1,0,0) : glm::vec3(1,0,0);
	}
	else if((row % (numRows-1)) == 0) //top and bottom face
	{
		neighbors.push_back(this->getParticle(col-1,row,dep));
		neighbors.push_back(this->getParticle(col+1,row,dep));
		neighbors.push_back(this->getParticle(col,row,dep-1));
		neighbors.push_back(this->getParticle(col,row,dep+1));
		suggestedNormal = row == 0 ? glm::vec3(0,-1,0) : glm::vec3(0,1,0);
	}
	else if((dep % (numDeps-1)) == 0) //front and back face
	{
		neighbors.push_back(this->getParticle(col-1,row,dep));
		neighbors.push_back(this->getParticle(col+1,row,dep));
		neighbors.push_back(this->getParticle(col,row-1,dep));
		neighbors.push_back(this->getParticle(col,row+1,dep));
		suggestedNormal = dep == 0 ? glm::vec3(0,0,-1) : glm::vec3(0,0,1);
	}

	glm::vec3 accum;
	for(unsigned int i = 0; i < neighbors.size(); i++)
	{
		glm::vec3 neighborPos = neighbors.at(i)->position;
		glm::vec3 edge = glm::normalize(particle->position - neighborPos);
		accum += edge;
	}

	float accumLength = glm::length(accum);
	accum = (accumLength < .001) ? suggestedNormal : glm::normalize(accum);
	return accum;
}
int Jello::convert3DTo1DIndex(int col, int row, int dep)
{
	return col*dimensions.y*dimensions.z + row*dimensions.z + dep;
}
Jello::Particle* Jello::getParticle(int col, int row, int dep)
{
	return this->particles.at(this->convert3DTo1DIndex(col,row,dep));
}

/*---------------------------------------------
  Getters
---------------------------------------------*/
std::string Jello::getShape()
{
	return this->shape;
}
glm::vec3 Jello::getOrigin()
{
	return this->origin;
}
glm::vec3 Jello::getSize()
{
	return this->size;
}
glm::uvec3 Jello::getDimensions()
{
	return this->dimensions;
}

/*---------------------------------------------
  Particle
---------------------------------------------*/
Jello::Particle::Particle()
{
	this->index1D = -1;
	this->mass = 1;
}
Jello::Particle::~Particle(){}

/*---------------------------------------------
  Spring
---------------------------------------------*/
Jello::Spring::Spring()
{
	this->p1 = 0;
	this->p2 = 0;
}
Jello::Spring::~Spring(){}

/*---------------------------------------------
  Intersection
---------------------------------------------*/
Jello::Intersection::Intersection()
{
	this->particle = 0;
}
Jello::Intersection::~Intersection(){}