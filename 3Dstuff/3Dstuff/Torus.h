#include <cmath>
#include <vector>
#include <glm\glm.hpp>
class Torus
{
private:
	int numVertices;
	int numIndices;
	int prec;
	int precRing;
	float inner;
	float outer;
	std::vector<int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> sTangents;
	std::vector<glm::vec3> tTangents;
	void init(int initx, int inity, int x1, int y1, int x2, int y2, int endx, int endy, int precRing);
	void initArrow(int initx, int inity, int x1, int y1, int x2, int y2, int endx, int endy, int precRing);
	float toRadians(float degrees);

public:
	Torus();
	Torus(int initx, int inity, int x1, int y1, int x2, int y2, int endx, int endy, int precRing);
	int getNumVertices();
	int getNumIndices();
	std::vector<int> getIndices();
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec2> getTexCoords();
	std::vector<glm::vec3> getNormals();
	std::vector<glm::vec3> getStangents();
	std::vector<glm::vec3> getTtangents();
};