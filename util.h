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

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
std::string read_file(char* filename){
	int fd = open(filename, O_RDONLY);
	if(!fd){
		std::cout<<FAIL"failed to open file "<<BLU
			<<filename<<CLR"\n";
		exit(1);
	}
	struct stat file_stat;
	if(fstat(fd,&file_stat)<0){
		std::cout<<FAIL"failed to get file status on "<<BLU
			<<filename<<CLR"\n";
		exit(1);
	}
	auto filesize = file_stat.st_size;
	std::string str(filesize, 0);
	if(read(fd, &str[0], filesize)<0){
		std::cout<<FAIL"failed to read from file "<<BLU
			<<filename<<CLR"\n";
		exit(1);
	}
	return str;
}

