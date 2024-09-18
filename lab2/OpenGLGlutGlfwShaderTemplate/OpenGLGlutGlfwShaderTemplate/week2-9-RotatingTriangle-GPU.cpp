
///////////////////////////////////////////////////////////////////////
//
// Rotating Triangles.cpp
//
///////////////////////////////////////////////////////////////////////

using namespace std;

#include "stdlib.h"
#include "time.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "prepShader.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <array>
#include <string>


#define BUFFER_OFFSET(x)  ((const void*) (x))

GLuint Buffers[1];

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint vaoT, vaoS, points_vbo, colors_vbo, modelID, squarepoints_vbo, squarecolors_vbo;

const GLfloat scale = 0.5f;

 float angle = 45.0f;

GLfloat points[] = {
	0.0f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
};

GLfloat colors[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

GLfloat squarepoints[] = {
	0.5f, 0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f
};

GLfloat squarecolors[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f
};

static unsigned int
program,
vertexShaderId,
fragmentShaderId;


void init(void)
{

	// Create shader program executable.

	vertexShaderId = setShader((char*)"vertex", (char*)"cube.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"cube.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
	glUseProgram(program);

	modelID = glGetUniformLocation(program, "model");

	vaoT = 0;
	glGenVertexArrays(1, &vaoT);
	glBindVertexArray(vaoT);
	points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.
	//glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.


	vaoS = 1;
	glGenVertexArrays(1, &vaoS);
	glBindVertexArray(vaoS);
	squarepoints_vbo = 0;
	glGenBuffers(1, &squarepoints_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, squarepoints_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), squarepoints, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	squarecolors_vbo = 0;
	glGenBuffers(1, &squarecolors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, squarecolors_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), squarecolors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	//glBindVertexArray(0);
}

//---------------------------------------------------------------------
//
// transformModel
//

void transformObject(glm::vec3(scale), glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, scale);
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &Model[0][0]);
}

//---------------------------------------------------------------------
//
// display
//

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.0f;
	static float Delta = 0.005f;

	Scale += Delta;
	if ((Scale > 1.0f) || (Scale < -1.0f))
	{
		Delta *= -1.0f;
	}

	glBindVertexArray(vaoT);
	angle = angle + 0.5f;
	transformObject(glm::vec3(0.25f, 0.25f, 0.25f), Z_AXIS, angle, glm::vec3(-0.5f, 0.5f, 0.0f));

	//Let's have fun!
	//transformObject(glm::vec3(sinf(Scale), sinf(Scale), sinf(Scale)), Z_AXIS, angle, glm::vec3(0.0f, 0.0f, 0.0f));

	//Ordering the GPU to start the pipeline
	glDrawArrays(GL_TRIANGLES, 0, 3); // GL_LINE_LOOP works too!

	transformObject(glm::vec3(0.25f, 0.25f, 0.25f), Z_AXIS, angle, glm::vec3(0.5f, 0.5f, 0.0f));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	transformObject(glm::vec3(0.25f, 0.25f, 0.25f), Z_AXIS, angle, glm::vec3(0.5f, -0.5f, 0.0f));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	transformObject(glm::vec3(0.25f, 0.25f, 0.25f), Z_AXIS, angle, glm::vec3(-0.5f, -0.5f, 0.0f));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	transformObject(glm::vec3(0.25f, 0.25f, 0.25f), Z_AXIS, angle, glm::vec3(0.0f, 0.0f, 0.0f));
	glDrawArrays(GL_LINE_LOOP, 0, 3);

	//drawing squares
	glBindVertexArray(vaoS);
	transformObject(glm::vec3(0.4f, 0.4f, 0.4f), Z_AXIS, 0.0, glm::vec3(-0.5f, -0.5f, 0.0f));
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	transformObject(glm::vec3(0.4f, 0.4f, 0.4f), Z_AXIS, 0.0, glm::vec3(0.5f, -0.5f, 0.0f));
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	transformObject(glm::vec3(0.4f, 0.4f, 0.4f), Z_AXIS, 0.0, glm::vec3(-0.5f, 0.5f, 0.0f));
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	transformObject(glm::vec3(0.4f, 0.4f, 0.4f), Z_AXIS, 0.0, glm::vec3(0.5f, 0.5f, 0.0f));
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

//---------------------------------------------------------------------
//
// main
//

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Triangle Tranformation");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutMainLoop();
}
