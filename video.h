#ifndef VIDEO_HH
#define VIDEO_HH

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void init_gl(unsigned short, unsigned short);
GLFWwindow* init(unsigned short, unsigned short);
void drawBox();
void draw(std::vector<GLuint>);

std::vector<GLuint> loadTexture();

#endif
