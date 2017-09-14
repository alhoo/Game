#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "video.h"

using namespace std;

int width = 1024, height=768;
unsigned char * pixels = new unsigned char[width * height * 3];

class World {
  public:
    World(){
      this->state[0] = 0;
      start = chrono::system_clock::now();
    }

    void update(){
      auto end = chrono::system_clock::now();
      float dt = chrono::duration_cast<chrono::microseconds>(end - start).count()*0.000001;
      start = end;
      this->state[0] += 100*dt;
    }

    void draw(void (*fn)(unsigned char *, int, int),
              unsigned short width, unsigned short heigth){
      auto k = (((int)(this->state[0]))%3000 + 1000);
      for (auto i = 0; i < 3 * width * heigth; i++) {
        pixels[i] = 0x00;
        if (i%4000 == k){
          pixels[i] = 0xFF;
        }
      }
      fn(pixels,width,heigth);
    }

  private:
     chrono::time_point<chrono::system_clock> start;
     unordered_map<int, int> state;

};

void updatePixels(unsigned char * pixels, int width, int heigth){
  glTexSubImage2D(GL_TEXTURE_2D, 0 ,0, 0, width, heigth,
                  GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)pixels);
}

int main(){

    GLFWwindow* window = init(width, height);
    if(window == NULL) return 1;
    vector<GLuint> texture = loadTexture();
    World w;
    unsigned long frame = 0;
    do{
        w.update();
        w.draw(updatePixels, width, height);
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
