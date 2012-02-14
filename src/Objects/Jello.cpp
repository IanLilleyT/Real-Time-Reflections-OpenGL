#include "Jello.h"

//Constants
float Jello::structuralKs = 1000; 
float Jello::structuralKd = 10; 
float Jello::attachmentKs = 0;
float Jello::attachmentKd = 0;
float Jello::shearKs = 5000;
float Jello::shearKd = 5;
float Jello::bendKs = 10;
float Jello::bendKd = 1;
float Jello::penaltyKs = 0.0;
float Jello::penaltyKd = 0.0;

Jello::Jello()
{
	this->externalAcceleration = glm::vec3(0,-10,0);
	this->integrationType = RK4;
	this->integrationTimestep = 0.01f;
}
Jello::~Jello(){}

void Jello::initialize(std::string shape, glm::vec3 origin, glm::vec3 size, glm::uvec3 divisions)
{
	this->shape = shape;
	this->origin = origin;
	this->size = size;
	this->numCols = divisions.x + 1;
	this->numRows = divisions.y + 1;
	this->numDeps = divisions.z + 1;
	this->initializeParticles();
	this->initializeExternalParticles();
	this->initializeNormalMesh();
	this->initializeSprings();
}
void Jello::update()
{
	this->checkForCollisions();
	this->computeForces(this->particles);
	this->resolveContacts();
	this->resolveCollisions();
	switch (this->integrationType)
    {
		case EULER: EulerIntegrate(); break;
		case MIDPOINT: MidPointIntegrate(); break;
		case RK4: RK4Integrate(); break;
    }
	this->updateExternalParticles();
	RenderObject::update();
}
void Jello::render()
{
	RenderObject::render();
	//this->normalMesh->Render();
}

/*---------------------------------------------
  Update
---------------------------------------------*/
void Jello::checkForCollisions()
{
	contacts.clear();
    collisions.clear();

	for(unsigned int i = 0; i < this->particles.size(); i++)
	{
		Particle& p = this->getParticle(i);

        Intersection intersection;
		bool floorCollision = FloorIntersection(p, intersection);
		if(floorCollision)
		{
			if (intersection.type == CONTACT)
				contacts.push_back(intersection);
			else if(intersection.type == COLLISION)
				collisions.push_back(intersection);
		}
    }
}
bool Jello::FloorIntersection(Particle& p, Intersection& intersection)
{
	float particlePosY = p.position[1];
	float floorPosY = 0.0f;
	float epsilon = .05f;
	intersection.normal = glm::vec3(0,1,0);
	intersection.p = p.index1D;
	if(particlePosY < floorPosY)
	{
		float diffY = particlePosY - floorPosY;
		intersection.distance = diffY;
		intersection.type = CONTACT;
		return true;
	}
    return false;
}
void Jello::computeForces(std::vector<Particle>& particles)
{
	for(unsigned int i = 0; i < particles.size(); i++)
	{
		Particle& p = particles.at(i);
		p.force = p.mass*externalAcceleration;
	}
	//do spring force calculations
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
	std::map<int,int>::iterator iter;
	for(iter = this->exteriorParticlesMap.begin(); iter != this->exteriorParticlesMap.end(); ++iter)
	{
		Particle& particle = this->getParticle(iter->first);
		int mappedIndex = iter->second;

		//Vertex
		vboData[0 + mappedIndex*3 + 0] = (GLfloat)particle.position.x;
		vboData[0 + mappedIndex*3 + 1] = (GLfloat)particle.position.y;
		vboData[0 + mappedIndex*3 + 2] = (GLfloat)particle.position.z;

		//Normal
		glm::vec3 normal = this->getNormal(particle);
		vboData[vboData.size()/2 + mappedIndex*3 + 0] = (GLfloat)normal.x;
		vboData[vboData.size()/2 + mappedIndex*3 + 1] = (GLfloat)normal.y;
		vboData[vboData.size()/2 + mappedIndex*3 + 2] = (GLfloat)normal.z;

		//Normal vbo
		normalVBOData[mappedIndex*6 + 0] = (GLfloat)(particle.position.x);
		normalVBOData[mappedIndex*6 + 1] = (GLfloat)(particle.position.y);
		normalVBOData[mappedIndex*6 + 2] = (GLfloat)(particle.position.z);
		normalVBOData[mappedIndex*6 + 3] = (GLfloat)(particle.position.x+normal.x);
		normalVBOData[mappedIndex*6 + 4] = (GLfloat)(particle.position.y+normal.y);
		normalVBOData[mappedIndex*6 + 5] = (GLfloat)(particle.position.z+normal.z);
	}
}
glm::vec3 Jello::getNormal(Particle& particle)
{
	int numCols = this->numCols;
	int numRows = this->numRows;
	int numDeps = this->numDeps;

	int col = particle.index3D.x;
	int row = particle.index3D.y;
	int dep = particle.index3D.z;

	glm::vec3 suggestedNormal;
	std::vector<Particle> neighbors;

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
		glm::vec3 neighborPos = neighbors.at(i).position;
		glm::vec3 edge = glm::normalize(particle.position - neighborPos);
		accum += edge;
	}

	float accumLength = glm::length(accum);
	accum = (accumLength < .001) ? suggestedNormal : glm::normalize(accum);
	return accum;
}
/*---------------------------------------------
  Integration Methods
---------------------------------------------*/
void Jello::EulerIntegrate()
{

}
void Jello::MidPointIntegrate()
{

}
void Jello::RK4Integrate()
{
	//Create temp arrays and fill them
	std::vector<Particle> target = this->particles;
	std::vector<Particle> accum1 = this->particles;
	std::vector<Particle> accum2 = this->particles;
	std::vector<Particle> accum3 = this->particles;
	std::vector<Particle> accum4 = this->particles;

    // Step 1
	for(unsigned int i = 0; i < this->particles.size(); i++)
	{
		Particle& s = this->particles.at(i);
        Particle& k = accum1.at(i);
		Particle& t = target.at(i);

		k.force = this->integrationTimestep * s.force * 1.0f/s.mass;
		k.velocity = this->integrationTimestep * s.velocity;
        t.velocity = s.velocity + k.force * 0.5f;
        t.position = s.position + k.velocity * 0.5f;
	}

	this->computeForces(target);

    // Step 2
    for(unsigned int i = 0; i < this->particles.size(); i++)
	{
		Particle& s = this->particles.at(i);
        Particle& k = accum2.at(i);
		Particle& t = target.at(i);

		k.force = this->integrationTimestep * t.force * 1.0f/t.mass;
		k.velocity = this->integrationTimestep * t.velocity;
        t.velocity = s.velocity + k.force * 0.5f;
        t.position = s.position + k.velocity * 0.5f;
    }

    this->computeForces(target);

    // Step 3
	for(unsigned int i = 0; i < this->particles.size(); i++)
	{
		Particle& s = this->particles.at(i);
        Particle& k = accum3.at(i);
		Particle& t = target.at(i);

		k.force = this->integrationTimestep * t.force * 1.0f/t.mass;
		k.velocity = this->integrationTimestep * t.velocity;
        t.velocity = s.velocity + k.force;
        t.position = s.position + k.velocity;
    }
    
	this->computeForces(target);

    // Step 4
    for(unsigned int i = 0; i < this->particles.size(); i++)
	{
        Particle& k = accum4.at(i);
		Particle& t = target.at(i);

		k.force = this->integrationTimestep * t.force * 1.0f/t.mass;
		k.velocity = this->integrationTimestep * t.velocity;
    }

    // Put it all together
    float asixth = 1/6.0f;
    float athird = 1/3.0f;
    for(unsigned int i = 0; i < this->particles.size(); i++)
	{
		Particle& p =  this->particles.at(i);
        Particle& k1 = accum1.at(i);
        Particle& k2 = accum2.at(i);
        Particle& k3 = accum3.at(i);
        Particle& k4 = accum4.at(i);
                
        p.velocity = p.velocity + asixth * k1.force + 
            athird * k2.force + athird * k3.force + asixth * k4.force;

        p.position = p.position + asixth * k1.velocity + 
            athird * k2.velocity + athird * k3.velocity + asixth * k4.velocity;
    }
}
/*---------------------------------------------
  Initializers
---------------------------------------------*/
void Jello::initializeParticles()
{
	int numParticles = numCols * numRows * numDeps;
	this->particles = std::vector<Particle>(numParticles);

	float colSize = size.x / (numCols-1);
	float rowSize = size.y / (numRows-1);
	float depSize = size.z / (numDeps-1);

	//Create the particles
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

				Particle particle = Particle();
				particle.position = position;
				particle.index3D = glm::uvec3(col,row,dep);
				particle.index1D = this->convert3DTo1DIndex(col,row,dep);
				this->particles[particle.index1D] = particle;
			}
		}
	}
}
void Jello::initializeExternalParticles()
{
	//Create map of grid index to mesh index
	for(unsigned int i = 0; i < this->particles.size(); i++)
	{
		Particle& particle = this->particles.at(i);
		this->exteriorParticlesMap[particle.index1D] = this->exteriorParticlesMap.size();
	}

	//Set up mesh
	this->mesh = new GLMesh();
	std::vector<GLfloat> vboData = std::vector<GLfloat>(this->exteriorParticlesMap.size()*3*2,(GLfloat)0); // x3 for components. x2 for normals
	int numTriangles = 4*((numCols-1)*(numRows-1) + (numRows-1)*(numDeps-1) + (numCols-1)*(numDeps-1));
	std::vector<GLushort> iboData = std::vector<GLushort>(numTriangles*3,(GLushort)0); // x3 for three points in triangle
	this->mesh->setVBOData(vboData,iboData,(GLuint)this->exteriorParticlesMap.size(),GL_TRIANGLES);
	this->initializeExternalParticlesIBO();
}
void Jello::initializeExternalParticlesIBO()
{
	std::vector<GLushort>& iboData = this->mesh->getIBOData();
	int iboCounter = 0;
	std::map<int,int>::iterator iter;
	for(iter = this->exteriorParticlesMap.begin(); iter != this->exteriorParticlesMap.end(); ++iter)
	{
		Particle& particle = this->getParticle(iter->first);
		int col = particle.index3D.x;
		int row = particle.index3D.y;
		int dep = particle.index3D.z;

		if(dep == (numDeps-1) && col > 0 && row > 0) //front
		{
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[particle.index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep).index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep).index1D];
				
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep).index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep).index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row-1,dep).index1D];	
		}
		if(dep == 0 && col > 0 && row > 0) //back
		{
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[particle.index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep).index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep).index1D];
			
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep).index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row-1,dep).index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep).index1D];
		}
		if(col == (numCols-1) && dep > 0 && row > 0) //right
		{
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[particle.index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep).index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row,dep-1).index1D];

			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep).index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep-1).index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row,dep-1).index1D];
		}
		if(col == 0 && dep > 0 && row > 0) //left
		{
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[particle.index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row,dep-1).index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep).index1D];
				
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep).index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row,dep-1).index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row-1,dep-1).index1D];
		}
		if(row == (numRows-1) && dep > 0 && col > 0) //top
		{
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[particle.index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row,dep-1).index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep).index1D];
				
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row,dep-1).index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep-1).index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep).index1D];	
		}
		if(row == 0 && dep > 0 && col > 0) //bottom
		{
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[particle.index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep).index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row,dep-1).index1D];
				
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col,row,dep-1).index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep).index1D];
			iboData[iboCounter++] = (GLushort)this->exteriorParticlesMap[this->getParticle(col-1,row,dep-1).index1D];
		}
	}
}
void Jello::initializeNormalMesh()
{
	//Set up normal mesh
	this->normalMesh = new GLMesh();
	std::vector<GLfloat> normalVBOData = std::vector<GLfloat>(this->exteriorParticlesMap.size()*3*2,(GLfloat)0); // x3 for components, two points per line
	std::vector<GLushort> normalIBOData = std::vector<GLushort>(normalVBOData.size(),(GLushort)0);
	for(unsigned int i = 0; i < normalIBOData.size(); i++)
		normalIBOData[i] = i;
	this->normalMesh->setVBOData(normalVBOData,normalIBOData,normalVBOData.size(),GL_LINES);
	this->normalMesh->setProgramType(GLProgramDatabase::TYPE_WHITE);
}
void Jello::initializeSprings()
{
    for (int i = 0; i < numCols; i++)
    {
        for (int j = 0; j < numRows; j++)
        {
            for (int k = 0; k < numDeps; k++)
            {
				//Structural Springs
				if (i < numCols-1) this->addStructuralSpring(this->getParticle(i,j,k), this->getParticle(i+1,j,k));
                if (j < numRows-1) this->addStructuralSpring(this->getParticle(i,j,k), this->getParticle(i,j+1,k));
                if (k < numDeps-1) this->addStructuralSpring(this->getParticle(i,j,k), this->getParticle(i,j,k+1));

				//Shear Springs
				if (i < numCols-1 && k < numDeps-1) this->addShearSpring(this->getParticle(i,j,k), this->getParticle(i+1,j,k+1));
				if (j < numRows-1 && i < numCols-1) this->addShearSpring(this->getParticle(i,j,k), this->getParticle(i+1,j+1,k));
                if (j < numRows-1 && k < numDeps-1) this->addShearSpring(this->getParticle(i,j,k), this->getParticle(i,j+1,k+1));
				if (i > 0 && k < numDeps-1) this->addShearSpring(this->getParticle(i,j,k), this->getParticle(i-1,j,k+1));
				if (j > 0 && i < numCols-1) this->addShearSpring(this->getParticle(i,j,k), this->getParticle(i+1,j-1,k));
				if (j > 0 && k < numDeps-1) this->addShearSpring(this->getParticle(i,j,k), this->getParticle(i,j-1,k+1));
				
				//Bend Springs
				if(i%2 == 0 && j%2 == 0 && k%2 == 0)
				{
					if (i < numCols - 2) this->addBendSpring(this->getParticle(i,j,k), this->getParticle(i+2,j,k));
					if (j < numRows - 2) this->addBendSpring(this->getParticle(i,j,k), this->getParticle(i,j+2,k));
					if (k < numDeps - 2) this->addBendSpring(this->getParticle(i,j,k), this->getParticle(i,j,k+2));
				}
            }
        }
    }
}
/*---------------------------------------------
  Springs
---------------------------------------------*/
void Jello::addStructuralSpring(Particle& p1, Particle& p2)
{
	float restLen = glm::length(p1.position - p2.position);
	this->springs.push_back(Spring(STRUCTURAL, p1.index1D, p2.index1D, structuralKs, structuralKd, restLen));
}
void Jello::addBendSpring(Particle& p1, Particle& p2)
{
    float restLen = glm::length(p1.position - p2.position);
    this->springs.push_back(Spring(BEND, p1.index1D, p2.index1D, bendKs, bendKd, restLen));
}
void Jello::addShearSpring(Particle& p1, Particle& p2)
{
    float restLen = glm::length(p1.position - p2.position);
    this->springs.push_back(Spring(SHEAR, p1.index1D, p2.index1D, shearKs, shearKd, restLen));
}
/*---------------------------------------------
  Helpers
---------------------------------------------*/
int Jello::convert3DTo1DIndex(int col, int row, int dep)
{
	return col*numRows*numDeps + row*numDeps + dep;
}
Jello::Particle& Jello::getParticle(int col, int row, int dep)
{
	return this->particles.at(this->convert3DTo1DIndex(col,row,dep));
}
Jello::Particle& Jello::getParticle(int index)
{
	return this->particles.at(index);
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
Jello::Spring::Spring(){}
Jello::Spring::Spring(SpringType t, int p1, int p2, float Ks, float Kd, float restLen): 
	type(t), Ks(Ks), Kd(Kd), p1(p1), p2(p2), restLen(restLen) {}
Jello::Spring::~Spring(){}

/*---------------------------------------------
  Intersection
---------------------------------------------*/
Jello::Intersection::Intersection(){}
Jello::Intersection::~Intersection(){}