#pragma once

#include "util.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>
#include <chrono>
#include <cmath>

using namespace std;

#define PI 3.14159265358979323846264338327950288419716939937510 

struct global_position_of_plane{
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

void global_position_of_plane_init(global_position_of_plane &ori){
	ori.rotation_x = 0.0f; 
	ori.rotation_y = 0.0f;
	ori.rotation_z = 0.0f;

	ori.position_x = 0.0f;
	ori.position_y = 0.0f;
	ori.position_z = 0.0f;

	ori.velocity = 5.0f;
	ori.resitance = 0.0f;
	ori.gravity = 9.0f;
	ori.lift = 9.81f;
	ori.time_of_last_loop = 0.0f;
}

void move(global_position_of_plane &ori, float &axis, float direction){
	axis += ori.time_of_last_loop * direction;
}

void getInput(GLFWwindow* &window, global_position_of_plane &ori){

	ori.resitance = 0.5f * 1.2754f * ori.velocity * ori.velocity * 0.5f * 0.4f;

	glm::vec3 combinded_vector_without_gravity = glm::vec3(0,ori.lift,ori.velocity - ori.resitance);
	glm::vec3 without_gravity_and_rotated; 
	glm::vec3 rotated_and_with_gravity;

	without_gravity_and_rotated = glm::rotate(combinded_vector_without_gravity, 
		float(ori.rotation_x * PI), glm::vec3(1,0,0));
	without_gravity_and_rotated = glm::rotate(without_gravity_and_rotated, 
		float(-ori.rotation_y * PI), glm::vec3(0,1,0));
	without_gravity_and_rotated = glm::rotate(without_gravity_and_rotated, 
		float(ori.rotation_z * PI), glm::vec3(0,0,1));

	rotated_and_with_gravity.x = without_gravity_and_rotated.x;
	rotated_and_with_gravity.y = without_gravity_and_rotated.y - ori.gravity;
	rotated_and_with_gravity.z = without_gravity_and_rotated.z;

	ori.position_x += ori.time_of_last_loop * rotated_and_with_gravity.x;
	ori.position_y += ori.time_of_last_loop * rotated_and_with_gravity.y;
	ori.position_z -= ori.time_of_last_loop * rotated_and_with_gravity.z;

	cout << "x: " << rotated_and_with_gravity.x << " y: " << rotated_and_with_gravity.y <<
		" z: " << rotated_and_with_gravity.z << endl;



	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		move(ori,ori.rotation_x, 0.5f);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		move(ori,ori.rotation_x, -0.5f);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		move(ori,ori.rotation_z, -0.5f);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		move(ori,ori.rotation_z, 0.5f);
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
		move(ori,ori.rotation_y, -0.5f);
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
		move(ori,ori.rotation_y, 0.5f);
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


