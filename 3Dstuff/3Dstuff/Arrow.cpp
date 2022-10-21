#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <cmath>
#include <vector>
#include <random>
#include "Arrow.h"
using namespace std;






Arrow::Arrow() {
	prec = 48;
	inner = 0.5f;
	outer = 0.2f;
	initArrow(0, 0, 0, 0, 0, 0, 0, 0);
}

Arrow::Arrow(float initx, float inity, float x1, float y1, float x2, float y2, float endx, float endy) {
	
	initArrow(initx, inity, x1, y1, x2, y2, endx, endy);
}

float Arrow::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void swapArrow(float* xp, float* yp)
{
	float temp = *xp;
	*xp = *yp;
	*yp = temp;
}

// A function to implement array sort
void arraySortArrow(float arrX[], float arrY[], int n, bool t)
{
	int i, j;
	for (i = 0; i < n - 1; i++)
		for (j = 0; j < n - i - 1; j++)
			if (t)
			{
				if (arrX[j] > arrX[j + 1]) {
					swapArrow(&arrY[j], &arrY[j + 1]);
					swapArrow(&arrX[j], &arrX[j + 1]);
				}
			}
			else
			{
				if (arrX[j] < arrX[j + 1]) {
					swapArrow(&arrY[j], &arrY[j + 1]);
					swapArrow(&arrX[j], &arrX[j + 1]);
				}
			}
	for (int i = 0; i < 4; i++)
	{
		cout << "Y A : " << arrY[i] << endl;
		cout << "X A: " << arrX[i] << endl;
	}



}

void Arrow::initArrow(float initx, float inity, float x1, float y1, float x2, float y2, float endx, float endy) {
	float x[4], y[4];

	x[0] = initx; x[1] = x1; x[2] = x2; x[3] = endx;//fixed points for testing
	y[0] = inity; y[1] = y1; y[2] = y2; y[3] = endy;

	//arraySort(x, y, 4, true);//no for this

	//x[0] = -20; x[1] = 10; x[2] = 15; x[3] = 69;//fixed points for testing
	//y[0] = 0; y[1] = 10; y[2] = 85; y[3] = 1;
	prec = (int)(x[3] - x[0]);
	/*x[0] = 0;  x[3] = 69;//fixed points for testing
	y[0] = random(-60,60);  y[3] = 0;

	x[1] = x[3]/4; x[2] = x[3]-(x[3]/4);
	y[1] = random(-60,60); y[2] = random(-60,60) ;
	*/
	cout << "pass by" << endl;
	numVertices = (prec + 1)*(prec + 1);
	numIndices = prec * prec * 6;
	for (int i = 0; i < numVertices; i++) { vertices.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { texCoords.push_back(glm::vec2()); }
	for (int i = 0; i < numVertices; i++) { normals.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { sTangents.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { tTangents.push_back(glm::vec3()); }
	for (int i = 0; i < numIndices; i++) { indices.push_back(0); }

	// calculate first ring
	float xt = 0;
	float yt = 0;
	double t = 0;
	int testloop = 0;
	int div = vertices.size();
	int checkPoint = div;
	int current = 0;
	int next = 1;
	//for (int i = 0; i < 4; i++, t+=0.01) {
	for (int i = 0; i < prec; i++) {
		checkPoint++;
		if (checkPoint == i)
		{
			current++; next++;
			checkPoint = checkPoint + div;
		}
		float amt = 1;// toRadians(i*360.0f / prec);
		//xt = pow(1 - t, 3)*x[0] + 3 * t*pow(1 - t, 2)*x[1] + 3 * pow(t, 2)*(1 - t)*x[2] + pow(t, 3)*x[3];
		//yt = pow(1 - t, 3)*y[0] + 3 * t*pow(1 - t, 2)*y[1] + 3 * pow(t, 2)*(1 - t)*y[2] + pow(t, 3)*y[3];
		//xt = pow(1 - t, 2)*x[0] + 2 * t*(1 - t)*x[1] + pow(t, 2)*x[2];
		//yt = pow(1 - t, 2)*y[0] + 2 * t*(1 - t)*y[1] + pow(t, 2)*y[2];

		//BAZIER generation
		//xt = pow(1 - t, 2)*x[0] + (-2 * pow(t, 2) + 2 * t)*x[1] + pow(t, 2)*x[2];
		//yt = pow(1 - t, 2)*y[0] + (-2 * pow(t, 2) + 2 * t)*y[1] + pow(t, 2)*y[2];
		//cout << " xt; " << xt << " yt: " << yt << endl;

		//curve easy ??
		xt += ((x[next] - x[current]) / div);
		if (checkPoint > div)
		{
			yt += ((y[next] - y[current]) / div);

		}
		else
		{
			yt = y[current];
		}

		glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
		//rMat = glm::transpose(rMat);
		//uncomment to test shape bazier curve
		//glm::vec3 initPos(rMat * glm::vec4(xt,yt, outer, 1.0f));
		//vertices[i] = glm::vec3(initPos + glm::vec3(xt, yt, inner));
		glm::vec3 initPos(rMat * glm::vec4(xt, yt, 0, 1.0f));
		vertices[i] = glm::vec3(initPos + glm::vec3(xt, yt, 0));

		//vertices[i] =  glm::vec3(xt, yt, 0.0f);//bad

		//torus
		//glm::vec3 initPos(rMat * glm::vec4( 0.0f, 0.0f, outer, 1.0f));
		//vertices[i] = glm::vec3(initPos + glm::vec3( 0, 0,inner));

		cout << "vertexs " << vertices[i].x << " , " << vertices[i].y << " , " << vertices[i].z << endl;
		texCoords[i] = glm::vec2(0.0f, ((float)i / (float)prec));

		rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
		tTangents[i] = glm::vec3(rMat * glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f));

		sTangents[i] = glm::vec3(glm::vec3(0.0f, -1.0f, 0.0f));
		normals[i] = glm::cross(tTangents[i], sTangents[i]);
	}
	// rotate the first ring about Y to get the other rings
	for (int ring = 1; ring < prec + 1; ring++) {
		for (int i = 0; i < prec + 1; i++) {
			float amt = 1;//(float)toRadians((float)ring * 360.0f / (prec));

			glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(1.0f, 0.0f, 0.0f));
			//vertices[0] = glm::vec3(inner + outer + 0.01, 0.0f, 0.0f);
			vertices[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(vertices[i], 1.0f));

			texCoords[ring*(prec + 1) + i] = glm::vec2((float)ring*2.0f / (float)prec, texCoords[i].t);
			//if (texCoords[ring*(prec + 1) + i].s > 1.0) texCoords[ring*(prec+1)+i].s -= 1.0f; // deleted and added someting at end of inint

			rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(1.0f, 0.0f, 0.0f));
			sTangents[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(sTangents[i], 1.0f));

			rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(1.0f, 0.0f, 0.0f));
			tTangents[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(tTangents[i], 1.0f));

			rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(1.0f, 0.0f, 0.0f));
			normals[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(normals[i], 1.0f));
		}
	}
	// calculate triangle indices
	for (int ring = 0; ring < prec; ring++) {
		for (int i = 0; i < prec; i++) {
			indices[((ring*prec + i) * 2) * 3 + 0] = ring * (prec + 1) + i;
			indices[((ring*prec + i) * 2) * 3 + 1] = (ring + 1)*(prec + 1) + i;
			indices[((ring*prec + i) * 2) * 3 + 2] = ring * (prec + 1) + i + 1;
			indices[((ring*prec + i) * 2 + 1) * 3 + 0] = ring * (prec + 1) + i + 1;
			indices[((ring*prec + i) * 2 + 1) * 3 + 1] = (ring + 1)*(prec + 1) + i;
			indices[((ring*prec + i) * 2 + 1) * 3 + 2] = (ring + 1)*(prec + 1) + i + 1;

		}
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//seen to not be needed

}
int Arrow::getNumVertices() { return numVertices; }
int Arrow::getNumIndices() { return numIndices; }
std::vector<int> Arrow::getIndices() { return indices; }
std::vector<glm::vec3> Arrow::getVertices() { return vertices; }
std::vector<glm::vec2> Arrow::getTexCoords() { return texCoords; }
std::vector<glm::vec3> Arrow::getNormals() { return normals; }
std::vector<glm::vec3> Arrow::getStangents() { return sTangents; }
std::vector<glm::vec3> Arrow::getTtangents() { return tTangents; }