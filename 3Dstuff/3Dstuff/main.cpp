/* HEADER-----
Assignment 2 CSCE 515

Name: Roberto Salazar ULID c00416436

date: 10/09/2022

List of current Bugs:
1.- For some reason I was not able to implement more than one vbo element. I tried but not positive result were present

2.-the current program is able to  display a vase drawing a line between three point that can be modify in the tauros constructor.

3.-the rotation are done in the following pattern:
	<- && -> arrow  move matrix multipy to the right on Y
	^ && v arrow  move matrix multipy to the left on X
	A && S key  move matrix multipy to the right  on Z
	Z && X key  move matrix multipy to the left  on Z
	Q && W key  move matrix multipy to the right  on Y

4.- there is some problems in the rotation. I belive beacuse  I did not implemented a new homogenus M matrix properly.

5.- I belive I will need the program that Dr. brost mentionen in classes to improve my own program. Due that I still strugelling with openGL implementation.


*/
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Torus.h"
#include "Arrow.h"
#include "Utils.h"
using namespace std;

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

#define numVAOs 1
#define numVBOs 4
float AngleIncrement = 10.0f;
float cameraX, cameraY, cameraZ;
float torLocX, torLocY, torLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint vbo2[numVBOs];
GLuint brickTexture;
float rotAmt = 0.0f;

// variable allocation for display
GLuint mvLoc, projLoc,mLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, M;
glm::mat4 temp;
float Ax, Ay, Az;
//x[0] = -20; x[1] = 10; x[2] = 15; x[3] = 69;//fixed points for testing
	//y[0] = 0; y[1] = 10; y[2] = 85; y[3] = 1;
Torus myTorus(0,60,25,50,65,40,95,30,48);
Arrow myArrow(0, 1, 10, 1, 15, 1, 70, 1);
glm::mat4 buildTranslate(float x, float y,float z) {
	glm::mat4 r = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1
	);
	return r;
}

glm::mat4 rotateX(float rad) {
	glm::mat4 r = glm::mat4(
		1, 0, 0, 0,
		0, cos(rad), sin(rad), 0,
		0, -sin(rad), cos(rad), 0,
		0, 0, 0, 1
	);
	return r;
}
glm::mat4 rotateY(float rad) {
	glm::mat4 r = glm::mat4(
		cos(rad), 0, -sin(rad), 0,
		0, 1, 0, 0,
		sin(rad), 0, cos(rad), 0,
		0, 0, 0, 1
	);
	return r;
}
glm::mat4 rotateZ(float rad) {
	glm::mat4 r = glm::mat4(
		cos(rad), sin(rad), 0, 0,
		-sin(rad), cos(rad), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	return r;
}
glm::mat4 buildScala(float x, float y, float z) {
	glm::mat4 r = glm::mat4(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	);
	return r;
}
void setupVertices(void) {
	std::vector<int> ind = myTorus.getIndices();
	std::vector<glm::vec3> vert = myTorus.getVertices();
	std::vector<glm::vec2> tex = myTorus.getTexCoords();
	std::vector<glm::vec3> norm = myTorus.getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;
	
	for (int i = 0; i < myTorus.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}
	///////////
	std::vector<int> indA = myArrow.getIndices();
	std::vector<glm::vec3> vertA = myArrow.getVertices();
	std::vector<glm::vec2> texA = myArrow.getTexCoords();
	std::vector<glm::vec3> normA = myArrow.getNormals();

	std::vector<float> pAvalues;
	std::vector<float> tAvalues;
	std::vector<float> nAvalues;

	for (int i = 0; i < myArrow.getNumVertices(); i++) {
		pAvalues.push_back(vertA[i].x);
		pAvalues.push_back(vertA[i].y);
		pAvalues.push_back(vertA[i].z);
		tAvalues.push_back(texA[i].s);
		tAvalues.push_back(texA[i].t);
		nAvalues.push_back(normA[i].x);
		nAvalues.push_back(normA[i].y);
		nAvalues.push_back(normA[i].z);
	}
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);

	///////
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo2[0]);
	glBufferData(GL_ARRAY_BUFFER, pAvalues.size() * 4, &pAvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo2[1]);
	glBufferData(GL_ARRAY_BUFFER, tAvalues.size() * 4, &tAvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo2[2]);
	glBufferData(GL_ARRAY_BUFFER, nAvalues.size() * 4, &nAvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo2[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indA.size() * 4, &indA[0], GL_STATIC_DRAW);


}

void init(GLFWwindow* window) {

	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 2.0f;
	torLocX = 0.0f; torLocY = 0.0f; torLocZ = -0.5f;

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	//pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
	pMat = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1
	);
	vMat = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	mMat = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	M = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	setupVertices();
	brickTexture = Utils::loadTexture("brick1.jpg");
	glEnable(GL_PRIMITIVE_RESTART);
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	
}

void display(GLFWwindow* window, double currentTime,float xTr, float yTr, float zTr, glm::mat4 &temp) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);
	mLoc = glGetUniformLocation(renderingProgram, "m_matrix");
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	//vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, cameraZ*cameraZ*cameraZ)); // changed to identity
	/*double tf;
	for (int i = 0; i < 100; i++)
	{
		tf = currentTime + i;
		mMat = glm::translate(glm::mat4(1.0f), glm::vec3(Ax*tf, Ay*tf, torLocZ*tf));
		//mMat *= glm::eulerAngleXYZ(toRadians(30.0f), 0.0f, 0.0f);
		mMat = glm::rotate(mMat, toRadians(30.0f*tf), glm::vec3(1.0f, 0.0f, 0.0f));*/
		//mMat = glm::translate(glm::mat4(1.0f), glm::vec3(xTr, yTr, zTr));
		//mMat *= glm::eulerAngleXYZ(toRadians(30.0f), 0.0f, 0.0f);
		//mvMat = temp;
		glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(M));
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glActiveTexture(GL_TEXTURE0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
		glBindTexture(GL_TEXTURE_2D, brickTexture);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glPrimitiveRestartIndex(0xFFFF);//
		glEnable(GL_PRIMITIVE_RESTART);//ADDED BUT DOES NOT WORK
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
		//glDrawElements(GL_TRIANGLE_STRIP, myTorus.getIndices().size(), GL_UNSIGNED_INT, 0);// TRIANGLE_STRIP ADDED, DOESNOT WORK
		glDrawElements(GL_TRIANGLE_STRIP, myTorus.getIndices().size(), GL_UNSIGNED_INT, 0);
	//}

		//glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(M));
		//glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

		glBindBuffer(GL_ARRAY_BUFFER, vbo2[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo2[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glActiveTexture(GL_TEXTURE0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
		glBindTexture(GL_TEXTURE_2D, brickTexture);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glPrimitiveRestartIndex(0xFFFF);//
		glEnable(GL_PRIMITIVE_RESTART);//
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo2[3]);
		//glDrawElements(GL_TRIANGLE_STRIP, myArrow.getIndices().size(), GL_UNSIGNED_INT, 0);

		// connne

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	glm::mat4 Mtemp = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		cout << "UP" << endl;
		Ay -= AngleIncrement;
		//mMat = mMat + buildTranslate(0, Ay, 0);
		M =  M *rotateY(toRadians(Ay));
		//mvMat = mMat * vMat;
		mMat = M;
		mvMat = mMat * vMat;
		//mvMat += mvMat;

		
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		cout << "Down" << endl;
		Ay += AngleIncrement;
		//mMat = mMat + buildTranslate(0, Ay, 0);
		M = M * rotateY(toRadians(Ay)) ;
		//mvMat = mMat * vMat;
		mMat = M;
		mvMat = mMat * vMat;
		//mvMat += mvMat;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		cout << "lefy" << endl;
		Az -= AngleIncrement;
		//mMat = mMat + buildTranslate(Ax, 0, 0);
		M = rotateZ(toRadians(Az)) * M ;
		//mvMat = mMat * vMat;
		mMat = M;
		mvMat = mMat * vMat;
		//mvMat += mvMat;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		cout << "righfy" << endl;
		Az += AngleIncrement;
		//mMat = mMat + buildTranslate(Ax, 0, 0);
		M = rotateZ(toRadians(Az))* M ;
		//mvMat = mMat * vMat;
		mMat = M;
		mvMat = mMat * vMat;
		//mvMat += mvMat;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		cout << "Z right --" << endl;
		Az -= AngleIncrement;
		//mMat = mMat + buildTranslate(0, 0, Az);
		M =  M  * rotateZ(toRadians(Az));
		//mvMat = mMat * vMat;
		mMat = M;
		mvMat = mMat * vMat;
		//mvMat += mvMat;


	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		cout << "z right ++" << endl;
		Az += AngleIncrement;
		//mMat = mMat + buildTranslate(0, 0, Az);
		M =  M  * rotateZ(toRadians(Az));
		//mvMat = mMat * vMat;
		mMat = M;
		mvMat = mMat * vMat;
		//mvMat += mvMat;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		cout << "Z left --" << endl;
		//mMat = mMat + buildTranslate(0, 0, Az);
		M = buildTranslate( -0.1,0,0)* M ;
		//mvMat = mMat * vMat;
		//vMat = buildTranslate(10, 0, 0) * vMat;
		mMat = M;
		mvMat = mMat * vMat;
		//mvMat += mvMat;


	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		cout << "Z left ++" << endl;
		//mMat = mMat + buildTranslate(0, 0, Az);
		M = buildTranslate( 0.1, 0, 0)* M ;// not working either
		//mvMat = mMat * vMat;
		//vMat = buildTranslate(-10, 0, 0)*vMat;
		mMat = M;
		mvMat = mMat * vMat;
		//mvMat += mvMat;
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		cout << "X left --" << endl;
		//mMat = mMat + buildTranslate(0, 0, Ax);
		M =  M *(buildTranslate( -0.1, 0, 0));//nop working
		mMat = M ;
		mvMat = mMat * vMat;
		//mvMat += mvMat;


	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		cout << "X left ++" << endl;
		//mMat = mMat + buildTranslate(0, 0, Ax);
		M =   M *(buildTranslate( 0.1, 0, 0));
		mMat = M ;
		mvMat = mMat * vMat;
		//mvMat += mvMat;
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		Ax += AngleIncrement;
		cout << "X left --" << endl;
		//mMat = mMat + buildTranslate(0, 0, Ax);
		vMat = vMat * rotateX(toRadians(Ax));//nop working
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		mvMat = mMat * vMat;
		//mvMat += mvMat;


	}
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		Ax -= AngleIncrement;
		cout << "X left ++" << endl;
		//mMat = mMat + buildTranslate(0, 0, Ax);
		vMat = vMat * rotateX(toRadians(Ax));;//nop working
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		mvMat = mMat * vMat;
		//mvMat += mvMat;
	}
		
}


void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1
	);
}



void CartesianPlane() {

	// draw some lines
	glColor3f(1.0, 0.0, 0.0); // red x
	glBegin(GL_LINES);
	// x aix

	glVertex3f(-4.0, 0.0f, 0.0f);
	glVertex3f(4.0, 0.0f, 0.0f);

	glVertex3f(4.0, 0.0f, 0.0f);
	glVertex3f(3.0, 1.0f, 0.0f);

	glVertex3f(4.0, 0.0f, 0.0f);
	glVertex3f(3.0, -1.0f, 0.0f);
	glEnd();

	// y 
	glColor3f(0.0, 1.0, 0.0); // green y
	glBegin(GL_LINES);
	glVertex3f(0.0, -4.0f, 0.0f);
	glVertex3f(0.0, 4.0f, 0.0f);

	glVertex3f(0.0, 4.0f, 0.0f);
	glVertex3f(1.0, 3.0f, 0.0f);

	glVertex3f(0.0, 4.0f, 0.0f);
	glVertex3f(-1.0, 3.0f, 0.0f);
	glEnd();

	// z 
	glColor3f(0.0, 0.0, 1.0); // blue z
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0f, -4.0f);
	glVertex3f(0.0, 0.0f, 4.0f);


	glVertex3f(0.0, 0.0f, 4.0f);
	glVertex3f(0.0, 1.0f, 3.0f);

	glVertex3f(0.0, 0.0f, 4.0f);
	glVertex3f(0.0, -1.0f, 3.0f);
	glEnd();

}


int main(void) {
	Ax = Ay = 0;
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(1500, 1000, "here we go again program2", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_size_callback);

	init(window);
	//mMat = rotateY(toRadians(0));
//	mvMat = mMat * vMat;
	//M = mvMat;
	display(window, glfwGetTime(), 0, 0, 0, temp);
	while (!glfwWindowShouldClose(window)) {
		//mMat = rotateY(toRadians(0));
		mvMat = mMat * vMat;
		glfwSwapBuffers(window);
		display(window, glfwGetTime(), Ax, Ay, Az, temp);
		glfwSetKeyCallback(window, key_callback);
		glfwWaitEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}