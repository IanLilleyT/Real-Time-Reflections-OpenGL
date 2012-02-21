#pragma once

#include <glm/glm.hpp> //For math

#include "RenderObject.h" //Superclass
#include "../OpenGL/GLProgramDatabase.h" //For changing shaders on lines
#include "../EventHandler.h" //For keyboard events
#include "../Singleton.h" //For accessing GLProgramDatabase
#include "../Ray.h" //For setting up primitive intersections
#include "../IntersectionAlgorithms.h" //For doing primitive intersections

class Jello: public RenderObject
{
public:
	//Construct/Destruct
	Jello();
	virtual ~Jello();

	//Initialize
	virtual void initialize(TiXmlElement* element);
	void initialize(std::string type, std::string name, std::string material, std::string program,
		glm::vec3 origin, glm::vec3 size, glm::uvec3 divisions);

	//Update
	virtual void update();
	virtual void render();

protected:
	void initialize(glm::vec3 origin, glm::vec3 size, glm::uvec3 divisions);

	//Enums
	enum IntegrationType { EULER, MIDPOINT, RK4 };
	enum SpringType { STRUCTURAL, SHEAR, BEND, PENALTY }; 
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
        float restLen;
    };
    class Intersection
    {
    public:
        Intersection();
		~Intersection();

        int p;
		glm::vec3 point;
        glm::vec3 normal;
		int reflection;
        float distance;
        IntersectionType type;
    };

	//Initializers
	void initializeParticles();
	void initializeSprings();
	void initializeSpring(SpringType s,Particle& p1, Particle& p2);
	void initializeSpringConstants();
	void initializeMeshes();
	void initializeJelloMesh();
	void initializeJelloMeshIBO();
	void initializeFaceAtIndex(int& i, FaceType f, glm::uvec3 p1, glm::uvec3 p2, glm::uvec3 p3);
	void initializeJelloMeshNeighbors();
	void initializeSpringMeshes();
	void initializeNormalMesh();
	void initializeForcesMesh();
	void initializeCollisionNormalsMesh();

	//Updaters
	void checkForCollisions();
	void computeForces(std::vector<Particle>& particles);
	void resolveContacts();
	void resolveCollisions();
	void updateJelloMesh();
	void updateSpringMeshes();

	//Integration
	void EulerIntegrate();
	void MidPointIntegrate();
	void RK4Integrate();

	//Particles
	int convert3DTo1DIndex(int col, int row, int dep);
	Particle& getParticle(int col, int row, int dep);
	Particle& getParticle(int index);
	glm::vec3 getNormal(FaceType f, Particle& particle);
	std::vector<Particle> particles;
	std::map<std::pair<FaceType,int>,int> exteriorParticlesToVBOIndexMap;
	std::map<std::pair<FaceType,int>,std::vector<int>> exteriorParticlesToNeighborsMap;

	//Springs	
	std::map<SpringType,std::vector<Spring>> springMap;
	std::map<SpringType,GLMesh*> springMeshes;
	std::map<std::pair<SpringType,IntegrationType>,std::pair<float,float>> springConstants;
	glm::vec3 getSpringForce(Spring& spring, Particle& p1, Particle& p2);
	void renderSprings();

	//Intersections
	std::vector<Intersection> contacts;
	std::vector<Intersection> collisions;
	bool FloorIntersection(Particle& p, Intersection& intersection);
	bool SphereIntersection(Particle& p, Intersection& intersection, glm::mat4 T);
    bool CylinderIntersection(Particle& p, Intersection& intersection, glm::mat4 T);

	//Dimensions
	glm::vec3 origin;
	glm::vec3 size;
	int numRows;
	int numCols;
	int numDeps;
	
	//Meshes
	GLMesh* normalMesh;
	GLMesh* forcesMesh;
	GLMesh* collisionNormalsMesh;

	//Other
	static Jello::IntegrationType integrationType;
	static float integrationTimestep;
	glm::vec3 externalAcceleration;

	//External forces
	void setExternalAcceleration(glm::vec3 acceleration);
	void addExternalAcceleration(glm::vec3 acceleration);
	glm::vec3 getExternalAcceleration();

	//Events
	void keyDown(sf::Event event);
};