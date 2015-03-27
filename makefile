all : main.cc object.h util.h shader.h phisx.h
	clang++ -std=c++14 -O3 -Wall main.cc -lglfw -lGL -lGLEW -lm

