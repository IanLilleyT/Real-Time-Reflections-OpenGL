#pragma once

#include <glm/glm.hpp>

#include "../OpenGL/GlProgramDatabase.h"
#include "RenderObject.h"
#include "../EventHandler.h"

class Jello: public RenderObject
{
public:
	Jello();
	Jello(std::string name, std::string material, std::string program,
		glm::vec3 origin, glm::vec3 size, glm::uvec3 divisions); 
	virtual ~Jello();

	virtual void update();
	virtual void render();

	void setExternalAcceleration(glm::vec3 acceleration);
	void addExternalAcceleration(glm::vec3 acceleration);
	glm::vec3 getExternalAcceleration();


protected:
	static std::string className;

	//Enums
	enum IntegrationType { EULER, MIDPOINT, RK4 };
	enum SpringType { STRUCTURAL, SHEAR, BEND }; 
	enum IntersectionType { CONTACT, COLLISION };
	enum FaceType { LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK };

	//Helper Classes
	class Particle
    {
    public:
        Particle();
		~Particle();
        
        int index1D;
		glm::uvec3 index3D;
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 force;
        float mass;
    };
	class Spring
    {
    public:
        Spring();
		Spring(SpringType t, int p1, int p2, float Ks, float Kd, float restLen);
		~Spring();

        SpringType type;
        int p1;
        int p2;
        float Ks;
        float Kd;
        float restLen;
    };
    class Intersection
    {
    public:
        Intersection();
		~Intersection();

        int p;
        glm::vec3 normal;
        float distance;
        IntersectionType type;
    };

	//Initializers
	void initializeParticles();
	void initializeJelloMesh();
	void initializeJelloMeshIBO();
	void addFaceAtIndex(int& i, FaceType f, int c1, int r1, int d1, 
		                int c2, int r2, int d2, int c3, int r3, int d3);
	void initializeJelloMeshNeighbors();
	void initializeNormalMesh();
	void initializeSprings();

	//Particles
	void updateJelloMesh();
	int convert3DTo1DIndex(int col, int row, int dep);
	Particle& getParticle(int col, int row, int dep);
	Particle& getParticle(int index);
	glm::vec3 getNormal(FaceType f, Particle& particle);
	std::vector<Particle> particles;
	std::map<std::pair<FaceType,int>,int> exteriorParticlesToVBOIndexMap;
	std::map<std::pair<FaceType,int>,std::vector<int>> exteriorParticlesToNeighborsMap;

	//Normals
	GLMesh* normalMesh;

	//Integration
	void EulerIntegrate();
	void MidPointIntegrate();
	void RK4Integrate();

	//Springs
	void computeForces(std::vector<Particle>& particles);
	void updateSpringMeshes();
	std::map<SpringType,std::vector<Spring>> springMap;
	std::map<SpringType,GLMesh*> springMeshes;
	std::map<SpringType,std::pair<float,float>> springConstants;
	void addSpring(SpringType s,Particle& p1, Particle& p2);
	void renderSprings();

	//Intersections
	std::vector<Intersection> contacts;
	std::vector<Intersection> collisions;
	void checkForCollisions();
	void resolveContacts();
	void resolveCollisions();
	bool FloorIntersection(Particle& p, Intersection& intersection);
    //virtual bool CylinderIntersection(Particle& p, World::Cylinder* cylinder, Intersection& intersection);

	//Dimensions
	glm::vec3 origin;
	glm::vec3 size;
	int numRows;
	int numCols;
	int numDeps;
	
	//Tweak
	static Jello::IntegrationType integrationType;
	static float integrationTimestep;
	glm::vec3 externalAcceleration;

	//Events
	void keyDown(sf::Event event);
};