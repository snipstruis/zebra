#pragma once

#define CLR  "\x1B[0m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"
#define OK   "[" GRN " OK " CLR "] "
#define WARN "[" YEL "WARN" CLR "] "
#define FAIL "[" RED "FAIL" CLR "] "
#define INFO "[" BLU "INFO" CLR "] "

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

glm::ivec2 getScreenSize(GLFWwindow* window){
	glm::ivec2 t;
	glfwGetFramebufferSize(window,&t.x,&t.y);
	return t;
}


