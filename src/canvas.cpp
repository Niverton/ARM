#include "canvas.h"
#include <QSurfaceFormat>
#include <iostream> // TODO A ENLEVER !
#include <memory>

Canvas::Canvas(QOpenGLContext *context)
    : QOpenGLWindow(context), QOpenGLFunctions(context),
      program{std::make_unique<QOpenGLShaderProgram>()} {}

void Canvas::initializeProgram() {
  program->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                   "../data/vertex.vert");
  program->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                   "../data/fragment.frag");
  // TODO Check for errors
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

  voxelMesh.loadFromPGM3D("../data/shepp_logan.pgm3d");
  positionArray = voxelMesh.position_array;
  intensityArray = voxelMesh.intensity_array;
}

void Canvas::initializeGL() {
  makeCurrent();
  initializeOpenGLFunctions();
  // TODO Debug
  std::cout << "Vendor: \t" << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer:\t" << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Version:\t" << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL Version:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION)
            << std::endl;

  initializeProgram();
  initializeGeometry();
  glClearColor(0.0, 0.0, 0.0, 1.0);

  glEnable(GL_DEPTH_TEST);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

  int nb_vtx_coord = 3;
  glEnableVertexAttribArray(0);
  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * vertexArray.size(),
               vertexArray.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, nb_vtx_coord, GL_FLOAT, GL_FALSE, 0, nullptr);

  glGenBuffers(1, &faceBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(Vec3_base<short>) * faceArray.size(), faceArray.data(),
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  int nb_params = 3;
  glGenBuffers(1, &instanceBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * positionArray.size(),
               positionArray.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, nb_params, GL_FLOAT, GL_FALSE, sizeof(Vec3),
                        nullptr);
  glVertexAttribDivisor(1, 1);

  glEnableVertexAttribArray(2);
  int nb_params_itensity = 1;
  glGenBuffers(1, &instanceBufferIntensity);
  glBindBuffer(GL_ARRAY_BUFFER, instanceBufferIntensity);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * intensityArray.size(),
               intensityArray.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(2, nb_params_itensity, GL_FLOAT, GL_FALSE,
                        sizeof(float), nullptr);
  glVertexAttribDivisor(2, 1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  mView.lookAt({0, 0, -1}, {0, 0, 0}, {0, 1, 0});
  mObj.setToIdentity();
  mProj.perspective(90.0, 4.0 / 3.0, 0.1, 100.0);
}

void Canvas::resizeGL(int w, int h) {
  std::cout << "resize to " << w << "x" << h << std::endl;
  glViewport(0, 0, w, h);
}

void Canvas::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  program->bind();

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceBuffer);

  program->setUniformValue("viewMatrix", mView);
  program->setUniformValue("objectMatrix", mObj);
  program->setUniformValue("projectionMatrix", mProj);
  program->setUniformValue("meshBounds", (float)voxelMesh.column,
                           (float)voxelMesh.line, (float)voxelMesh.depth);
  int nb_instance = positionArray.size();

  glDrawElementsInstanced(GL_TRIANGLES, faceArray.size() * 3, GL_UNSIGNED_SHORT,
                          nullptr, nb_instance);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);

  update();
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
  QPoint dpos = event->pos() - mouse_prev_pos;

  if (event->buttons() & Qt::LeftButton) {
    float angleX = (float)dpos.x();
    float angleY = (float)dpos.y();
    mObj.rotate(angleX, rotateX);
    QMatrix4x4 compensate;
    compensate.rotate(angleX, -rotateX);
    rotateY = compensate * rotateY;
    mObj.rotate(angleY, rotateY);
    compensate.setToIdentity();
    compensate.rotate(angleY, -rotateY);
    rotateX = compensate * rotateX;
  }
  mouse_prev_pos = event->pos();
}

void Canvas::wheelEvent(QWheelEvent *event) {
  int numDegrees = event->delta() / 8;
  mView.translate(0.0, 0.0, -0.01 * numDegrees);
}
