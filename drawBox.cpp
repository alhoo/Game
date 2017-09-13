#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <GL/glu.h>

int width, height;
using namespace std;

void updateTexture(int f);

void init_gl(){
    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();

    float aspect_ratio = ((float)height) / width;
    gluPerspective(45.0f,(1.f/aspect_ratio),0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);

    glShadeModel(GL_SMOOTH);                        // Enables Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                   // Black Background
    glClearDepth(1.0f);                         // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Test To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective Calculations
}

GLFWwindow* init(){
    int s = 0;
    if( !glfwInit() )
    {
        cerr << "No can do" << s++ << endl;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    width = 1024;
    height = 768;

    GLFWwindow* window = glfwCreateWindow( width, height, "Tutorial 01", NULL, NULL);
    // Open a window and create its OpenGL context
    if( window == NULL )
    {
      cerr << "No can do" << s++ << endl;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
      cerr << "No can do" << s++ << endl;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    init_gl();
    return window;
}

void draw(vector<GLuint> texture, int frame)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
    glLoadIdentity();                           // Reset The Current Modelview Matrix


    glTranslatef(0.0f,0.0f,-2.0f);                 // Move Left 1.5 Units And Into The Screen 6.0
//    glColor3f(1.f,0.f,1.0f);              // Set The Color To Blue One Time Only


    updateTexture(frame);
    //glBindTexture(GL_TEXTURE_2D, texture.at(0));
    glLoadIdentity();                           // Reset The Current Modelview Matrix

}

vector<GLuint> loadTexture(){
  vector<GLuint> ret;
  int w = 1024, h=768;
  unsigned char * pixels = new unsigned char[w * h * 3];

  for (unsigned int i = 0; i < w * h; i+=3) {
    pixels[i + 0] = 0x00; // Red
    pixels[i + 1] = 0xf0; // Green
    pixels[i + 2] = 0xf0; // Blue
  }

  glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 

  glEnable(GL_TEXTURE_2D);
  ret.push_back(0);
  return ret;  
}

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

  glBegin(GL_QUADS);                      // Draw A Quad
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);              // Top Left
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 0.0f);              // Top Right
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,1.0f, 0.0f);              // Bottom Right
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,1.0f, 0.0f);              // Bottom Left
  glEnd();                            // Done Drawing The Quad
}
int main(){

    GLFWwindow* window = init();
    if(window == NULL) return 1;
    vector<GLuint> texture = loadTexture();
    unsigned long frame = 0;
    do{
        // Draw nothing, see you in tutorial 2 !
        draw(texture, frame);
        cout << frame++ << "\r";
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );
    cout << frame++ << endl;
    glfwTerminate();
    return 0;
}
