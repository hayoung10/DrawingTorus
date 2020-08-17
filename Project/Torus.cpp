#include "glSetup.h"
#include <Windows.h>
#include "include/GLFW/glfw3.h"
#include "include/GL/gl.h"
#include "include/GL/glut.h"

#include "include/glm/glm.hpp"		// OpenGL Mathematics
#include "include/glm/gtc/type_ptr.hpp"	// glm::value_ptr()
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtx/string_cast.hpp"
using namespace glm;

#pragma comment(lib, "lib/glfw3.lib")
#pragma comment(lib, "lib/opengl32.lib")
#pragma comment(lib, "lib/glut32.lib")

#include <iostream>
using namespace std;

// Camera configuration
vec3	eye(3, 3, 3);
vec3	center(0, 0, 0);
vec3	up(0, 1, 0);

// Light configuration
vec4	light(5.0, 5.0, 0.0, 1);	// Light position

									// Global coordinate frame
float	AXIS_LENGTH = 3;
float	AXIS_LINE_WIDTH = 2;

// Colors
GLfloat bgColor[4] = { 1, 1, 1, 1 };

// Selected example
int selection = 0;

// Control Variable
int yperiod = 0;
int zperiod = 17;
bool toggle = false; // toggle with '5' key

vec3	p[36][18];		// Points
vec3	qcenter[36][18];	// Quads' center points
vec3	normal[36][18];		// Quads' normal vectors

							// Light
void setupLight()
{
	GLfloat ambient[4] = { 0.1, 0.1, 0.1, 1 };
	GLfloat diffuse[4] = { 1.0, 1.0, 1.0, 1 };
	GLfloat specular[4] = { 1.0, 1.0, 1.0, 1 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(light));
}

// Material
void setupMaterial()
{
	// Material
	GLfloat mat_ambient[4] = { 0.1, 0.1, 0.1, 1 };
	GLfloat mat_specular[4] = { 0.5, 0.5, 0.5, 1 };
	GLfloat mat_shininess = 128;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void setDiffuseColor(const vec3& color)
{
	GLfloat mat_diffuse[4] = { color[0], color[1], color[2], 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
}

void init()
{
	// Prepare points p[36][18]
	float angle1 = 0.0f, angle2 = 0.0f;
	vec3 start(0.5f, 0.0f, 0.0f);
	vec3 axisy(0.0f, 1.0f, 0.0f);
	vec3 axisz(0.0f, 0.0f, 1.0f);
	mat4 TR = translate(mat4(1.0), vec3(1.0f, 1.0f, 0.0f));

	// Set p[j][i]
	for (int i = 0; i < 18; i++)
	{
		mat4 R1 = rotate(mat4(1.0), angle1, axisz);
		for (int j = 0; j < 36; j++)
		{
			mat4 R2 = rotate(mat4(1.0), angle2, axisy);
			vec4 V = R2 * TR * R1 * vec4(start, 1);
			p[j][i] = vec3(V.x, V.y, V.z);
			angle2 += (pi<float>() / 18.0f);
		}
		angle1 -= (pi<float>() / 9.0f);
	}

	// Set qcenter[j][i] and normal[j][i]
	for (int i = 0; i < 18; i++)
		for (int j = 0; j < 36; j++)
		{
			qcenter[j][i] = (p[j][i] + p[j][(i + 1) % 18] + p[(j + 1) % 36][(i + 1) % 18] + p[(j + 1) % 36][i]) / 4.0f;
			normal[j][i] = normalize(cross(p[j][i] - p[(j + 1) % 36][(i + 1) % 18], p[j][(i + 1) % 18] - p[(j + 1) % 36][i]));
		}

	// Keyboard
	cout << "Draw 36x18 data points using 'GL_POINTS': '1' key" << endl;
	cout << "Draw the wireframe only: '2' key" << endl;
	cout << "Draw the quads only: '3' key" << endl;
	cout << "Draw the quads and the wireframe: '4' key" << endl;

	cout << "Draw the normal vectors of the polygons: toggle with '5' key" << endl;
	cout << "Two-sided constant shading with the wireframe: '6' key" << endl;

	cout << "Around the y-axis with the 'a', 's' keys" << endl;
	cout << "Around the z-axis with the 'j', 'k' keys" << endl;
}

void drawPoints(int yp, int zp) // Points
{
	glColor3f(0, 0, 0);
	glPointSize(2.0*dpiScaling);
	glBegin(GL_POINTS);
	for (int i = 0; i < zp + 1; i++)
		for (int j = 0; j < yp + 1; j++)
			glVertex3fv(value_ptr(p[j % 36][i % 18]));
	glEnd();
}

void drawWireframe(int yp, int zp) // Wireframe
{
	glColor3f(0, 0, 0);
	glPolygonOffset(0.0f, 0.0f);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (yp == 0 && zp != 0)
	{
		glBegin(GL_LINES);
		for (int i = 0; i < zp; i++)
		{
			glVertex3fv(value_ptr(p[0][i]));
			glVertex3fv(value_ptr(p[0][(i + 1) % 18]));
		}
		glEnd();
	}
	else if (zp == 0 && yp != 0)
	{
		glBegin(GL_LINES);
		for (int j = 0; j < yp; j++)
		{
			glVertex3fv(value_ptr(p[j][0]));
			glVertex3fv(value_ptr(p[(j + 1) % 36][0]));
		}
		glEnd();
	}
	else if (yp == 0 && zp == 0)
	{
		glBegin(GL_POINTS);
		glVertex3fv(value_ptr(p[0][0]));
		glEnd();
	}
	else
	{
		for (int i = 0; i < zp; i++)
		{
			glBegin(GL_QUADS);
			for (int j = 0; j < yp; j++)
			{
				glVertex3fv(value_ptr(p[j][i]));
				glVertex3fv(value_ptr(p[j][(i + 1) % 18]));
				glVertex3fv(value_ptr(p[(j + 1) % 36][(i + 1) % 18]));
				glVertex3fv(value_ptr(p[(j + 1) % 36][i]));
			}
			glEnd();
		}
	}
}

void drawQuads(int yp, int zp) // Quads
{
	glPolygonOffset(1.f, 1.f);

	if (yp == 0 && zp != 0)
	{
		glColor3f(0, 0, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_LINES);
		for (int i = 0; i < zp; i++)
		{
			glVertex3fv(value_ptr(p[0][i]));
			glVertex3fv(value_ptr(p[0][(i + 1) % 18]));
		}
		glEnd();
	}
	else if (zp == 0 && yp != 0)
	{
		glColor3f(0, 0, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_LINES);
		for (int j = 0; j < yp; j++)
		{
			glVertex3fv(value_ptr(p[j][0]));
			glVertex3fv(value_ptr(p[(j + 1) % 36][0]));
		}
		glEnd();
	}
	else if (yp == 0 && zp == 0)
	{
		glColor3f(0, 0, 0);
		glBegin(GL_POINTS);
		glVertex3fv(value_ptr(p[0][0]));
		glEnd();
	}
	else
	{
		glColor3f(0, 0, 1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_QUADS);
		for (int i = 0; i < zp; i++)
		{

			for (int j = 0; j < yp; j++)
			{
				glVertex3fv(value_ptr(p[j][i]));
				glVertex3fv(value_ptr(p[j][(i + 1) % 18]));
				glVertex3fv(value_ptr(p[(j + 1) % 36][(i + 1) % 18]));
				glVertex3fv(value_ptr(p[(j + 1) % 36][i]));
			}
		}
		glEnd();
	}
}

void drawWQ(int yp, int zp) // Quads and Wireframe
{
	glEnable(GL_POLYGON_OFFSET_FILL);
	drawQuads(yp, zp);
	glDisable(GL_POLYGON_OFFSET_FILL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	drawWireframe(yp, zp);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void normalVector(int yp, int zp) // Two-sided constant shading with Wireframe
{
	glEnable(GL_POLYGON_OFFSET_FILL);
	drawQuads(yp, zp);
	glDisable(GL_POLYGON_OFFSET_FILL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	drawWireframe(yp, zp);

	if (yp != 0 && zp != 0)
	{
		glColor3f(0, 0, 0);
		glPolygonOffset(1.f, 1.f);
		for (int i = 0; i < zp; i++)
			for (int j = 0; j < yp; j++)
			{
				glBegin(GL_LINES);
				glVertex3fv(value_ptr(qcenter[j][i]));
				glVertex3fv(value_ptr(qcenter[j][i] + 0.1f * normal[j][i]));
				glEnd();
			}
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void twoSidedShading(int yp, int zp) // Two-sided constant Shading with Wireframe
{
	glEnable(GL_POLYGON_OFFSET_FILL);
	if (yp != 0 && zp != 0)
	{
		glPolygonOffset(1.f, 1.f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		for (int i = 0; i < zp; i++)
		{
			glBegin(GL_QUADS);
			for (int j = 0; j < yp; j++)
			{
				vec3 v = eye - qcenter[j][i];
				if (dot(normal[j][i], v) >= 0)
					glColor3f(0, 0, 1);
				else
					glColor3f(1, 0, 0);
				glVertex3fv(value_ptr(p[j][i]));
				glVertex3fv(value_ptr(p[j][(i + 1) % 18]));
				glVertex3fv(value_ptr(p[(j + 1) % 36][(i + 1) % 18]));
				glVertex3fv(value_ptr(p[(j + 1) % 36][i]));
			}
			glEnd();
		}
	}
	else
		drawQuads(yp, zp);
	glDisable(GL_POLYGON_OFFSET_FILL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	drawWireframe(yp, zp);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int ypd(int yp) // yperiod
{
	int y;
	if (yp >= 0)
		y = (yp % 37);
	else
	{
		if (abs(yp) % 37 == 0)
			y = 0;
		else
			y = (37 - (abs(yp) % 37));
	}
	return y;
}

int zpd(int zp) // zperiod
{
	int z;
	if (zp + 1 >= 0)
		z = ((zp + 1) % 19);
	else
	{
		if (abs(zp + 1) % 19 == 0)
			z = 0;
		else
			z = (19 - (abs(zp + 1) % 19));
	}
	return z;
}

void render(GLFWwindow* window)
{
	// Background color
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	// Axes
	glDisable(GL_LIGHTING);
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH*dpiScaling);

	// Lighting
	setupLight();

	// Material
	setupMaterial();

	int y = ypd(yperiod);
	int z = zpd(zperiod);

	// Draw
	switch (selection)
	{
	case 1: drawPoints(y, z); break;
	case 2: drawWireframe(y, z); break;
	case 3: drawQuads(y, z); break;
	case 4: drawWQ(y, z); break;
	case 5: if (toggle == true) normalVector(y, z); else drawWQ(y, z); break;
	case 6: twoSidedShading(y, z); break;
	}
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
			// Quit
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE); break;

			// Example selection
		case GLFW_KEY_1: selection = 1; 			break;
		case GLFW_KEY_2: selection = 2; 			break;
		case GLFW_KEY_3: selection = 3; 			break;
		case GLFW_KEY_4: selection = 4; 			break;
		case GLFW_KEY_5: selection = 5; toggle = !toggle;	break;
		case GLFW_KEY_6: selection = 6; 			break;

			// Around the y-axis
		case GLFW_KEY_A: yperiod += 1; break;
		case GLFW_KEY_S: yperiod -= 1; break;

			// Around the z-axis
		case GLFW_KEY_J: zperiod += 1; break;
		case GLFW_KEY_K: zperiod -= 1; break;
		}
	}
}

int main(int argc, char* argv[])
{
	// Initialize the OpenGL system
	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL)	return -1;

	// Callbacks
	glfwSetKeyCallback(window, keyboard);

	// Depth test
	glEnable(GL_DEPTH_TEST);

	// Normal vectors are normalized after transformation
	glEnable(GL_NORMALIZE);

	// Viewport and perspective setting
	reshape(window, windowW, windowH);

	// Initialization
	init();

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();		// Events
		render(window);			// Draw one frame
		glfwSwapBuffers(window);	// Swap buffers
	}

	// Terminate the glfw system
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}