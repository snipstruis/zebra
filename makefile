all : main.cc object.h util.h shader.h
	clang++ -std=c++14 -O3 main.cc -lglfw -lGL -lGLEW -lm

