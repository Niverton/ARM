#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QGLWidget(parent) { setWindowTitle("arm"); }

void Canvas::initializeGL() {
  vertexArray.emplace_back({1, 1, 0});
  vertexArray.emplace_back({-1, -1, 0});
  vertexArray.emplace_back({1, -1, 0});

  faceArray.emplace_back({0, 1, 2});

  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3<unsigned int>) * vertexArray.size(),
               vertexArray.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &faceBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vec3<short>), faceArray.data(),
               GL_STATIC_DRAW);
}

void Canvas::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void Canvas::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceBuffer);

  glDrawElements(GL_TRIANGLES, 3 * faceArray.size(), GL_SHORT, 0);
}
