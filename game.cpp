#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "video.h"
#include "simulation.h"



using namespace std;
using namespace boost::numeric;

typedef float ScalarType;

int width = 1024, height=768;

void updatePixels(unsigned char * pixels, int width, int height){
  glTexSubImage2D(GL_TEXTURE_2D, 0 ,0, 0, width, height,
                  GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)pixels);
}

int main(){

    GLFWwindow* window = init(width, height);
    if(window == NULL) return 1;
    vector<GLuint> texture = loadTexture(256, 256);
    World w;
    unsigned long frame = 0;
    do{
        w.update();
        w.draw(updatePixels, 256, 256);
        draw(texture);
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        frame++;
    }
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );
    cout << frame << endl;
    glfwTerminate();
    return 0;
}
