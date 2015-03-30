#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // for perspective
#include <cstdlib> // exit

#include "util.h" // colors and getScreenSize
#include "object.h" 
#include "shader.h"
#include "phisx.h" // to store the orientation

#include <iostream> // cout endl
using namespace std;
using namespace glm;
using namespace std::chrono;


GLFWwindow* createWindow(ivec2 windowsize){
	GLFWwindow* window;

	// create window with an OpenGL 3.1 context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
	window = glfwCreateWindow(windowsize.x, windowsize.y, "float", NULL, NULL);

	// check if window has been created
	if (!window){ 
		cout<<FAIL"GLFW failed to create a window\n";
		glfwTerminate();
		exit(EXIT_FAILURE); }
	else cout<<OK"GLFW window created"<<endl;

	// assign context to current thread
	glfwMakeContextCurrent(window);

	return window;
}

void uniform_set(GLint loc, glm::mat4 data){
	float array[16] = {
		data[0][0],data[0][1],data[0][2],data[0][3],
		data[1][0],data[1][1],data[1][2],data[1][3],
		data[2][0],data[2][1],data[2][2],data[2][3],
		data[3][0],data[3][1],data[3][2],data[3][3]
	};
	glUniformMatrix4fv(loc,1,false,array);
}

void uniform_set(GLint loc,glm::vec3 data){
	glUniform3f(loc,data.x,data.y,data.z);
}

int main(int argc, char** argv){
	if(argc!=3){
		cout<<FAIL"usage: "<<argv[0]<<" shader.vert shader.frag\n";
		return 1;
	}
	std::string vert_src = read_file(argv[1]);
	std::string frag_src = read_file(argv[2]);

	// initialize glfw
	if (!glfwInit()){
		cout<<FAIL"GLFW failed to initialize\n";
		exit(EXIT_FAILURE);
	}
	else cout<<OK"GLFW initialized\n";

	GLFWwindow* window = createWindow(ivec2(800,600));

	// initialise GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err){
		cerr<<FAIL"GLEW "<<glewGetErrorString(err)<<endl;
		exit(EXIT_FAILURE);
	} else cout<<OK"GLEW initialized\n";

	// set GL states
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glCullFace(GL_FRONT);
	
	std::vector<GLfloat> vertices = {
		-1.0,-1.0, 1.0,
         1.0,-1.0, 1.0,
         1.0, 1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0,-1.0,-1.0,
         1.0,-1.0,-1.0,
         1.0, 1.0,-1.0,
        -1.0, 1.0,-1.0
	};

	std::vector<GLushort> indices = {
		0, 1, 2,  2, 3, 0,
        3, 2, 6,  6, 7, 3,
        7, 6, 5,  5, 4, 7,
        4, 0, 3,  3, 7, 4,
        0, 1, 5,  5, 4, 0,
        1, 5, 6,  6, 2, 1
	};

	std::vector<float> locations_of_circles = {
		-1.0,-1.0, 1.0,
         1.0,-1.0, 1.0,
         1.0, 1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0,-1.0,-1.0,
         1.0,-1.0,-1.0,
         1.0, 1.0,-1.0,
        -1.0, 1.0,-1.0
	};

	// load shaders
	GLuint program = make_shader(vert_src, frag_src);
	if(!program) return 1;
	glUseProgram(program);

	// set color
	GLint color_uniform = glGetUniformLocation(program,"color");
	if(color_uniform==-1){
		std::cout<<FAIL
			"failed to find uniform \"color\" in shader program\n";
	}else cout<<OK"found uniform \"color\"\n";
	uniform_set(color_uniform, glm::vec3(0.7,0.1,0.5));
	
	// set modelview matrix
	GLint modelview_uniform = glGetUniformLocation(program,"modelview");
	if(modelview_uniform==-1){
		std::cout<<FAIL
			"failed to find uniform \"modelview\" in shader program\n";
	}else cout<<OK"found uniform \"modelview\"\n";

	// set projection matrix
	GLint projection_uniform = glGetUniformLocation(program,"projection");
	if(projection_uniform==-1){
		std::cout<<FAIL
			"failed to find uniform \"projection\" in shader program\n";
	}else cout<<OK"found uniform \"projection\"\n";

	uniform_set(projection_uniform, glm::perspective(1.f,4.f/3.f,0.1f,100.f));
	object o = create_object( vertices, indices );
	
	uniform_set(projection_uniform, glm::perspective(1.f,4.f/3.f,0.1f,100.f));
	object g = create_object( vertices, indices );

	float x=0;
	cout<<INFO"entering main loop\n";
	
	global_position_of_plane ori;
	global_position_of_plane_init(ori);
	
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	auto previous = high_resolution_clock::now();

	glm::mat4 model;

	addCircleGrid(locations_of_circles, 15, 10, 100); // fils the map with a grid of objects so you can see that you're moving.

	while(true){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		model = glm::translate(glm::mat4(),glm::vec3(0,-0.5f,-1.f))
					* glm::rotate(glm::mat4(),0.0f,
							glm::normalize(vec3(1,1,-1)))
					* glm::scale(mat4(),vec3(0.1,0.1,0.1));

		uniform_set(modelview_uniform, model);

		draw_object(o);

		auto current = high_resolution_clock::now();
		auto elapsed = duration_cast<microseconds>(current-previous);
		ori.time_of_last_loop = elapsed.count() / 1000000.0f;
		previous = current;

		glfwPollEvents();
		getInput(window, ori);
		glfwSwapInterval(1);

		for(int i = 0; i < locations_of_circles.size()/3; i++){
			model =			  glm::rotate(glm::mat4(),ori.rotation_x,glm::normalize(vec3(1,0,0)))
					* model = glm::rotate(glm::mat4(),ori.rotation_y,glm::normalize(vec3(0,1,0)))
					* model = glm::rotate(glm::mat4(),ori.rotation_z,glm::normalize(vec3(0,0,1)))
					* glm::translate(glm::mat4(),glm::vec3(	-ori.position_x + locations_of_circles[i*3],
															 ori.position_y + locations_of_circles[i*3+1],
															-ori.position_z + locations_of_circles[i*3+2]))
					* glm::scale(mat4(),vec3(0.1,0.1,0.1));
	
			uniform_set(modelview_uniform, model);
			
			draw_object(g);
		}

		glfwSwapBuffers(window); // blocking when window is visible (under X)
		x+=0.01;
	}

	destroy_object(o);
	glUseProgram(0);
	glDeleteProgram(program);
}

