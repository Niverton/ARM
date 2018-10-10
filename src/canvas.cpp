#include "canvas.h"
#include <iostream> // A ENLEVER !

Canvas::Canvas(const QGLFormat &gl_format, QWidget *parent)
    : QGLWidget(gl_format, parent) {
  setWindowTitle("arm");
}

void Canvas::initializeProgram() {
  program = std::make_unique<QGLShaderProgram>(this->context());
  vertexShader =
      std::make_unique<QGLShader>(QGLShader::Vertex, this->context());
  fragmentShader =
      std::make_unique<QGLShader>(QGLShader::Fragment, this->context());
  if (!fragmentShader->compileSourceFile("../data/fragment.frag")) {
    std::cerr << fragmentShader->log().toStdString() << "\n";
    std::terminate();
  }
  if (!vertexShader->compileSourceFile("../data/vertex.vert")) {
    std::cerr << vertexShader->log().toStdString() << "\n";
    std::terminate();
  }
  program->addShader(vertexShader.get());
  program->addShader(fragmentShader.get());
  program->link();
}

void Canvas::initializeGeometry() {
  vertexArray.push_back({-0.5f, 0.5f, -0.5f});
  vertexArray.push_back({0.5f, 0.5f, -0.5f});
  vertexArray.push_back({0.5f, -0.5f, -0.5f});
  vertexArray.push_back({-0.5f, -0.5f, -0.5f});

  vertexArray.push_back({-0.5f, 0.5f, 0.5f});
  vertexArray.push_back({0.5f, 0.5f, 0.5f});
  vertexArray.push_back({0.5f, -0.5f, 0.5f});
  vertexArray.push_back({-0.5f, -0.5f, 0.5f});

  faceArray.push_back({0, 1, 2});
  faceArray.push_back({0, 2, 3});

  faceArray.push_back({0, 3, 4});
  faceArray.push_back({3, 4, 7});

  faceArray.push_back({4, 5, 7});
  faceArray.push_back({5, 6, 7});

  faceArray.push_back({1, 5, 6});
  faceArray.push_back({1, 2, 6});

  faceArray.push_back({2, 3, 6});
  faceArray.push_back({3, 6, 7});

  faceArray.push_back({0, 1, 4});
  faceArray.push_back({1, 4, 5});

  positionArray.push_back({0, 0, -1});
  positionArray.push_back({0, 0.5, 0.5});
  positionArray.push_back({0.5, 0, 2});
}

void Canvas::initializeGL() {
  makeCurrent();
  initializeProgram();
  initializeGeometry();

  glEnable(GL_DEPTH_TEST);

  glEnableVertexAttribArray(0);
  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3<float>) * vertexArray.size(),
               vertexArray.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

  glGenBuffers(1, &faceBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vec3<short>) * faceArray.size(),
               faceArray.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  glGenBuffers(1, &instanceBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3<float>) * positionArray.size(),
               positionArray.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3<float>),
                        (void *)0);
  glVertexAttribDivisor(1, 1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  mView.lookAt({2, 1, -20}, {0, 0, 0}, {0, 1, 0});
  mObj.setToIdentity();
  mProj.perspective(90.0, 4.0 / 3.0, 0.1, 100.0);
}

void Canvas::resizeGL(int w, int h) {
  std::cout << "resize to " << w << "x" << h << std::endl;
  glViewport(0, 0, w, h);
}

/*void Canvas::drawCube(){

}*/

void Canvas::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  program->bind();

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceBuffer);

  program->setUniformValue("viewMatrix", mView);
  program->setUniformValue("objectMatrix", mObj);
  program->setUniformValue("projectionMatrix", mProj);

  // glDrawElements(GL_TRIANGLES, faceArray.size()*3, GL_UNSIGNED_SHORT, 0);
  glDrawElementsInstanced(GL_TRIANGLES, faceArray.size() * 3, GL_UNSIGNED_SHORT,
                          nullptr, 3);
  // TODO : Declare the VoxelMesh BEFORE calling this function(paintGL())
  /*VoxelMesh vmesh;
  vmesh.drawAsCubes(*this);*/

  glDisableVertexAttribArray(0);
}
