#include "canvas.h"
#include <iostream> // A ENLEVER !

Canvas::Canvas(QWidget *parent) : QGLWidget(parent) { setWindowTitle("arm"); }

void Canvas::initializeGL() {
  vertexArray.push_back({1.0f, 1.0f, 0.0f});
  vertexArray.push_back({-1.0f, -1.0f, 0.0f});
  vertexArray.push_back({1.0f, -1.0f, 0.0f});

  faceArray.push_back({0, 1, 2});

  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3<float>) * vertexArray.size(),
               vertexArray.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &faceBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vec3<short>) * faceArray.size(),
               faceArray.data(), GL_STATIC_DRAW);
}

void Canvas::resizeGL(int w, int h) { 
  std::cout << "resize to " << w << "x" << h <<std::endl;
  glViewport(0, 0, w, h); 
}

void Canvas::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceBuffer);

  glDrawElements(GL_TRIANGLES, 3 * faceArray.size(), GL_SHORT, 0);

  glDisableVertexAttribArray(0);
}

void Canvas::timeOutSlot(){

}
