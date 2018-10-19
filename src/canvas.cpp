#include "OBJ_Loader.h"
#include "canvas.h"
#include <QSurfaceFormat>
#include <QWheelEvent>
#include <cassert>
#include <iostream> // TODO A ENLEVER !
#include <memory>

Canvas::Canvas(QOpenGLContext *context, std::string file)
    : QOpenGLWindow(context),
      QOpenGLFunctions(context), file_name{std::move(file)},
      program{std::make_unique<QOpenGLShaderProgram>()} {
  // Find mesh type
  // 1: extract file name
  std::size_t fp = file_name.find_last_of('/');
  if (fp == std::string::npos) {
    fp = -1;
  }
  std::string name = file_name.substr(fp + 1);
  // 2 file extension
  fp = name.find_last_of('.');
  if (fp != std::string::npos) {
    std::string ext = name.substr(fp + 1);
    if (ext == "obj") {
      is_voxel = false;
      return;
    }
    if (ext == "pgm3d") {
      is_voxel = true;
      return;
    }
  } // Else error
  std::cerr << file_name << " is neither a .obj nor a .pgm3d file.\n";
  std::exit(1);
}

void Canvas::initializeProgram() {
  const char *vertex_file{nullptr}, *fragment_file{nullptr};
  if (is_voxel) {
    vertex_file = "../data/vertex.vert";
    fragment_file = "../data/fragment.frag";
  } else {
    vertex_file = "../data/mesh.vert";
    fragment_file = "../data/mesh.frag";
  }

  assert(program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertex_file));
  assert(
      program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragment_file));
  assert(program->link());
}

void Canvas::initializeMeshGeometry() {
  using namespace objl;
  Loader obj_loader{};
  if (!obj_loader.LoadFile(file_name)) {
    std::cerr << "Could not load file " << file_name << " as obj\n";
    std::exit(1);
  }
  std::cout << "Found " << obj_loader.LoadedMeshes.size() << " meshes.\n";
  Mesh &mesh = obj_loader.LoadedMeshes[0];
  std::cout << "Loaded mesh " << mesh.MeshName << ".\n";

  vertexArray.reserve(mesh.Vertices.size());
  faceArray.reserve(mesh.Indices.size() / 3);

  auto convert = [](const objl::Vector3 &v) -> Vec3 { return {v.X, v.Y, v.Z}; };
  for (const auto &vertex : mesh.Vertices) {
    vertexArray.emplace_back(convert(vertex.Position));
  }

  std::vector<unsigned int> &faces = mesh.Indices;
  for (unsigned int i = 0; i < faces.size(); i += 3) {
    faceArray.push_back({faces[i], faces[i + 1], faces[i + 2]});
  }

  std::cout << vertexArray.size() << " vertices\n";
  std::cout << faceArray.size() << " faces\n";
}

void Canvas::initializeVoxelGeometry() {
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

  voxelMesh.loadFromPGM3D(file_name);
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

  if (is_voxel) {
    initializeVoxelGeometry();
  } else {
    initializeMeshGeometry();
  }

  glClearColor(0.0, 0.0, 0.0, 1.0);

  // glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);

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
               sizeof(Vec3_base<unsigned int>) * faceArray.size(),
               faceArray.data(), GL_STATIC_DRAW);

  if (is_voxel) {
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
  }

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
  if (is_voxel) {
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceBuffer);

  program->setUniformValue("viewMatrix", mView);
  program->setUniformValue("objectMatrix", mObj);
  program->setUniformValue("projectionMatrix", mProj);
  if (is_voxel) {
    program->setUniformValue("meshBounds", (float)voxelMesh.column,
                             (float)voxelMesh.line, (float)voxelMesh.depth);
    int nb_instance = positionArray.size();

    glDrawElementsInstanced(GL_TRIANGLES, faceArray.size() * 3,
                            GL_UNSIGNED_SHORT, nullptr, nb_instance);
  } else {
    glDrawElements(GL_TRIANGLES, faceArray.size() * 3, GL_UNSIGNED_INT,
                   nullptr);
  }

  glDisableVertexAttribArray(0);
  if (is_voxel) {
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
  }

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cerr << "GL Error (" << error << ") !\n";
  }

  program->release();

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
