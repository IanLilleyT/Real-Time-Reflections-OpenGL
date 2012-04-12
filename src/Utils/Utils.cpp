#include "Utils.h"

float Utils::degToRad = 3.14159f / 180.0f;

glm::mat4 Utils::getRotationMatrixDegrees(glm::vec3 axis, float angleDegrees)
{
	return Utils::getRotationMatrixRads(axis, angleDegrees * Utils::degToRad);
}
glm::mat4 Utils::getRotationMatrixRads(glm::vec3 axis, float angleRads)
{
	float fCos = cosf(angleRads);
	float fInvCos = 1.0f - fCos;
	float fSin = sinf(angleRads);

	glm::vec3 axisNorm = glm::normalize(axis);

	glm::mat4 theMat(1.0f);
	theMat[0].x = (axisNorm.x * axisNorm.x) + ((1 - axisNorm.x * axisNorm.x) * fCos);
	theMat[1].x = axisNorm.x * axisNorm.y * (fInvCos) - (axisNorm.z * fSin);
	theMat[2].x = axisNorm.x * axisNorm.z * (fInvCos) + (axisNorm.y * fSin);

	theMat[0].y = axisNorm.x * axisNorm.y * (fInvCos) + (axisNorm.z * fSin);
	theMat[1].y = (axisNorm.y * axisNorm.y) + ((1 - axisNorm.y * axisNorm.y) * fCos);
	theMat[2].y = axisNorm.y * axisNorm.z * (fInvCos) - (axisNorm.x * fSin);

	theMat[0].z = axisNorm.x * axisNorm.z * (fInvCos) - (axisNorm.y * fSin);
	theMat[1].z = axisNorm.y * axisNorm.z * (fInvCos) + (axisNorm.x * fSin);
	theMat[2].z = (axisNorm.z * axisNorm.z) + ((1 - axisNorm.z * axisNorm.z) * fCos);

	return theMat;
}
float Utils::getRandom(float min, float max)
{
	float initial = ((float)rand())/((float)RAND_MAX);
	float final = max - initial * (max - min);
	return final;
}

void Utils::printMat4(glm::mat4 mat)
{
    std::cout << 
	mat[0].x << " " << mat[1].x << " " << mat[2].x << " " << mat[3].x << "\n" <<
    mat[0].y << " " << mat[1].y << " " << mat[2].y << " " << mat[3].y << "\n" <<
    mat[0].z << " " << mat[1].z << " " << mat[2].z << " " << mat[3].z << "\n" <<
    mat[0].w << " " << mat[1].w << " " << mat[2].w << " " << mat[3].w << "\n" <<
	std::endl;
}
void Utils::printMat3(glm::mat3 mat)
{
    std::cout << 
	mat[0].x << " " << mat[1].x << " " << mat[2].x << "\n" <<
    mat[0].y << " " << mat[1].y << " " << mat[2].y << "\n" <<
    mat[0].z << " " << mat[1].z << " " << mat[2].z << "\n" <<
	std::endl;
}
void Utils::printMat2(glm::mat2 mat)
{
    std::cout << 
	mat[0].x << " " << mat[1].x << "\n" <<
    mat[0].y << " " << mat[1].y << "\n" <<
	std::endl;
}

void Utils::printVec4(glm::vec4 vec)
{
    std::cout << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << std::endl;
}
void Utils::printVec3(glm::vec3 vec)
{
    std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
}
void Utils::printVec2(glm::vec2 vec)
{
	std::cout << vec.x << " " << vec.y << std::endl;
}

//Conversions
std::string Utils::convertIntToString(int val)
{
	std::ostringstream ss;
	ss << val;
	return ss.str();
}
std::string Utils::convertFloatToString(float val)
{
	std::ostringstream ss;
	ss << val;
	return ss.str();
}

//Tokenizing
std::vector<std::string> Utils::splitByCharacter(std::string s, char delim)
{
	std::vector<std::string> elems = std::vector<std::string>();
	std::stringstream ss(s);
	std::string item;
	while(std::getline(ss, item, delim))
		elems.push_back(item);
	return elems;
}
float Utils::parseIntoFloat(std::string line)
{
	std::vector<std::string> results = Utils::splitByCharacter(line, ' ');
	return Utils::parseIntoFloat(results);
}
float Utils::parseIntoFloat(std::vector<std::string>& data)
{
	int numParts = data.size();
	float part1 = (float)atof(data.at(numParts-1).c_str());
	return part1;
}
int Utils::parseIntoInt(std::string line)
{
	return (int)Utils::parseIntoFloat(line);
}
int Utils::parseIntoInt(std::vector<std::string>& data)
{
	return (int)Utils::parseIntoFloat(data);
}

//Vec4
glm::vec4 Utils::parseIntoVec4(std::string line)
{
	std::vector<std::string> results = Utils::splitByCharacter(line, ' ');
	return Utils::parseIntoVec4(results);
}
glm::vec4 Utils::parseIntoVec4(std::vector<std::string>& data)
{
	int numParts = data.size();
	float part1 = (float)atof(data.at(numParts-4).c_str());
	float part2 = (float)atof(data.at(numParts-3).c_str());
	float part3 = (float)atof(data.at(numParts-2).c_str());
	float part4 = (float)atof(data.at(numParts-1).c_str());
	glm::vec4 parts = glm::vec4(part1,part2,part3,part4);
	return parts;
}
glm::ivec4 Utils::parseIntoIVec4(std::string line)
{
	return glm::ivec4(Utils::parseIntoVec4(line));
}
glm::ivec4 Utils::parseIntoIVec4(std::vector<std::string>& data)
{
	return glm::ivec4(Utils::parseIntoVec4(data));
}
glm::uvec4 Utils::parseIntoUVec4(std::string line)
{
	return glm::uvec4(Utils::parseIntoVec4(line));
}
glm::uvec4 Utils::parseIntoUVec4(std::vector<std::string>& data)
{
	return glm::uvec4(Utils::parseIntoVec4(data));
}

//Vec3
glm::vec3 Utils::parseIntoVec3(std::string line)
{
	std::vector<std::string> results = Utils::splitByCharacter(line, ' ');
	return Utils::parseIntoVec3(results);
}
glm::vec3 Utils::parseIntoVec3(std::vector<std::string>& data)
{
	int numParts = data.size();
	float part1 = (float)atof(data.at(numParts-3).c_str());
	float part2 = (float)atof(data.at(numParts-2).c_str());
	float part3 = (float)atof(data.at(numParts-1).c_str());
	glm::vec3 parts = glm::vec3(part1,part2,part3);
	return parts;
}
glm::ivec3 Utils::parseIntoIVec3(std::string line)
{
	return glm::ivec3(Utils::parseIntoVec3(line));
}
glm::ivec3 Utils::parseIntoIVec3(std::vector<std::string>& data)
{
	return glm::ivec3(Utils::parseIntoVec3(data));
}
glm::uvec3 Utils::parseIntoUVec3(std::string line)
{
	return glm::uvec3(Utils::parseIntoVec3(line));
}
glm::uvec3 Utils::parseIntoUVec3(std::vector<std::string>& data)
{
	return glm::uvec3(Utils::parseIntoVec3(data));
}

//Vec2
glm::vec2 Utils::parseIntoVec2(std::string line)
{
	std::vector<std::string> results = Utils::splitByCharacter(line, ' ');
	return Utils::parseIntoVec2(results);
}
glm::vec2 Utils::parseIntoVec2(std::vector<std::string>& data)
{
	int numParts = data.size();
	float part1 = (float)atof(data.at(numParts-2).c_str());
	float part2 = (float)atof(data.at(numParts-1).c_str());
	glm::vec2 parts = glm::vec2(part1,part2);
	return parts;
}
glm::ivec2 Utils::parseIntoIVec2(std::string line)
{
	return glm::ivec2(Utils::parseIntoVec2(line));
}
glm::ivec2 Utils::parseIntoIVec2(std::vector<std::string>& data)
{
	return glm::ivec2(Utils::parseIntoVec2(data));
}
glm::uvec2 Utils::parseIntoUVec2(std::string line)
{
	return glm::uvec2(Utils::parseIntoVec2(line));
}
glm::uvec2 Utils::parseIntoUVec2(std::vector<std::string>& data)
{
	return glm::uvec2(Utils::parseIntoVec2(data));
}