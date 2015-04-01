#pragma once

#include "util.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <chrono>
#include <cmath>

using namespace std;

#define PI 3.14159265358979323846264338327950288419716939937510 

float const drag_constant    = 0.127752;
float const gravity_constant = 9.00;
float const lift_constant    = 9.81;

struct jet_t{
	glm::quat rotation;
	glm::vec3 position;
	float velocity;
};

void jet_init(jet_t &jet){
	jet.rotation = glm::quat(1,0,0,0);
	jet.position = glm::vec3(0.f,0.f,0.f);
	jet.velocity = 5.0f;
}

void getInput(GLFWwindow* &window, jet_t &jet,float time_of_last_loop){

	float resitance = drag_constant * jet.velocity * jet.velocity;
	
	glm::quat rotation_vector = jet.rotation;
	rotation_vector.z *= -1.f; 

	glm::vec3 combined_vector = glm::mat3_cast(rotation_vector) * glm::vec3(0,lift_constant ,jet.velocity - resitance);

	jet.position.x += time_of_last_loop *  combined_vector.x;
	jet.position.y += time_of_last_loop * (combined_vector.y - gravity_constant);
	jet.position.z -= time_of_last_loop *  combined_vector.z;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		jet.rotation = glm::rotate(jet.rotation, time_of_last_loop * 0.5f, glm::vec3(1,0,0));
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		jet.rotation = glm::rotate(jet.rotation, time_of_last_loop * -0.5f, glm::vec3(1,0,0));
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		jet.rotation = glm::rotate(jet.rotation, time_of_last_loop * -0.5f, glm::vec3(0,0,1));
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		jet.rotation = glm::rotate(jet.rotation, time_of_last_loop * 0.5f, glm::vec3(0,0,1));
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
		jet.rotation = glm::rotate(jet.rotation, time_of_last_loop * 0.5f, glm::vec3(0,1,0));
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
		jet.rotation = glm::rotate(jet.rotation, time_of_last_loop * -0.5f, glm::vec3(0,1,0));
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






