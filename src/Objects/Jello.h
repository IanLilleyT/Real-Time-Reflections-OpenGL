#pragma once

#include <glm/glm.hpp>

#include "../OpenGL/GlProgramDatabase.h"
#include "RenderObject.h"

class Jello: public RenderObject
{
public:
	Jello();
	virtual ~Jello();

	void initialize(std::string shape, glm::vec3 origin, glm::vec3 size, glm::uvec3 divisions); 
	virtual void update();
	virtual void render();

protected:

	enum IntegrationType { EULER, MIDPOINT, RK4 };
	enum SpringType { STRUCTURAL, SHEAR, BEND }; 
	enum IntersectionType { CONTACT, COLLISION };

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

	//Variables
	GLMesh* normalMesh;
	std::string shape;
	glm::vec3 origin;
	glm::vec3 size;
	int numRows;
	int numCols;
	int numDeps;
	std::vector<Particle> particles;
	std::map<int,int> exteriorParticlesMap;

	Jello::IntegrationType integrationType;
	float integrationTimestep;
	glm::vec3 externalAcceleration;

	//Integration
	void EulerIntegrate();
	void MidPointIntegrate();
	void RK4Integrate();

	//Updaters
	void checkForCollisions();
	void computeForces(std::vector<Particle>& particles);
	void resolveContacts();
	void resolveCollisions();
	void updateExternalParticles();

	//Initializers
	void initializeParticles();
	void initializeExternalParticles();
	void initializeExternalParticlesIBO();
	void initializeNormalMesh();
	void initializeSprings();

	//Springs
	std::vector<Spring> springs;
	void addStructuralSpring(Particle& p1, Particle& p2);
	void addBendSpring(Particle& p1, Particle& p2);
	void addShearSpring(Particle& p1, Particle& p2);

	//Intersections
	std::vector<Intersection> contacts;
	std::vector<Intersection> collisions;
	bool FloorIntersection(Particle& p, Intersection& intersection);
    //virtual bool CylinderIntersection(Particle& p, World::Cylinder* cylinder, Intersection& intersection);

	//Spring constants
	static float structuralKs; 
	static float structuralKd; 
	static float attachmentKs;
	static float attachmentKd;
	static float shearKs;
	static float shearKd;
	static float bendKs;
	static float bendKd;
	static float penaltyKs;
	static float penaltyKd;

	//Helpers
	glm::vec3 getNormal(Particle& particle);
	int convert3DTo1DIndex(int col, int row, int dep);
	Particle& getParticle(int col, int row, int dep);
	Particle& getParticle(int index);
};