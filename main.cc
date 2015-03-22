#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // for perspective
#include <cstdlib> // exit

#include "util.h" // colors and getScreenSize
#include "object.h" 
#include "shader.h"

#include <iostream> // cout endl
using namespace std;
using namespace glm;

std::string vert_src =
	"#version 140\n"
	"uniform mat4 modelview;\n"
	"uniform mat4 projection;\n"
	"in vec3 vertex;\n"
	"out vec3 pos;\n"
	"void main(void) {\n"
	"	gl_Position = projection * modelview * vec4(vertex,1);\n"
	"	vec4 pos4 = modelview * vec4(vertex,1);\n"
	"	pos = pos4.xyz;\n"
	"}\n";

std::string frag_src =
	"#version 140\n"
	"uniform vec3 color;\n"
	"in  vec3 pos;\n"
	"out vec4 fragColor;\n"
	"vec3 lighting(const vec3 normal, const vec3 lightangle){\n"
	"	return vec3(acos(dot(normal,lightangle)));\n"
	"}\n"
	"void main(){\n"
	"	vec3 normal = normalize(cross(dFdx(pos), dFdy(pos)));\n"
	"	vec3 lightangle = normalize(vec3(1.0,1.0,1.0));\n"
	"	fragColor = vec4(color*lighting(normal,lightangle),1.0);\n"
	"}\n";

GLFWwindow* createWindow(ivec2 windowsize){
	GLFWwindow* window;

	// create window with an OpenGL 3.1 context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
	window = glfwCreateWindow(windowsize.x, windowsize.y, "window!", NULL, NULL);

	// check if window has been created
	if (!window){ glfwTerminate(); exit(EXIT_FAILURE); }
	else cout<<OK"GLFW: window succesfully created"<<endl;

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

int main(){
	if (!glfwInit()) exit(EXIT_FAILURE);
	GLFWwindow* window = createWindow(ivec2(800,600));

	ivec2 screensize = getScreenSize(window);

	// initialise GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err){
		cerr<<FAIL"GLEW: "<<glewGetErrorString(err)<<endl;
		exit(EXIT_FAILURE);
	} else cout<<OK"GLEW: succesfully initialized"<<endl;

	// set GL states
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glCullFace(GL_FRONT);
	
	std::vector<GLfloat> vertices = {
		1.0f, -1.0f, 1.0f, 
		1.0f, -1.0f, 1.0f, 
		1.0f, 1.0f, 1.0f, 
		1.0f, 1.0f, 1.0f, 
		1.0f, -1.0f, -1.0f, 
		1.0f, -1.0f, -1.0f, 
		1.0f, 1.0f, -1.0f, 
		1.0f, 1.0f, -1.0f 
	};

	std::vector<GLushort> indices = {
		0, 1, 2, 2, 3, 0, 
		3, 2, 6, 6, 7, 3, 
		7, 6, 5, 5, 4, 7, 
		4, 0, 3, 3, 7, 4, 
		0, 1, 5, 5, 4, 0,
		1, 5, 6, 6, 2, 1 
	};	

	GLuint program = make_shader(vert_src, frag_src);

	if(!program) return 1;
	glUseProgram(program);

	// set color
	GLint color_uniform = glGetUniformLocation(program,"color");
	if(color_uniform==-1){
		std::cout<<FAIL"failed to find uniform \"color\" in shader program\n";
	}
	uniform_set(color_uniform, glm::vec3(0.7,0.1,0.5));
	
	// set modelview matrix
	GLint modelview_uniform = glGetUniformLocation(program,"modelview");
	if(modelview_uniform==-1){
		std::cout<<FAIL"failed to find uniform \"modelview\" in shader program\n";
	}
	uniform_set(modelview_uniform, glm::mat4(0.2,0  ,0  ,0,
											 0  ,0.2,0  ,0,
											 0  ,0  ,0.2,-0.5,
											 0  ,0  ,0  ,1));

	// set projection matrix
	GLint projection_uniform = glGetUniformLocation(program,"projection");
	if(projection_uniform==-1){
		std::cout<<FAIL"failed to find uniform \"projection\" in shader program\n";
	}
	/*uniform_set(projection_uniform, glm::perspectiveFov(60.f,
													 (float)screensize.x,
													 (float)screensize.y,
													 0.01f,
													 100.f));
	*/

	uniform_set(projection_uniform, glm::ortho(-1,1,-1,1));

	object o = create_object( vertices, indices );

	float x=0;
	while(true){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::translate(glm::mat4(),vec3(0,0,-0.5))
						* glm::rotate(glm::mat4(),x,vec3(0,1,0))
						* glm::scale(mat4(),vec3(0.2,0.2,0.2));

		uniform_set(modelview_uniform, model);

		draw(o);
		
		glfwSwapBuffers(window);
		x+=0.001;
	}
	
}
