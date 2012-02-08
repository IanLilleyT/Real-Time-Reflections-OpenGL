#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <iterator>
#include <istream>
#include <fstream>

#include <glm/glm.hpp>

class Utils
{
public:
	//Math
	static float degToRad;
	static glm::mat4 getRotationMatrixDegrees(glm::vec3 axis, float angleDegrees);
	static glm::mat4 getRotationMatrixRads(glm::vec3 axis, float angleRads);

	//Printing
	static void printMat4(glm::mat4 mat);
	static void printMat3(glm::mat3 mat);
	static void printMat2(glm::mat2 mat);
	static void printVec4(glm::vec4 vec);
	static void printVec3(glm::vec3 vec);
	static void printVec2(glm::vec2 vec);

	//Parsing and Splitting
	static std::vector<std::string> splitByCharacter(std::string s, char delim);
	static int getNumPartsInLine(std::string line);
	static float parseIntoFloat(std::string line);
	static float parseIntoFloat(std::vector<std::string>& data);
	static int parseIntoInt(std::string line);
	static int parseIntoInt(std::vector<std::string>& data);

	static glm::vec4 parseIntoVec4(std::string line);
	static glm::vec4 parseIntoVec4(std::vector<std::string>& data);
	static glm::ivec4 parseIntoIVec4(std::string line);
	static glm::ivec4 parseIntoIVec4(std::vector<std::string>& data);
	static glm::uvec4 parseIntoUVec4(std::string line);
	static glm::uvec4 parseIntoUVec4(std::vector<std::string>& data);

	static glm::vec3 parseIntoVec3(std::string line);
	static glm::vec3 parseIntoVec3(std::vector<std::string>& data);
	static glm::ivec3 parseIntoIVec3(std::string line);
	static glm::ivec3 parseIntoIVec3(std::vector<std::string>& data);
	static glm::uvec3 parseIntoUVec3(std::string line);
	static glm::uvec3 parseIntoUVec3(std::vector<std::string>& data);

	static glm::vec2 parseIntoVec2(std::string line);
	static glm::vec2 parseIntoVec2(std::vector<std::string>& data);
	static glm::ivec2 parseIntoIVec2(std::string line);
	static glm::ivec2 parseIntoIVec2(std::vector<std::string>& data);
	static glm::uvec2 parseIntoUVec2(std::string line);
	static glm::uvec2 parseIntoUVec2(std::vector<std::string>& data);
};