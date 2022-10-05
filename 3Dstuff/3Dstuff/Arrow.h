#pragma once
#include <cmath>
#include <vector>
#include <glm\glm.hpp>
class Arrow
{
private:
	int numVertices;
	int numIndices;
	int prec;
	float inner;
	float outer;
	std::vector<int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> sTangents;
	std::vector<glm::vec3> tTangents;
	void initArrow(float initx, float inity, float x1, float y1, float x2, float y2, float endx, float endy);
	float toRadians(float degrees);

public:
	Arrow();
	Arrow(float initx, float inity, float x1, float y1, float x2, float y2, float endx, float endy);
	int getNumVertices();
	int getNumIndices();
	std::vector<int> getIndices();
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec2> getTexCoords();
	std::vector<glm::vec3> getNormals();
	std::vector<glm::vec3> getStangents();
	std::vector<glm::vec3> getTtangents();
};