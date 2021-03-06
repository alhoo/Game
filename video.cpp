#include "video.h"

using namespace std;

void init_gl(unsigned short width, unsigned short height){
    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();

    /*
    gluLookAt (0.0, 1.0, 1.7, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    glScalef (1.0, 2.0, 1.0);
    */

    float aspect_ratio = ((float)height) / width;
    gluPerspective(45.0f,(1.f/aspect_ratio),0.1f,10.0f);
    glMatrixMode(GL_MODELVIEW);

    glShadeModel(GL_SMOOTH);                        // Enables Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                   // Black Background
    glClearDepth(1.0f);                         // Depth Buffer Setup
    glDisable(GL_DEPTH_TEST);
    //glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Test To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective Calculations
    glEnable(GL_MULTISAMPLE);
}

GLFWwindow* init(unsigned short width, unsigned short height){
    int s = 0;
    if( !glfwInit() )
    {
        cerr << "No can do" << s++ << endl;
    }
    glfwWindowHint(GLFW_SAMPLES, 16); // 16x antialiasing

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

    init_gl(width, height);
    return window;
}

void drawBox(){
  glBegin(GL_QUADS);                      // Draw A Quad
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);              // Top Left
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 0.0f);              // Top Right
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,1.0f, 0.0f);              // Bottom Right
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,1.0f, 0.0f);              // Bottom Left
  glEnd();                            // Done Drawing The Quad
}
int frame = 0;
void draw(vector<GLuint> texture)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
    glLoadIdentity();                           // Reset The Current Modelview Matrix


    glTranslatef(0.0f,0.0f,-2.5f);
    glRotatef(-70.0f, 1, 0, 0);
    glRotatef(0.1f * frame++, 0, 0, 1 );

    drawBox();
    glLoadIdentity();                           // Reset The Current Modelview Matrix
}

vector<GLuint> loadTexture(int w, int h){
  vector<GLuint> ret;
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
