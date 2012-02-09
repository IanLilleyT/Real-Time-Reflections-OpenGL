#pragma once

#include <glm/glm.hpp>

#include "RenderObject.h"

class Jello: public RenderObject
{
public:
	Jello();
	virtual ~Jello();

	virtual void update();
	virtual void render();

	void initialize(std::string shape, glm::vec3 origin, glm::vec3 size, glm::uvec3 divisions); 

	std::string getShape();
	glm::vec3 getOrigin();
	glm::vec3 getSize();
	glm::uvec3 getDimensions();

	static std::string SHAPE_CUBE;
	static std::string SHAPE_SPHERE;

protected:
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
        double mass;
    };

	std::string shape;
	glm::vec3 origin;
	glm::vec3 size;
	glm::uvec3 dimensions;
	std::vector<Particle*> particles;
	std::map<Particle*,int> exteriorParticlesMap;

	void initializeExternalParticlesIBO();
	void updateExternalParticles();
	void updateSprings();

	glm::vec3 getNormal(Particle* particle);
	int convert3DTo1DIndex(int col, int row, int dep);
	Particle* getParticle(int col, int row, int dep);
};