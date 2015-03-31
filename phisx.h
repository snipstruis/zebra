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

struct jet_t{
glm::quat rotation;

	glm::vec3 position;

	float velocity;
	float resitance;
	float gravity;
	float lift;

};

void jet_init(jet_t &jet){
	jet.rotation = glm::quat(1,0,0,0);

	jet.position = glm::vec3(0.f,0.f,0.f);

	jet.velocity = 5.0f;
	jet.resitance = 0.0f;
	jet.gravity = 9.0f;
	jet.lift = 9.81f;

}

void getInput(GLFWwindow* &window, jet_t &jet,float time_of_last_loop){

	jet.resitance = 0.5f * 1.2754f * jet.velocity * jet.velocity * 0.5f * 0.4f;

	glm::vec3 combinded_vector_without_gravity = glm::vec3(0,jet.lift,jet.velocity - jet.resitance);
	
glm::vec3 without_gravity_and_rotated = glm::mat3_cast(jet.rotation) * combinded_vector_without_gravity;

	jet.position.x += time_of_last_loop *  without_gravity_and_rotated.x;
	jet.position.y += time_of_last_loop * (without_gravity_and_rotated.y - jet.gravity);
	jet.position.z -= time_of_last_loop *  without_gravity_and_rotated.z;

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
		jet.rotation = glm::rotate(jet.rotation, time_of_last_loop * -0.5f, glm::vec3(0,1,0));
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
		jet.rotation = glm::rotate(jet.rotation, time_of_last_loop * 0.5f, glm::vec3(0,1,0));
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

