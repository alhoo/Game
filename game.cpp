#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "video.h"

using namespace std;


int width, height;

void updateTexture(int f);

void updateTexture(int f){
  int w = 1024, h=768;
  unsigned char * pixels = new unsigned char[w * h * 3];
  for (unsigned int i = 0; i < 3 * w * h; i++) {
    pixels[i] = 0x00;
    if (i%4000 == (f%3000 + 1000)){
      pixels[i] = 0xFF;
    }
  }
  glTexSubImage2D(GL_TEXTURE_2D, 0 ,0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)pixels);
}

int main(){

    GLFWwindow* window = init(width, height);
    if(window == NULL) return 1;
    vector<GLuint> texture = loadTexture();
    unsigned long frame = 0;
    do{
        // Draw nothing, see you in tutorial 2 !
        updateTexture(frame);
        draw(texture);
        cout << frame++ << "\r";
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );
    cout << frame << endl;
    glfwTerminate();
    return 0;
}
