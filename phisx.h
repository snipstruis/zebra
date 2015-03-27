#pragma once

#include "util.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <chrono>
#include <cmath>

using namespace std;

struct orientation{
	float rotation_x;
	float rotation_y;
	float rotation_z;

	float position_x;
	float position_y;
	float position_z;

	float velocity;
	float resitance;
	float gravity;
	float lift;
	float time_of_last_loop;

};

void orientation_init(orientation &ori){
	ori.rotation_x = 0.0f; 
	ori.rotation_y = 0.0f;
	ori.rotation_z = 0.0f;

	ori.position_x = 0.0f;
	ori.position_y = 0.0f;
	ori.position_z = 0.0f;

	ori.velocity = 5.0f;
	ori.resitance = 0.0f;
	ori.gravity = 9.81f;
	ori.lift = 9.81f;
	ori.time_of_last_loop = 0.0f;
}

void move(orientation &ori, float &axis, float direction){
	axis += ori.time_of_last_loop * direction;
}

void getInput(GLFWwindow* &window, orientation &ori){

	ori.resitance = 0.5f * 1.2754f * ori.velocity * ori.velocity * 1.0f * 0.4f;


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		move(ori,ori.position_z, -1.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		move(ori,ori.position_z, 1.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		move(ori,ori.position_x, -1.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		move(ori,ori.position_x, 1.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
		move(ori,ori.position_y, -1.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
		move(ori,ori.position_y, 1.0f);
	}
	


}

void addCircle(std::vector<float> &vec, float x, float y, float z){
	vec.push_back(x);
	vec.push_back(y);
	vec.push_back(z);
}

void addCircleGrid(std::vector<float> &vec, int x_axis, int y_axis, int z_axis){
	for(float x = 0.0f; x < x_axis; x+=1.0f){
		for(float y = 0.0f - y_axis; y < 0.0; y+=1.0f){
			for(float z = 0.0f - z_axis; z < 0.0f; z+=1.0f){
				addCircle(vec, x, y, z);
			}
		}
	}
}


