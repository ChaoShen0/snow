#include "main.h"
#include "PointCloud.h"
#include "Grid.h"
#include <time.h>

using namespace std;

//Old and new time values for each timestep
double old_time, new_time = glfwGetTime();
bool dirty_buffer = true;

//Simulation data
PointCloud snow;
Grid* grid;

int main(int argc, char** argv) {	
	//Create GLFW window
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(WIN_W, WIN_H, "Snow Simulator", NULL, NULL);
	if (!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	
	//Center window on screen
	const GLFWvidmode* monitor = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, (monitor->width-WIN_W)/2, (monitor->height-WIN_H)/2);
	
	//Setup OpenGL context
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, WIN_W, WIN_H);
	glOrtho(0, WIN_W, 0, WIN_H, 0, 1);
	
	//Set default visualization parameters
	glClearColor(1, 1, 1, 1);
	
	//Setup simulation data
	snow = *PointCloud::createSquare(1.5, 20);
	snow.scale(Vector2f(0), Vector2f(7));
	snow.translate(Vector2f(230, 400));
	
	grid = new Grid(Vector2f(0), Vector2f(WIN_W, WIN_H), Vector2f(60), &snow);
	grid->initialize();	
	grid->calculateVolumes();	//only for first iteration
	
	//Create default simulation loop
	pthread_t sim_thread;
	pthread_create(&sim_thread, NULL, simulate, NULL);
	
	//Drawing & event loop
	while (!glfwWindowShouldClose(window)){
		if (dirty_buffer){
			redraw();
			dirty_buffer = false;
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Exit
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
	return 0;
}
//Print all errors to console
static void error_callback(int error, const char* description){
	printf("\nError: %s",description);
}
//Key listener
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    switch (key){
		default: return;
	}
}

void redraw(){
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_POINT_SMOOTH);
		
	//Grid nodes
	glColor3f(1, 0, 0);
	glPointSize(2);
	glBegin(GL_POINTS);
	for (int i=0; i<grid->size[0]; i++){
		for (int j=0; j<grid->size[1]; j++)
			glVertex2fv((grid->origin+grid->cellsize*Vector2f(i, j)).loc);
	}
	glEnd();
	
	//Snow particles
	glColor3f(0, 0, 1);
	glPointSize(4);
	glBegin(GL_POINTS);
	for (int i=0; i<snow.size; i++)
		glVertex2fv(snow.particles[i].position.loc);
	glEnd();
}

void *simulate(void *args){
	cout << "Starting simulation thread..." << endl;
	pthread_exit(NULL);
}
