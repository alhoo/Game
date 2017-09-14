#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "video.h"

using namespace std;

int w = 1024, h=768;
unsigned char * pixels = new unsigned char[w * h * 3];

class World {
  public:
    World(){
      this->state[0] = 0;
      start = chrono::system_clock::now();
    };
    void update(){
      auto end = chrono::system_clock::now();
      float dt = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()*0.000001;
      start = end;
      this->state[0] += 100*dt;
    }
    void draw(unsigned char * pixels, unsigned short w, unsigned short h){
      auto k = (((int)(this->state[0]))%3000 + 1000);
      for (auto i = 0; i < 3 * w * h; i++) {
        pixels[i] = 0x00;
        if (i%4000 == k){
          pixels[i] = 0xFF;
        }
      }
    }
  private:
     chrono::time_point<chrono::system_clock> start;
     unordered_map<int, int> state;
};

void updateTexture(World world){
  world.draw(pixels, w, h);
  glTexSubImage2D(GL_TEXTURE_2D, 0 ,0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)pixels);
}

int main(){

    GLFWwindow* window = init(w, h);
    if(window == NULL) return 1;
    vector<GLuint> texture = loadTexture();
    World w;
    unsigned long frame = 0;
    do{
        updateTexture(w);
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        w.update();
        draw(texture);
        frame++;
    }
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );
    cout << frame << endl;
    glfwTerminate();
    return 0;
}
