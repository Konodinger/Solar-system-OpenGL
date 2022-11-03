// ----------------------------------------------------------------------------
// main.cpp
//
//  Created on: 24 Jul 2020
//      Author: Kiwon Um
//        Mail: kiwon.um@telecom-paris.fr
//
// Description: IGR201 Practical; OpenGL and Shaders (DO NOT distribute!)
//
// Copyright 2020-2022 Kiwon Um
//
// The copyright to the computer program(s) herein is the property of Kiwon Um,
// Telecom Paris, France. The program(s) may be used and/or copied only with
// the written permission of Kiwon Um or in accordance with the terms and
// conditions stipulated in the agreement/contract under which the program(s)
// have been supplied.
// ----------------------------------------------------------------------------

#define _USE_MATH_DEFINES

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <memory>
#include <random>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// constants
const static float kSizeSun = 1;
const static float kSizeEarth = 0.5;
const static float kSizeMoon = 0.25;
const static float kRadOrbitEarth = 10;
const static float kRadOrbitMoon = 2;

// light source position
const static glm::vec3 light = {0., 0., 0.};

// Model transformation matrices
glm::mat4 g_sun, g_earth, g_moon;

// Window parameters
GLFWwindow *g_window = nullptr;

// GPU objects
GLuint g_program = 0; // A GPU program contains at least a vertex shader and a fragment shader

// OpenGL identifiers
GLuint g_vao = 0;
GLuint g_posVbo = 0;
GLuint g_colVbo = 0;
GLuint g_ibo = 0;

// Texture ID
GLuint g_earthTexID;
GLuint g_moonTexID;

// All vertex positions packed in one array [x0, y0, z0, x1, y1, z1, ...]
std::vector<float> g_vertexPositions;
// All vertex colors packed in one array [r0, g0, b0, r1, g1, b1, ...]
std::vector<float> g_vertexColors;
// All triangle indices packed in one array [v00, v01, v02, v10, v11, v12, ...] with vij the index of j-th vertex of the i-th triangle
std::vector<unsigned int> g_triangleIndices;

//Standard functions
void crossProduct(const std::vector<float> &a, const std::vector<float> &b, std::vector<float> &c) { // c = a ^ b
  assert ((a.size() == 3) & (b.size() == 3));
  c.push_back(a[1]*b[2] - b[1]*a[2]);
  c.push_back(a[2]*b[0] - b[2]*a[0]);
  c.push_back(a[0]*b[1] - b[0]*a[1]);
}

void normal(const std::vector<float> &a, const std::vector<float> &b, const std::vector<float> &c, std::vector<float> &n) { // n is the normal of the triangle abc
  assert((a.size() == 3) & (b.size() == 3) & (c.size() == 3));
  crossProduct({b[0] - a[0], b[1] - a[1], b[2] - a[2]}, {c[0] - a[0], c[1] - a[1], c[2] - a[2]}, n);
  float norm = 0;
  for (int i = 0; i < 3; i++) {
    norm += pow(n[i], 2);
  }
  norm = pow(norm, 0.5);
  for (int i = 0; i < 3; i++) {
    n[i] /= norm;
  }

}

void convertSpherical(float r, float t, float p, std::vector<float> & v) { // v is the cartesian coordinates conversion of the spherical coordinates (r, t, p)
  r = abs(r);
  v.push_back(r * sin(t) * cos(p));
  v.push_back(r * sin(t) * sin(p));
  v.push_back(r * cos(t));
}

void randomizeN(float range, std::vector<float> & a, std::vector<float> & b) { //Add a uniform noise between -range and range
  float r;
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_real_distribution<float> distr(-range, range);
  for (std::vector<float>::iterator it = a.begin(); it != a.end(); ++it) {
    r = distr(eng);
    b.push_back(*it + r);
  }
}


// Basic camera model
class Camera {
public:
  inline float getFov() const { return m_fov; }
  inline void setFoV(const float f) { m_fov = f; }
  inline float getAspectRatio() const { return m_aspectRatio; }
  inline void setAspectRatio(const float a) { m_aspectRatio = a; }
  inline float getNear() const { return m_near; }
  inline void setNear(const float n) { m_near = n; }
  inline float getFar() const { return m_far; }
  inline void setFar(const float n) { m_far = n; }
  inline void setPosition(const glm::vec3 &p) { m_pos = p; }
  inline glm::vec3 getPosition() { return m_pos; }

  inline glm::mat4 computeViewMatrix() const {
    return glm::lookAt(m_pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  }

  // Returns the projection matrix stemming from the camera intrinsic parameter.
  inline glm::mat4 computeProjectionMatrix() const {
    return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_near, m_far);
  }

private:
  glm::vec3 m_pos = glm::vec3(0, 0, 0);
  float m_fov = 45.f;        // Field of view, in degrees
  float m_aspectRatio = 1.f; // Ratio between the width and the height of the image
  float m_near = 0.1f; // Distance before which geometry is excluded from the rasterization process
  float m_far = 10.f; // Distance after which the geometry is excluded from the rasterization process
};
Camera g_camera;

// Class mesh for geometry manipulation
class Mesh {
  public:
    void init() {// should properly set up the geometry buffer

      #ifdef _MY_OPENGL_IS_33_
        glGenVertexArrays(1, &m_vao); // If your system doesn't support OpenGL 4.5, you should use this instead of glCreateVertexArrays.
      #else
        glCreateVertexArrays(1, &m_vao);
      #endif
        glBindVertexArray(m_vao);

        // Generate a GPU buffer to store the positions of the vertices
        size_t vertexBufferSize = sizeof(float)*m_vertexPositions.size(); // Gather the size of the buffer from the CPU-side vector
      #ifdef _MY_OPENGL_IS_33_
        glGenBuffers(1, &m_posVbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_posVbo);
        glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, m_vertexPositions.data(), GL_DYNAMIC_READ);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
      #else
        glCreateBuffers(1, &m_posVbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_posVbo);
        glNamedBufferStorage(m_posVbo, vertexBufferSize, m_vertexPositions.data(), GL_DYNAMIC_STORAGE_BIT); // Create a data storage on the GPU and fill it from a CPU array
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
      #endif

      // Generate a GPU buffer to store the colors of the vertices
        size_t colorBufferSize = sizeof(float)*m_vertexNormals.size(); // Gather the size of the buffer from the CPU-side vector
      #ifdef _MY_OPENGL_IS_33_
        glGenBuffers(1, &m_colVbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_colVbo);
        glBufferData(GL_ARRAY_BUFFER, colorBufferSize, m_vertexNormals.data(), GL_DYNAMIC_READ);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
        glEnableVertexAttribArray(1);
      #else
        glCreateBuffers(1, &m_colVbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_colVbo);
        glNamedBufferStorage(m_colVbo, colorBufferSize, m_ambientColor.data(), GL_DYNAMIC_STORAGE_BIT); // Create a data storage on the GPU and fill it from a CPU array
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
        glEnableVertexAttribArray(1);
      #endif

      // Generate a GPU buffer to store the texture position of the vertices
        size_t texPosBufferSize = sizeof(float)*m_vertexTexCoords.size(); // Gather the size of the buffer from the CPU-side vector
      #ifdef _MY_OPENGL_IS_33_
        glGenBuffers(1, &m_texCoordVbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_texCoordVbo);
        glBufferData(GL_ARRAY_BUFFER, texPosBufferSize, m_vertexTexCoords.data(), GL_DYNAMIC_READ);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
        glEnableVertexAttribArray(2);
      #else
        glCreateBuffers(1, &m_texCoordVbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_texCoordVbo);
        glNamedBufferStorage(m_texCoordVbo, texPosBufferSize, m_vertexTexCoords.data(), GL_DYNAMIC_STORAGE_BIT); // Create a data storage on the GPU and fill it from a CPU array
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
        glEnableVertexAttribArray(2);
      #endif

      // Same for an index buffer object that stores the list of indices of the
      // triangles forming the mesh
        size_t indexBufferSize = sizeof(unsigned int)*m_triangleIndices.size();
      #ifdef _MY_OPENGL_IS_33_
        glGenBuffers(1, &m_normalVbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_normalVbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, m_triangleIndices.data(), GL_DYNAMIC_READ);
      #else
        glCreateBuffers(1, &m_normalVbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_normalVbo);
        glNamedBufferStorage(m_normalVbo, indexBufferSize, m_triangleIndices.data(), GL_DYNAMIC_STORAGE_BIT);
      #endif

        glBindVertexArray(0); // deactivate the VAO for now, will be activated again when rendering
        m_ambientColor = {0.0, 0.5, 1.0};
    }

    void render() { // should be called in the main rendering loop
      const glm::mat4 viewMatrix = g_camera.computeViewMatrix();
      const glm::mat4 projMatrix = g_camera.computeProjectionMatrix();
      const glm::vec3 camPosition = g_camera.getPosition();

      glActiveTexture(GL_TEXTURE0); // activate texture unit 0
      glBindTexture(GL_TEXTURE_2D, m_texID);

      glUniform1i(glGetUniformLocation(g_program, "texture"), textureMode); // compute the display mode of the triangles : 1 for texture, and 0 for uniform color
      glUniform3f(glGetUniformLocation(g_program, "camPos"), camPosition[0], camPosition[1], camPosition[2]); // compute the camera position vector
      glUniform3f(glGetUniformLocation(g_program, "ambient"), m_ambientColor[0], m_ambientColor[1], m_ambientColor[2]); // compute the ambient color matrix
      glUniform3f(glGetUniformLocation(g_program, "lightning"), light[0], light[1], light[2]); // compute the ambient color matrix
      glUniformMatrix4fv(glGetUniformLocation(g_program, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMatrix)); // compute the view matrix of the camera and pass it to the GPU program
      glUniformMatrix4fv(glGetUniformLocation(g_program, "projMat"), 1, GL_FALSE, glm::value_ptr(projMatrix)); // compute the projection matrix of the camera and pass it to the GPU program
      glUniformMatrix4fv(glGetUniformLocation(g_program, "transMat"), 1, GL_FALSE, glm::value_ptr(transformation)); // compute the transformation matrix of the mesh and pass it to the GPU program
      

      glBindVertexArray(m_vao);     // activate the VAO storing geometry data
      glDrawElements(GL_TRIANGLES, m_triangleIndices.size(), GL_UNSIGNED_INT, 0); // Call for rendering: stream the current GPU geometry through the current GPU program
    }

    void addTriangle(std::vector<float> const &verPos) { // Properly add a triangle to the mesh
      assert (verPos.size() >= 9);
      int size = m_vertexPositions.size() / 3;
      std::vector<float> a = {verPos[0], verPos[1], verPos[2]};
      std::vector<float> b = {verPos[3], verPos[4], verPos[5]};
      std::vector<float> c = {verPos[6], verPos[7], verPos[8]};
      std::vector<float> n;
      normal(a, b, c, n);

      for (int i = 0; i < 9; i++) {
        m_vertexPositions.push_back(verPos[i]);
      }
      for (unsigned int i = 0; i < 3; i++) {
        m_triangleIndices.push_back(size + 2 - i);
        m_triangleNormals.push_back(-n[i]);
      }
      
    }

    void setAmbientColor(std::vector<float> amb) {
      m_ambientColor = amb;
    }

    void setTransformation(glm::mat4 &trans) {
      transformation = trans;
    }

    void setTexID(GLuint &texID) {
      m_texID = texID;
      textureMode = 1;
    }
    
    static std::shared_ptr<Mesh> genSphere(size_t const resolution=16) { // should generate a unit sphere
      std::shared_ptr<Mesh> m = std::make_shared<Mesh>();

      const float coef = M_PI/resolution;
      std::vector<float> v;
      v.reserve(9);
      std::vector<float> *point;

      for (float theta = 0.; theta < resolution; theta +=1.) {
        for (float phi = 0.; phi < resolution; phi += 1.) {
          // Creation of a square (two triangles) with space and texture coordinates
          v.clear();
          convertSpherical(1., (theta/resolution*M_PI), (phi/resolution*2*M_PI), v);
          m->m_vertexTexCoords.push_back(phi/resolution);
          m->m_vertexTexCoords.push_back(1 - theta/resolution);
          convertSpherical(1., ((theta+1)/resolution*M_PI), (phi/resolution*2*M_PI), v);
          m->m_vertexTexCoords.push_back(phi/resolution);
          m->m_vertexTexCoords.push_back(1 - (theta+1)/resolution);
          convertSpherical(1., (theta/resolution*M_PI), ((phi+1)/resolution*2*M_PI), v);
          m->m_vertexTexCoords.push_back((phi+1)/resolution);
          m->m_vertexTexCoords.push_back(1 - theta/resolution);

          m->addTriangle(v);

          v.clear();
          convertSpherical(1., ((theta+1)/resolution*M_PI), (phi/resolution*2*M_PI), v);
          m->m_vertexTexCoords.push_back(phi/resolution);
          m->m_vertexTexCoords.push_back(1 - (theta+1)/resolution);
          convertSpherical(1., ((theta+1)/resolution*M_PI), ((phi+1)/resolution*2*M_PI), v);
          m->m_vertexTexCoords.push_back((phi+1)/resolution);
          m->m_vertexTexCoords.push_back(1 - (theta+1)/resolution);
          convertSpherical(1., (theta/resolution*M_PI), ((phi+1)/resolution*2*M_PI), v);
          m->m_vertexTexCoords.push_back((phi+1)/resolution);
          m->m_vertexTexCoords.push_back(1 - theta/resolution);

          m->addTriangle(v);
        }
      }
      randomizeN(0.1, m->m_vertexPositions, m->m_vertexNormals); // Slightly randomize the normal of vertexes
      //m->m_vertexNormals = m->m_vertexPositions;

      return m;

    }
    // ...
  private:
    std::vector<float> m_ambientColor; // Ambient color, if no texture used
    std::vector<float> m_vertexPositions; // Position of all vertexes
    std::vector<float> m_vertexNormals; // Normal of all vertexes
    std::vector<unsigned int> m_triangleIndices; // Indices of vertexes used for each triangles
    std::vector<float> m_triangleNormals; // Normal of all triangles (not necessary for sphere mesh)
    std::vector<float> m_vertexTexCoords; // Coordonates of the vertex in the texture map
    glm::mat4 transformation = glm::mat4(1.0); //Transformation matrix
    GLuint m_colVbo = 0;
    GLuint m_vao = 0;
    GLuint m_posVbo = 0;
    GLuint m_normalVbo = 0;
    GLuint m_texCoordVbo = 0;
    GLuint m_texID = 0; // ID of the texture
    GLuint textureMode = 0; // 0 if the mesh uses an ambient color, 1 if it uses a texture
    // ...
  
};

GLuint loadTextureFromFileToGPU(const std::string &filename) {
  int width, height, numComponents;
  // Loading the image in CPU memory using stb_image
  unsigned char *data = stbi_load(
    filename.c_str(),
    &width, &height,
    &numComponents, // 1 for a 8 bit grey-scale image, 3 for 24bits RGB image, 4 for 32bits RGBA image
    0);

  GLuint texID; // OpenGL texture identifier
  glGenTextures(1, &texID); // generate an OpenGL texture container
  glBindTexture(GL_TEXTURE_2D, texID); // activate the texture
  // Setup the texture filtering option and repeat mode; check www.opengl.org for details.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Fill the GPU texture with the data stored in the CPU image
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

  // Free useless CPU memory
  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0); // unbind the texture

  return texID;
}

// Executed each time the window is resized. Adjust the aspect ratio and the rendering viewport to the current window.
void windowSizeCallback(GLFWwindow* window, int width, int height) {
  g_camera.setAspectRatio(static_cast<float>(width)/static_cast<float>(height));
  glViewport(0, 0, (GLint)width, (GLint)height); // Dimension of the rendering region in the window
}

// Executed each time a key is entered.
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if(action == GLFW_PRESS && key == GLFW_KEY_W) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else if(action == GLFW_PRESS && key == GLFW_KEY_F) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else if(action == GLFW_PRESS && (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)) {
    glfwSetWindowShouldClose(window, true); // Closes the application if the escape key is pressed
  }
}

void errorCallback(int error, const char *desc) {
  std::cout <<  "Error " << error << ": " << desc << std::endl;
}

void initGLFW() {
  glfwSetErrorCallback(errorCallback);

  // Initialize GLFW, the library responsible for window management
  if(!glfwInit()) {
    std::cerr << "ERROR: Failed to init GLFW" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  // Before creating the window, set some option flags
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  // Create the window
  g_window = glfwCreateWindow(
    1024, 768,
    "Interactive 3D Applications (OpenGL) - Simple Solar System",
    nullptr, nullptr);
  if(!g_window) {
    std::cerr << "ERROR: Failed to open window" << std::endl;
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  // Load the OpenGL context in the GLFW window using GLAD OpenGL wrangler
  glfwMakeContextCurrent(g_window);
  glfwSetWindowSizeCallback(g_window, windowSizeCallback);
  glfwSetKeyCallback(g_window, keyCallback);
}

void initOpenGL() {
  // Load extensions for modern OpenGL
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "ERROR: Failed to initialize OpenGL context" << std::endl;
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  glCullFace(GL_BACK); // Specifies the faces to cull (here the ones pointing away from the camera)
  glEnable(GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
  glDepthFunc(GL_LESS);   // Specify the depth test for the z-buffer
  glEnable(GL_DEPTH_TEST);      // Enable the z-buffer test in the rasterization
  glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // specify the background color, used any time the framebuffer is cleared
}

// Loads the content of an ASCII file in a standard C++ string
std::string file2String(const std::string &filename) {
  std::ifstream t(filename.c_str());
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

// Loads and compile a shader, before attaching it to a program
void loadShader(GLuint program, GLenum type, const std::string &shaderFilename) {
  GLuint shader = glCreateShader(type); // Create the shader, e.g., a vertex shader to be applied to every single vertex of a mesh
  std::string shaderSourceString = file2String(shaderFilename); // Loads the shader source from a file to a C++ string
  const GLchar *shaderSource = (const GLchar *)shaderSourceString.c_str(); // Interface the C++ string through a C pointer
  glShaderSource(shader, 1, &shaderSource, NULL); // load the vertex shader code
  glCompileShader(shader);
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "ERROR in compiling " << shaderFilename << "\n\t" << infoLog << std::endl;
  }
  glAttachShader(program, shader);
  glDeleteShader(shader);
}

void initGPUprogram() {
  g_program = glCreateProgram(); // Create a GPU program, i.e., two central shaders of the graphics pipeline
  loadShader(g_program, GL_VERTEX_SHADER, "vertexShader.glsl");
  loadShader(g_program, GL_FRAGMENT_SHADER, "fragmentShader.glsl");
  glLinkProgram(g_program); // The main GPU program is ready to be handle streams of polygons

  glUseProgram(g_program);
  // TODO: set shader variables, textures, etc.
  g_earthTexID = loadTextureFromFileToGPU("media/earth.jpg");
  g_moonTexID = loadTextureFromFileToGPU("media/moon.jpg");
  glUniform1i(glGetUniformLocation(g_program, "material.albedoTex"), 0); // texture unit 0
}

void initCamera() {
  int width, height;
  glfwGetWindowSize(g_window, &width, &height);
  g_camera.setAspectRatio(static_cast<float>(width)/static_cast<float>(height));

  g_camera.setPosition(glm::vec3(0.0, 0.0, 5.0));
  g_camera.setNear(0.1);
  g_camera.setFar(80.1);
}

void init() {
  initGLFW();
  initOpenGL();
  initGPUprogram();
  initCamera();
}

void clear() {
  glDeleteProgram(g_program);

  glfwDestroyWindow(g_window);
  glfwTerminate();
}

// Update any accessible variable based on the current time
void update(const float currentTimeInSec, std::shared_ptr<Mesh> &earth, std::shared_ptr<Mesh> &moon, const float angV = 0.5f) {
  
  float velocity = currentTimeInSec * angV; // Customisable speed of rotation
  
  g_earth = glm::translate(glm::mat4(1), glm::vec3(kRadOrbitEarth * cos(velocity), kRadOrbitEarth * sin(velocity), 0.));  
  g_moon = glm::rotate(g_earth, 4 * velocity, glm::vec3(0, 0, 1));
  g_moon = glm::translate(g_moon, glm::vec3(kRadOrbitMoon, 0., 0.));

  g_earth = glm::rotate(g_earth, glm::radians(23.5f), glm::vec3(0, 0, 1));
  g_earth = glm::rotate(g_earth, 2 * velocity, glm::vec3(0, 0, 1));

  g_earth = glm::scale(g_earth, glm::vec3(kSizeEarth));
  g_moon = glm::scale(g_moon, glm::vec3(kSizeMoon));

  //Apply transformations
  earth->setTransformation(g_earth);
  moon->setTransformation(g_moon);

  //Camera rotation
  g_camera.setPosition(glm::mat3(glm::rotate(glm::mat4(1), velocity, glm::vec3(0, 0, 1))) * glm::vec3(5.0, -10.0, 20.0));
  
}

int main(int argc, char ** argv) {
  init(); // Your initialization code (user interface, OpenGL states, scene with geometry, material, lights, etc)
  std::shared_ptr<Mesh> sun = Mesh::genSphere(25);
  std::shared_ptr<Mesh> earth = Mesh::genSphere(25);
  std::shared_ptr<Mesh> moon = Mesh::genSphere(25);
  
  sun->init();
  earth->init();
  moon->init();

  // Set the colorr / textures
  sun->setAmbientColor({0.8, 0.6, 0.});
  earth->setAmbientColor({0.1, 1., 0.4});
  earth->setTexID(g_earthTexID);
  moon->setAmbientColor({0., 0.4, 1.});
  moon->setTexID(g_moonTexID);

  while(!glfwWindowShouldClose(g_window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Erase the color and z buffers
    update(static_cast<float>(glfwGetTime()), earth, moon); // Update the mesh positions
    sun->render();
    earth->render();
    moon->render();
    glfwSwapBuffers(g_window);
    glfwPollEvents();
  }
  clear();
  return EXIT_SUCCESS;
}
