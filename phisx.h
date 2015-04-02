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
float const gravity_constant = 0.00;
float const lift_constant    = 0.00;

struct jet_t{
	glm::quat rotation;
	glm::vec3 position;
	glm::vec3 origin;
	float velocity;
};

void jet_init(jet_t &jet){
	jet.rotation = glm::quat(1,0,0,0);
	jet.position = glm::vec3(0.f,0.f,0.f);
	jet.velocity = 100.0f;
}

uint16_t jet_rotate_left  = GLFW_KEY_Q,
		 jet_rotate_right = GLFW_KEY_E,
		 jet_rotate_up    = GLFW_KEY_S,
		 jet_rotate_down  = GLFW_KEY_W,
		 jet_rotate_cw    = GLFW_KEY_D,
		 jet_rotate_ccw   = GLFW_KEY_A;

void getInput(GLFWwindow* &window, jet_t &jet,float time_of_last_loop){
	glm::vec3 orientation = glm::vec3(0,0,0);
	if(glfwGetKey(window,jet_rotate_down)) orientation += glm::vec3( 0.02, 0, 0);
	if(glfwGetKey(window,jet_rotate_up))   orientation += glm::vec3(-0.02, 0, 0);
	if(glfwGetKey(window,jet_rotate_right))orientation += glm::vec3( 0, 0.005, 0);
	if(glfwGetKey(window,jet_rotate_left)) orientation += glm::vec3( 0,-0.005, 0);
	if(glfwGetKey(window,jet_rotate_cw))   orientation += glm::vec3( 0, 0, 0.05);
	if(glfwGetKey(window,jet_rotate_ccw))  orientation += glm::vec3( 0, 0,-0.05);
	jet.rotation  = glm::quat(orientation) * jet.rotation;
	jet.origin   += glm::vec3(0,0,time_of_last_loop*jet.velocity)
			      * jet.rotation;
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






