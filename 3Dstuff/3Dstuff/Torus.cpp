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
#include "Torus.h"
using namespace std;


std::random_device rd;
std::mt19937 gen(rd());

int random(int low, int high)
{
	std::uniform_int_distribution<> dist(low, high);
	return dist(gen);
}

Torus::Torus() {
	prec = 48;
	inner = 0.5f;
	outer = 0.2f;
	init(0,0, 0, 0);
}

Torus::Torus(float in, float out, int precIn) {
	prec = precIn;
	inner = in;
	outer = out;
	init(0,0, 0, 0);
}

float Torus::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Torus::init(int controlx, int controly,int endx, int endy) {
	float x[4], y[4];
	int newPrec= prec;
	/*if (endx > controlx)
	{
		x[0] = 0; x[1] = controlx; x[2] = endx; 
		y[0] = 0; y[1] = controly; y[2] = endy; 
	}
	else
	{
		x[0] = 0; x[2] = controlx; x[1] = endx; 
		y[0] = 0; y[2] = controly; y[1] = endy; 
	}*/
	
	/*if (x[0]<x[2])
	{
		newPrec = prec * (x[2] - x[0]);
	}
	else
	{
		newPrec = prec * (x[0] - x[2]);
	}*/

	x[0] = 0; x[1] = 10; x[2] = 15; x[3] = 69;//fixed points for testing
	y[0] = 0; y[1] = 10; y[2] = 85; y[3] = 0;

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
	float xt;
	float yt;
	double t = 0;
	for (int i = 0; i < prec + 1; i++, t+=0.01) {
		float amt = toRadians(i*360.0f / prec);
		//xt = pow(1 - t, 3)*x[0] + 3 * t*pow(1 - t, 2)*x[1] + 3 * pow(t, 2)*(1 - t)*x[2] + pow(t, 3)*x[3];
		//yt = pow(1 - t, 3)*y[0] + 3 * t*pow(1 - t, 2)*y[1] + 3 * pow(t, 2)*(1 - t)*y[2] + pow(t, 3)*y[3];
		//xt = pow(1 - t, 2)*x[0] + 2 * t*(1 - t)*x[1] + pow(t, 2)*x[2];
		//yt = pow(1 - t, 2)*y[0] + 2 * t*(1 - t)*y[1] + pow(t, 2)*y[2];

		//BAZIER generation
		xt = pow(1 - t, 2)*x[0] + (-2 * pow(t, 2) + 2 * t)*x[1] + pow(t, 2)*x[2];
		yt = pow(1 - t, 2)*y[0] + (-2 * pow(t, 2) + 2 * t)*y[1] + pow(t, 2)*y[2];
		//cout << " xt; " << xt << " yt: " << yt << endl;


		glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
		//rMat = glm::transpose(rMat);
		//uncomment to test shape
		glm::vec3 initPos(rMat * glm::vec4(xt,yt, outer, 1.0f));
		vertices[i] = glm::vec3(initPos + glm::vec3(xt, yt, inner));
		
		//vertices[i] =  glm::vec3(xt, yt, 0.0f);//bad

		//torus
		//glm::vec3 initPos(rMat * glm::vec4( 0.0f, 0.0f, outer, 1.0f));
		//vertices[i] = glm::vec3(initPos + glm::vec3( 0, 0,inner));

		cout << "vertexs " << vertices[i].x <<" , "<< vertices[i].y << " , "<<vertices[i].z <<endl;
		texCoords[i] = glm::vec2(0.0f, ((float)i / (float)prec));

		rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
		tTangents[i] = glm::vec3(rMat * glm::vec4(-1.0f,0.0f, 0.0f, 1.0f));

		sTangents[i] = glm::vec3(glm::vec3(0.0f, -1.0f, 0.0f));
		normals[i] = glm::cross(tTangents[i], sTangents[i]);
	}
	// rotate the first ring about Y to get the other rings
	for (int ring = 1; ring < prec + 1; ring++) {
		for (int i = 0; i < prec + 1; i++) {
			float amt = (float)toRadians((float)ring * 360.0f / (prec));

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
int Torus::getNumVertices() { return numVertices; }
int Torus::getNumIndices() { return numIndices; }
std::vector<int> Torus::getIndices() { return indices; }
std::vector<glm::vec3> Torus::getVertices() { return vertices; }
std::vector<glm::vec2> Torus::getTexCoords() { return texCoords; }
std::vector<glm::vec3> Torus::getNormals() { return normals; }
std::vector<glm::vec3> Torus::getStangents() { return sTangents; }
std::vector<glm::vec3> Torus::getTtangents() { return tTangents; }