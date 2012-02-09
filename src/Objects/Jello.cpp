#include "Jello.h"

std::string Jello::SHAPE_CUBE = "CUBE";
std::string Jello::SHAPE_SPHERE = "SPHERE";

Jello::Jello(){}
Jello::~Jello(){}

void Jello::update()
{
	updateSprings();
	RenderObject::update();
}
void Jello::render()
{
	//do other stuff first
	updateExternalParticles();
	RenderObject::render();
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

	float colSize = size.x / numCols;
	float rowSize = size.y / numRows;
	float depSize = size.z / numDeps;

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
	std::vector<GLfloat> vboData = std::vector<GLfloat>(this->exteriorParticlesMap.size()*3*2,0); // x3 for components. x2 for normals
	int numTriangles = 4*((numCols-1)*(numRows-1) + (numRows-1)*(numDeps-1) + (numCols-1)*(numDeps-1));
	std::vector<GLushort> iboData = std::vector<GLushort>(numTriangles*3,0);
	this->mesh->setVBOData(vboData,iboData,GL_TRIANGLES);
	this->initializeExternalParticlesIBO();
}
void Jello::initializeExternalParticlesIBO()
{
	int numCols = this->dimensions.x;
	int numRows = this->dimensions.y;
	int numDeps = this->dimensions.z;

	std::vector<GLushort>& iboData = this->mesh->getIBOData();
	std::map<Particle*,int>::iterator iter;
	for(iter = this->exteriorParticlesMap.begin(); iter != this->exteriorParticlesMap.end(); ++iter)
	{
		Particle* particle = iter->first;
		int col = particle->index3D.x;
		int row = particle->index3D.y;
		int dep = particle->index3D.z;

		if(dep == (numDeps-1) && col > 0 && row > 0) //front
		{
			iboData[iboData.size()] = this->exteriorParticlesMap[particle];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col-1,row,dep)];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col,row-1,dep)];
				
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col,row-1,dep)];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col-1,row,dep)];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col-1,row-1,dep)];	
		}
		if(dep == 0 && col > 0 && row > 0) //back
		{
			iboData[iboData.size()] = this->exteriorParticlesMap[particle];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col,row-1,dep)];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col-1,row,dep)];
			
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col,row-1,dep)];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col-1,row-1,dep)];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col-1,row,dep)];
		}
		if(col == (numCols-1) && dep > 0 && row > 0) //right
		{
			iboData[iboData.size()] = this->exteriorParticlesMap[particle];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col,row-1,dep)];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col,row,dep-1)];

			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col,row-1,dep)];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col,row-1,dep-1)];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col,row,dep-1)];
		}
		if(col == 0 && dep > 0 && row > 0) //left
		{
			iboData[iboData.size()] = this->exteriorParticlesMap[particle];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col,row,dep-1)];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col,row-1,dep)];
				
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col,row-1,dep)];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col,row,dep-1)];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col,row-1,dep-1)];
		}
		if(row == (numRows-1) && dep > 0 && col > 0) //top
		{
			iboData[iboData.size()] = this->exteriorParticlesMap[particle];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col,row,dep-1)];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col-1,row,dep)];
				
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col,row,dep-1)];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col-1,row,dep-1)];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col-1,row,dep)];	
		}
		if(row == 0 && dep > 0 && col > 0) //bottom
		{
			iboData[iboData.size()] = this->exteriorParticlesMap[particle];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col-1,row,dep)];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col,row,dep-1)];
				
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col,row,dep-1)];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col-1,row,dep)];
			iboData[iboData.size()] = this->exteriorParticlesMap[this->getParticle(col-1,row,dep-1)];
		}
	}
}
void Jello::updateSprings()
{

}
void Jello::updateExternalParticles()
{
	std::vector<GLfloat>& vboData = this->mesh->getVBOData();
	std::map<Particle*,int>::iterator iter;
	for(iter = this->exteriorParticlesMap.begin(); iter != this->exteriorParticlesMap.end(); ++iter)
	{
		Particle* particle = iter->first;
		int mappedIndex = iter->second;

		//Vertex
		vboData[mappedIndex*3 + 0] = particle->position.x;
		vboData[mappedIndex*3 + 1] = particle->position.y;
		vboData[mappedIndex*3 + 2] = particle->position.z;

		//Normal
		glm::vec3 normal = this->getNormal(particle);
		vboData[vboData.size()/2 + mappedIndex*3 + 0] = normal.x;
		vboData[vboData.size()/2 + mappedIndex*3 + 1] = normal.y;
		vboData[vboData.size()/2 + mappedIndex*3 + 2] = normal.z;
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

	glm::vec3 accum;
	for(int i = 0; i < neighbors.size(); i++)
	{
		glm::vec3 neighborPos = neighbors.at(i)->position;
		glm::vec3 edge = glm::normalize(particle->position - neighborPos);
		accum += edge;
	}
	accum = glm::normalize(accum);
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