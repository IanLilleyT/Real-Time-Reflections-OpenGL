#include "Jello.h"

//Static variables
Jello::IntegrationType Jello::integrationType = RK4;
float Jello::integrationTimestep = 0.01f;

std::string Jello::className = "Jello";
Jello::Jello()
{
	this->type = Jello::className;
}
Jello::Jello(std::string name, std::string material, std::string program,
		glm::vec3 origin, glm::vec3 size, glm::uvec3 divisions) : RenderObject(name) 
{
	this->type = Jello::className;

	//Event Listeners
	Singleton<EventHandler>::Instance()->addInputEventListener(sf::Event::KeyPressed, InputReceiver::from_method<Jello,&Jello::keyDown>(this));
	
	//External accelerations
	this->setExternalAcceleration(glm::vec3(0,-9.8,0));

	//Spring constants
	this->springConstants[STRUCTURAL] = std::pair<float,float>(1500.0f,10.0f); //Structural
	this->springConstants[SHEAR] =      std::pair<float,float>(5000.0f,5.0f);  //Shear
	this->springConstants[BEND] =       std::pair<float,float>(1000.0f,3.0f);  //Bend

	//Set values
	this->origin = origin;
	this->size = size;
	this->numCols = divisions.x + 1;
	this->numRows = divisions.y + 1;
	this->numDeps = divisions.z + 1;

	//Initialize everything
	this->initializeParticles();
	this->initializeSprings();
	this->initializeMeshes();

	this->setMaterial(material);
	this->setProgram(program);
}
Jello::~Jello(){}

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
	this->updateJelloMesh();
	this->updateSpringMeshes();
	RenderObject::update();
}
void Jello::render()
{
	RenderObject::render();
	if(this->normalMesh->isVisible())
		this->normalMesh->Render();
	if(this->forcesMesh->isVisible())
		this->forcesMesh->Render();
	this->renderSprings();
}
void Jello::renderSprings()
{
	std::map<SpringType,GLMesh*>::iterator iter;
	for(iter = this->springMeshes.begin(); iter != this->springMeshes.end(); ++iter)
	{
		GLMesh* springMesh = iter->second;
		if(springMesh->isVisible())
			springMesh->Render();
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
				float xPos = col*colSize - size.x/2 + origin.x;
				float yPos = row*rowSize - size.y/2 + origin.y;
				float zPos = dep*depSize - size.z/2 + origin.z;
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
void Jello::initializeSprings()
{
    for (int i = 0; i < numCols; i++)
    {
        for (int j = 0; j < numRows; j++)
        {
            for (int k = 0; k < numDeps; k++)
            {
				//Structural Springs
				if (i < numCols-1) this->addSpring(STRUCTURAL, this->getParticle(i,j,k), this->getParticle(i+1,j,k));
                if (j < numRows-1) this->addSpring(STRUCTURAL, this->getParticle(i,j,k), this->getParticle(i,j+1,k));
                if (k < numDeps-1) this->addSpring(STRUCTURAL, this->getParticle(i,j,k), this->getParticle(i,j,k+1));

				//Shear Springs
				if (i < numCols-1 && k < numDeps-1) this->addSpring(SHEAR, this->getParticle(i,j,k), this->getParticle(i+1,j,k+1));
				if (j < numRows-1 && i < numCols-1) this->addSpring(SHEAR, this->getParticle(i,j,k), this->getParticle(i+1,j+1,k));
                if (j < numRows-1 && k < numDeps-1) this->addSpring(SHEAR, this->getParticle(i,j,k), this->getParticle(i,j+1,k+1));
				if (i > 0 && k < numDeps-1) this->addSpring(SHEAR, this->getParticle(i,j,k), this->getParticle(i-1,j,k+1));
				if (j > 0 && i < numCols-1) this->addSpring(SHEAR, this->getParticle(i,j,k), this->getParticle(i+1,j-1,k));
				if (j > 0 && k < numDeps-1) this->addSpring(SHEAR, this->getParticle(i,j,k), this->getParticle(i,j-1,k+1));
				
				//Bend Springs
				if(i%2 == 0 && j%2 == 0 && k%2 == 0)
				{
					if (i < numCols - 2) this->addSpring(BEND, this->getParticle(i,j,k), this->getParticle(i+2,j,k));
					if (j < numRows - 2) this->addSpring(BEND, this->getParticle(i,j,k), this->getParticle(i,j+2,k));
					if (k < numDeps - 2) this->addSpring(BEND, this->getParticle(i,j,k), this->getParticle(i,j,k+2));
				}
            }
        }
    }
}
void Jello::addSpring(SpringType type, Particle& p1, Particle& p2)
{
	float restLen = glm::length(p1.position - p2.position);
	float ks = this->springConstants[type].first;
	float kd = this->springConstants[type].second;
	this->springMap[type].push_back(Spring(type, p1.index1D, p2.index1D, ks, kd, restLen));
}
void Jello::initializeMeshes()
{
	this->initializeJelloMesh();
	this->initializeSpringMeshes();
	this->initializeNormalMesh();
	this->initializeForcesMesh();
	this->initializeCollisionNormalsMesh();
}
void Jello::initializeJelloMesh()
{
	int vboCount = 0;
	for(unsigned int i = 0; i < this->particles.size(); i++)
	{
		Particle& p = this->getParticle(i);
		int col = p.index3D.x;
		int row = p.index3D.y;
		int dep = p.index3D.z;
		int index = p.index1D;
		if(col == 0) //left
			this->exteriorParticlesToVBOIndexMap[std::pair<FaceType,int>(LEFT,index)] = vboCount++;
		if(col == numCols-1) //right
			this->exteriorParticlesToVBOIndexMap[std::pair<FaceType,int>(RIGHT,index)] = vboCount++;
		if(row == 0) //bottom
			this->exteriorParticlesToVBOIndexMap[std::pair<FaceType,int>(BOTTOM,index)] = vboCount++;
		if(row == numRows-1) //top
			this->exteriorParticlesToVBOIndexMap[std::pair<FaceType,int>(TOP,index)] = vboCount++;
		if(dep == 0) //back
			this->exteriorParticlesToVBOIndexMap[std::pair<FaceType,int>(BACK,index)] = vboCount++;
		if(dep == numDeps-1) //front
			this->exteriorParticlesToVBOIndexMap[std::pair<FaceType,int>(FRONT,index)] = vboCount++;
	}

	//Set up mesh
	this->mesh = new GLMesh();
	std::vector<GLfloat> vboData = std::vector<GLfloat>(vboCount*3*2,(GLfloat)0); // x3 for components, x2 for normals
	int numTriangles = 4*((numCols-1)*(numRows-1) + (numRows-1)*(numDeps-1) + (numCols-1)*(numDeps-1));
	std::vector<GLushort> iboData = std::vector<GLushort>(numTriangles*3,(GLushort)0); // x3 for three points in triangle
	this->mesh->setVBOData(vboData,iboData,vboCount,GL_TRIANGLES);

	this->initializeJelloMeshIBO();
	this->initializeJelloMeshNeighbors();
}
void Jello::initializeJelloMeshIBO()
{
	int iboCounter = 0;
	for(unsigned int i = 0; i < this->particles.size(); i++)
	{
		Particle& particle = this->getParticle(i);
		int col = particle.index3D.x;
		int row = particle.index3D.y;
		int dep = particle.index3D.z;

		if(dep == (numDeps-1) && col > 0 && row > 0) //front
		{
			this->initializeFaceAtIndex(iboCounter,FRONT,  
				glm::uvec3(col,row,dep),  
				glm::uvec3(col-1,row,dep),  
				glm::uvec3(col,row-1,dep));
			this->initializeFaceAtIndex(iboCounter,FRONT,  
				glm::uvec3(col,row-1,dep),  
				glm::uvec3(col-1,row,dep),
				glm::uvec3(col-1,row-1,dep));
		}
		if(dep == 0 && col > 0 && row > 0) //back
		{
			this->initializeFaceAtIndex(iboCounter,BACK,  
				glm::uvec3(col,row,dep),  
				glm::uvec3(col,row-1,dep),
				glm::uvec3(col-1,row,dep));
			this->initializeFaceAtIndex(iboCounter,BACK,  glm::uvec3(col,row-1,dep),
				glm::uvec3(col-1,row-1,dep),
				glm::uvec3(col-1,row,dep));
		}
		if(col == (numCols-1) && dep > 0 && row > 0) //right
		{
			this->initializeFaceAtIndex(iboCounter,RIGHT,  
				glm::uvec3(col,row,dep), 
				glm::uvec3(col,row-1,dep),  
				glm::uvec3(col,row,dep-1));
			this->initializeFaceAtIndex(iboCounter,RIGHT,  
				glm::uvec3(col,row-1,dep),  
				glm::uvec3(col,row-1,dep-1),  
				glm::uvec3(col,row,dep-1));
		}
		if(col == 0 && dep > 0 && row > 0) //left
		{
			this->initializeFaceAtIndex(iboCounter,LEFT,  
				glm::uvec3(col,row,dep),  
				glm::uvec3(col,row,dep-1),  
				glm::uvec3(col,row-1,dep));
			this->initializeFaceAtIndex(iboCounter,LEFT,  
				glm::uvec3(col,row-1,dep),
				glm::uvec3(col,row,dep-1),
				glm::uvec3(col,row-1,dep-1));
		}
		if(row == (numRows-1) && dep > 0 && col > 0) //top
		{
			this->initializeFaceAtIndex(iboCounter,TOP,  glm::uvec3(col,row,dep), 
				glm::uvec3(col,row,dep-1),  
				glm::uvec3(col-1,row,dep));
			this->initializeFaceAtIndex(iboCounter,TOP,  glm::uvec3(col,row,dep-1),  
				glm::uvec3(col-1,row,dep-1),  
				glm::uvec3(col-1,row,dep));
		}
		if(row == 0 && dep > 0 && col > 0) //bottom
		{
			this->initializeFaceAtIndex(iboCounter,BOTTOM,  
				glm::uvec3(col,row,dep),
				glm::uvec3(col-1,row,dep),  
				glm::uvec3(col,row,dep-1));
			this->initializeFaceAtIndex(iboCounter,BOTTOM,  
				glm::uvec3(col,row,dep-1),
				glm::uvec3(col-1,row,dep),  
				glm::uvec3(col-1,row,dep-1));
		}
	}
}
void Jello::initializeFaceAtIndex(int& i, FaceType f, glm::uvec3 p1, glm::uvec3 p2, glm::uvec3 p3)
{
	std::vector<GLushort>& iboData = this->mesh->getIBOData();
	iboData[i++] = (GLushort)this->exteriorParticlesToVBOIndexMap[std::pair<FaceType,int>(f,this->convert3DTo1DIndex(p1.x,p1.y,p1.z))];
	iboData[i++] = (GLushort)this->exteriorParticlesToVBOIndexMap[std::pair<FaceType,int>(f,this->convert3DTo1DIndex(p2.x,p2.y,p2.z))];
	iboData[i++] = (GLushort)this->exteriorParticlesToVBOIndexMap[std::pair<FaceType,int>(f,this->convert3DTo1DIndex(p3.x,p3.y,p3.z))];
}
void Jello::initializeJelloMeshNeighbors()
{
	std::map<std::pair<FaceType,int>,int>::iterator iter;
	for(iter = this->exteriorParticlesToVBOIndexMap.begin(); iter != this->exteriorParticlesToVBOIndexMap.end(); ++iter)
	{
		FaceType f = iter->first.first;
		int index = iter->first.second;
		Particle& particle = this->getParticle(index);

		int col = particle.index3D.x;
		int row = particle.index3D.y;
		int dep = particle.index3D.z;

		//Might need to fix the neighbors for corners and edges
		std::vector<int>& neighbors = this->exteriorParticlesToNeighborsMap[std::pair<FaceType,int>(f,index)];
		if(f == FRONT)
		{
			neighbors.push_back(this->convert3DTo1DIndex(std::max(col-1,0),row,dep));
			neighbors.push_back(this->convert3DTo1DIndex(col,std::max(row-1,0),dep));
			neighbors.push_back(this->convert3DTo1DIndex(std::min(col+1,numCols-1),row,dep));
			neighbors.push_back(this->convert3DTo1DIndex(col,std::min(row+1,numRows-1),dep));
		}
		else if(f == BACK)
		{
			neighbors.push_back(this->convert3DTo1DIndex(std::max(col-1,0),row,dep));
			neighbors.push_back(this->convert3DTo1DIndex(col,std::min(row+1,numRows-1),dep));
			neighbors.push_back(this->convert3DTo1DIndex(std::min(col+1,numCols-1),row,dep));
			neighbors.push_back(this->convert3DTo1DIndex(col,std::max(row-1,0),dep));
		}
		else if(f == LEFT)
		{
			neighbors.push_back(this->convert3DTo1DIndex(col,std::max(row-1,0),dep));
			neighbors.push_back(this->convert3DTo1DIndex(col,row,std::min(dep+1,numDeps-1)));
			neighbors.push_back(this->convert3DTo1DIndex(col,std::min(row+1,numRows-1),dep));
			neighbors.push_back(this->convert3DTo1DIndex(col,row,std::max(dep-1,0)));
		}
		else if(f == RIGHT)
		{
			neighbors.push_back(this->convert3DTo1DIndex(col,std::max(row-1,0),dep));
			neighbors.push_back(this->convert3DTo1DIndex(col,row,std::max(dep-1,0)));
			neighbors.push_back(this->convert3DTo1DIndex(col,std::min(row+1,numRows-1),dep));
			neighbors.push_back(this->convert3DTo1DIndex(col,row,std::min(dep+1,numDeps-1)));
		}
		else if(f == TOP)
		{
			neighbors.push_back(this->convert3DTo1DIndex(std::max(col-1,0),row,dep));
			neighbors.push_back(this->convert3DTo1DIndex(col,row,std::min(dep+1,numDeps-1)));
			neighbors.push_back(this->convert3DTo1DIndex(std::min(col+1,numCols-1),row,dep));
			neighbors.push_back(this->convert3DTo1DIndex(col,row,std::max(dep-1,0)));
		}
		else if(f == BOTTOM)
		{
			neighbors.push_back(this->convert3DTo1DIndex(std::max(col-1,0),row,dep));
			neighbors.push_back(this->convert3DTo1DIndex(col,row,std::max(dep-1,0)));
			neighbors.push_back(this->convert3DTo1DIndex(std::min(col+1,numCols-1),row,dep));
			neighbors.push_back(this->convert3DTo1DIndex(col,row,std::min(dep+1,numDeps-1)));
		}
	}
}
void Jello::initializeSpringMeshes()
{
	std::map<SpringType,std::vector<Spring>>::iterator iter;
	for(iter = this->springMap.begin(); iter != this->springMap.end(); ++iter)
	{
		SpringType type = iter->first;
		int numSprings = iter->second.size();

		GLMesh* springMesh = new GLMesh();
		std::vector<GLfloat> springVBOData = std::vector<GLfloat>(numSprings*3*2,(GLfloat)0); // x3 for components, two points per line
		std::vector<GLushort> springIBOData = std::vector<GLushort>(numSprings*3*2,(GLushort)0); // x3 for components, two points per line
		for(unsigned int i = 0; i < springIBOData.size(); i++)
			springIBOData[i] = i;
		springMesh->setVBOData(springVBOData,springIBOData,numSprings,GL_LINES);
		springMesh->setProgram(GLProgramDatabase::TYPE_WHITE);
		springMesh->setVisible(false);
		this->springMeshes[type] = springMesh;
	}
}
void Jello::initializeNormalMesh()
{
	//Set up normal mesh
	this->normalMesh = new GLMesh();
	int numElements = this->mesh->getNumElements();
	std::vector<GLfloat> normalVBOData = std::vector<GLfloat>(numElements*3*2,(GLfloat)0); // x3 for components, two points per line
	std::vector<GLushort> normalIBOData = std::vector<GLushort>(numElements*3*2,(GLushort)0); // x3 for components, two points per line
	for(unsigned int i = 0; i < normalIBOData.size(); i++)
		normalIBOData[i] = i;
	this->normalMesh->setVBOData(normalVBOData,normalIBOData,normalVBOData.size(),GL_LINES);
	this->normalMesh->setProgram(GLProgramDatabase::TYPE_WHITE);
	this->normalMesh->setVisible(false);
}
void Jello::initializeForcesMesh()
{
	//Set up forces mesh
	this->forcesMesh = new GLMesh();
	int numElements = this->mesh->getNumElements();
	std::vector<GLfloat> forceVBOData = std::vector<GLfloat>(numElements*3*2,(GLfloat)0); // x3 for components, two points per line
	std::vector<GLushort> forceIBOData = std::vector<GLushort>(numElements*3*2,(GLushort)0); // x3 for components, two points per line
	for(unsigned int i = 0; i < forceIBOData.size(); i++)
		forceIBOData[i] = i;
	this->forcesMesh->setVBOData(forceVBOData,forceIBOData,forceVBOData.size(),GL_LINES);
	this->forcesMesh->setProgram(GLProgramDatabase::TYPE_WHITE);
	this->forcesMesh->setVisible(false);
}
void Jello::initializeCollisionNormalsMesh()
{

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
void Jello::computeForces(std::vector<Particle>& particleSet)
{
	for(unsigned int i = 0; i < particleSet.size(); i++)
	{
		Particle& p = particleSet.at(i);
		p.force = p.mass*externalAcceleration;
	}
	
	// Update springs
	std::map<SpringType,std::vector<Spring>>::iterator iter;
	for(iter = this->springMap.begin(); iter != springMap.end(); ++iter)
	{
		std::vector<Spring>& springs = iter->second;
		for(unsigned int i = 0; i < springs.size(); i++)
		{
			Spring& spring = springs.at(i);

			Particle& a = particleSet.at(spring.p1);
			Particle& b = particleSet.at(spring.p2);

			glm::vec3 diff = a.position - b.position;
			float dist = glm::length(diff);
			glm::vec3 diffNormalized = diff/dist;
			float displacement = dist - spring.restLen;
			glm::vec3 diffVel = a.velocity - b.velocity;
			glm::vec3 force;

			force = -diffNormalized*(spring.Ks*displacement + spring.Kd*(diffVel*diffNormalized));
 			a.force += force;
			b.force -= force;
		}
    }
}
void Jello::resolveContacts()
{
	for (unsigned int i = 0; i < this->contacts.size(); i++)
    {
		const Intersection& contact = contacts[i];
		Particle& p = this->getParticle(contact.p);
		glm::vec3 normal = contact.normal; 
		float dist = contact.distance;

		p.position[1] = 0;
		p.velocity *= -1;  
    }
}
void Jello::resolveCollisions()
{
	for(unsigned int i = 0; i < this->collisions.size(); i++)
    {
        Intersection result = this->collisions[i];
        Particle& pt = this->getParticle(result.p);
        glm::vec3 normal = result.normal;
        float dist = result.distance;
		//Doesn't do anything yet
	}
}
void Jello::updateJelloMesh()
{
	std::vector<GLfloat>& vboData = this->mesh->getVBOData();
	std::vector<GLfloat>& normalVBOData = this->normalMesh->getVBOData();
	std::vector<GLfloat>& forcesVBOData = this->forcesMesh->getVBOData();
	//std::vector<GLfloat>& collisionNormalsVBOData = this->collisionNormalsMesh->getVBOData();

	std::map<std::pair<FaceType,int>,int>::iterator iter;
	for(iter = this->exteriorParticlesToVBOIndexMap.begin(); iter != this->exteriorParticlesToVBOIndexMap.end(); ++iter)
	{
		FaceType type = iter->first.first;
		Particle& particle = this->getParticle(iter->first.second);
		int mappedIndex = iter->second;

		//Vertex
		vboData[0 + mappedIndex*3 + 0] = (GLfloat)particle.position.x;
		vboData[0 + mappedIndex*3 + 1] = (GLfloat)particle.position.y;
		vboData[0 + mappedIndex*3 + 2] = (GLfloat)particle.position.z;

		//Normal
		glm::vec3 normal = this->getNormal(type,particle);
		vboData[vboData.size()/2 + mappedIndex*3 + 0] = (GLfloat)normal.x;
		vboData[vboData.size()/2 + mappedIndex*3 + 1] = (GLfloat)normal.y;
		vboData[vboData.size()/2 + mappedIndex*3 + 2] = (GLfloat)normal.z;

		//Normal vbo
		if(this->normalMesh->isVisible())
		{
			float normalSize = 0.2f;
			normalVBOData[mappedIndex*6 + 0] = (GLfloat)(particle.position.x);
			normalVBOData[mappedIndex*6 + 1] = (GLfloat)(particle.position.y);
			normalVBOData[mappedIndex*6 + 2] = (GLfloat)(particle.position.z);
			normalVBOData[mappedIndex*6 + 3] = (GLfloat)(particle.position.x+normal.x*normalSize);
			normalVBOData[mappedIndex*6 + 4] = (GLfloat)(particle.position.y+normal.y*normalSize);
			normalVBOData[mappedIndex*6 + 5] = (GLfloat)(particle.position.z+normal.z*normalSize);
		}

		//Forces vbo
		if(this->forcesMesh->isVisible())
		{
			float forceSize = 0.2f;
			glm::vec3 force = glm::normalize(particle.force);
			forcesVBOData[mappedIndex*6 + 0] = (GLfloat)(particle.position.x);
			forcesVBOData[mappedIndex*6 + 1] = (GLfloat)(particle.position.y);
			forcesVBOData[mappedIndex*6 + 2] = (GLfloat)(particle.position.z);
			forcesVBOData[mappedIndex*6 + 3] = (GLfloat)(particle.position.x+force.x*forceSize);
			forcesVBOData[mappedIndex*6 + 4] = (GLfloat)(particle.position.y+force.y*forceSize);
			forcesVBOData[mappedIndex*6 + 5] = (GLfloat)(particle.position.z+force.z*forceSize);
		}
	}
}
glm::vec3 Jello::getNormal(FaceType f, Particle& particle)
{
	int index = particle.index1D;
	std::vector<int>& neighbors = this->exteriorParticlesToNeighborsMap[std::pair<FaceType,int>(f,index)];
	
	glm::vec3 accum;
	int numNeighbors = neighbors.size();
	//If only two neighbors, only do one cross product
	if(numNeighbors == 2) numNeighbors--; 
	for(int i = 0; i < numNeighbors; i++)
	{
		int neighbor1Index = neighbors.at(i);
		int neighbor2Index = neighbors.at((i+1)%neighbors.size());
		glm::vec3 neighbor1Pos = this->getParticle(neighbor1Index).position;
		glm::vec3 neighbor2Pos = this->getParticle(neighbor2Index).position;
		glm::vec3 edge1 = glm::normalize(particle.position - neighbor1Pos);
		glm::vec3 edge2 = glm::normalize(particle.position - neighbor2Pos);
		glm::vec3 cross = glm::cross(edge1,edge2);
		if(glm::length(cross) > 0)
			accum += cross;
	}

	accum = glm::normalize(accum);
	return accum;
}

void Jello::updateSpringMeshes()
{
	std::map<SpringType,std::vector<Spring>>::iterator iter;
	for(iter = this->springMap.begin(); iter != this->springMap.end(); ++iter)
	{
		SpringType type = iter->first;
		GLMesh* springMesh = this->springMeshes[type];
		if(springMesh->isVisible())
		{
			std::vector<GLfloat>& springMeshVBOData = springMesh->getVBOData();
			std::vector<Spring>& springs = iter->second;
			for(unsigned int i = 0; i < springs.size(); i++)
			{
				Spring& spring = springs.at(i);
				glm::vec3 p1Pos = this->getParticle(spring.p1).position;
				glm::vec3 p2Pos = this->getParticle(spring.p2).position;
				springMeshVBOData[i*6 + 0] = p1Pos.x;
				springMeshVBOData[i*6 + 1] = p1Pos.y;
				springMeshVBOData[i*6 + 2] = p1Pos.z;
				springMeshVBOData[i*6 + 3] = p2Pos.x;
				springMeshVBOData[i*6 + 4] = p2Pos.y;
				springMeshVBOData[i*6 + 5] = p2Pos.z;
			}
		}
	}
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
	std::vector<Particle> target = this->particles;

    // Step 1
	std::vector<Particle> accum1 = this->particles;
	for(unsigned int i = 0; i < this->particles.size(); i++)
	{
		Particle& s = this->getParticle(i);
        Particle& k = accum1.at(i);
		Particle& t = target.at(i);

		k.force = this->integrationTimestep * s.force * 1.0f/s.mass;
		k.velocity = this->integrationTimestep * s.velocity;
        t.velocity = s.velocity + k.force * 0.5f;
        t.position = s.position + k.velocity * 0.5f;
	}
	this->computeForces(target);
	
    // Step 2
	std::vector<Particle> accum2 = this->particles;
    for(unsigned int i = 0; i < this->particles.size(); i++)
	{
		Particle& s = this->getParticle(i);
        Particle& k = accum2.at(i);
		Particle& t = target.at(i);

		k.force = this->integrationTimestep * t.force * 1.0f/t.mass;
		k.velocity = this->integrationTimestep * t.velocity;
        t.velocity = s.velocity + k.force * 0.5f;
        t.position = s.position + k.velocity * 0.5f;
    }
    this->computeForces(target);

    // Step 3
	std::vector<Particle> accum3 = this->particles;
	for(unsigned int i = 0; i < this->particles.size(); i++)
	{
		Particle& s = this->getParticle(i);
        Particle& k = accum3.at(i);
		Particle& t = target.at(i);

		k.force = this->integrationTimestep * t.force * 1.0f/t.mass;
		k.velocity = this->integrationTimestep * t.velocity;
        t.velocity = s.velocity + k.force;
        t.position = s.position + k.velocity;
    }
	this->computeForces(target);
	
    // Step 4
	std::vector<Particle> accum4 = this->particles;
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
		Particle& p =  this->getParticle(i);
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
  Helpers
---------------------------------------------*/
int Jello::convert3DTo1DIndex(int col, int row, int dep)
{
	return col*numRows*numDeps + row*numDeps + dep;
}
Jello::Particle& Jello::getParticle(int col, int row, int dep)
{
	return this->getParticle(this->convert3DTo1DIndex(col,row,dep));
}
Jello::Particle& Jello::getParticle(int index)
{
	return this->particles.at(index);
}
void Jello::setExternalAcceleration(glm::vec3 acceleration)
{
	this->externalAcceleration = acceleration;
}
void Jello::addExternalAcceleration(glm::vec3 acceleration)
{
	this->externalAcceleration += acceleration;
}
glm::vec3 Jello::getExternalAcceleration()
{
	return this->externalAcceleration;
}
/*---------------------------------------------
  Events
---------------------------------------------*/
void Jello::keyDown(sf::Event sfEvent)
{
	//Toggling graphics
	sf::Keyboard::Key key = sfEvent.Key.Code;
	if(key == sf::Keyboard::Num1) //Toggle mesh
		this->mesh->setVisible(!this->mesh->isVisible());
	else if(key == sf::Keyboard::Num2) //Toggle forces
		this->forcesMesh->setVisible(!this->forcesMesh->isVisible());
	else if(key == sf::Keyboard::Num3) //Toggle collision normals
	{
		//do it
	}
	else if(key == sf::Keyboard::Num4) //Toggle structural springs
	{
		GLMesh* mesh = this->springMeshes[STRUCTURAL];
		mesh->setVisible(!mesh->isVisible());
	}
	else if(key == sf::Keyboard::Num5) //Toggle shear springs
	{
		GLMesh* mesh = this->springMeshes[SHEAR];
		mesh->setVisible(!mesh->isVisible());
	}
	else if(key == sf::Keyboard::Num6) //Toggle bend springs
	{
		GLMesh* mesh = this->springMeshes[BEND];
		this->springMeshes[BEND]->setVisible(!mesh->isVisible());
	}
	else if(key == sf::Keyboard::Num7) //Toggle normals
		this->normalMesh->setVisible(!this->normalMesh->isVisible());
	else if(key == sf::Keyboard::Num8) //Set Euler
		Jello::integrationType = EULER;
	else if(key == sf::Keyboard::Num9) //Set Midpoint
		Jello::integrationType = MIDPOINT;
	else if(key == sf::Keyboard::Num0) //Set RK4
		Jello::integrationType = RK4;

	//User forces
	EventHandler* eventHandler = Singleton<EventHandler>::Instance();
	float userForceMagnitude = 0.10f;
	if(eventHandler->isKeyDown(sf::Keyboard::Left))
		this->addExternalAcceleration(glm::vec3(-userForceMagnitude,0,0));
	if(eventHandler->isKeyDown(sf::Keyboard::Right))
		this->addExternalAcceleration(glm::vec3(userForceMagnitude,0,0));
	if(eventHandler->isKeyDown(sf::Keyboard::Up))
		this->addExternalAcceleration(glm::vec3(0,0,-userForceMagnitude));
	if(eventHandler->isKeyDown(sf::Keyboard::Down))
		this->addExternalAcceleration(glm::vec3(0,0,userForceMagnitude));
}
/*---------------------------------------------
  Particle
---------------------------------------------*/
Jello::Particle::Particle()
{
	this->index1D = 0;
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