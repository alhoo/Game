#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "video.h"

#define SHOW_BORDERS 1
#define SHOW_CENTER 1

using namespace std;

int width = 1024, height=768;
unsigned char * pixels = new unsigned char[256*256 * 3];

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
              unsigned short width, unsigned short height){
//      auto k = (((int)(this->state[0]))%3000 + 1000);
      int W = 1;
      for (auto i = 0; i < 3 * width * height; i++) {
        pixels[i] = 0x00;
      }
      auto px = 300*sin(this->state[0]*0.1);
      auto py = 300*cos(this->state[0]*0.1);
      auto cp = 3*((int)(3*width * height/2 + 3*width/2)/3);
      for (auto i = 0; i < 100; i++) {
        int x = 3*((int)((20*sin(i*0.063) + px)/3));
        int y = 3*((int)((20*cos(i*0.063) + py)/3));
        pixels[cp + x*width + y] = 0xFF;
        pixels[cp + x*width + y + 1] = 0xFF;
        pixels[cp + x*width + y + 2] = 0xFF;
      }
      //Borders
      if(SHOW_BORDERS){
        for (auto k = 0; k < 2*W; k++){
          for (auto i = 0; i < width; i++) {
            pixels[3*i + k*(3 * width)] = 0xFF;
            pixels[3*i + k*(3 * width) + 1] = 0xFF;
            pixels[3*i + (3 * width)*(height - 1 - k)] = 0xFF;
            pixels[3*i + (3 * width)*(height - 1 - k) + 1] = 0xFF;
          }
          for (auto i = 0; i < height; i++) {
            pixels[i*(3*width) + 3*k] = 0xFF;
            pixels[i*(3*width) + 3*k + 2] = 0xFF;
            pixels[i*(3*width) + 3*(width - 1 - k)] = 0xFF;
            pixels[i*(3*width) + 3*(width - 1 - k)+2] = 0xFF;
          }
        }
      }
      //Center cross
      if(SHOW_CENTER){
        for (auto k = -W; k <= W; k++){
          for (auto i = -50; i < +50; i++) {
            pixels[cp + 3*i + 1 + k*3*width] = 0xFF;
            pixels[cp + 3*i + 2 + k*3*width] = 0xFF;
          }
          for (auto i = height/2 - 50; i < height/2 + 50; i++) {
            pixels[i*(3*width) + 3*k + 3*(3*width/6) + 1] = 0xFF;
            pixels[i*(3*width) + 3*k + 3*(3*width/6) + 2] = 0xFF;
          }
        }
      }
      fn(pixels,width,height);
    }

  private:
     chrono::time_point<chrono::system_clock> start;
     unordered_map<int, int> state;

};

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
