#ifndef CANVAS_H
#define CANVAS_H

#define GL_GLEXT_PROTOTYPES
#include "vec.h"
#include "voxel_mesh.h"
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLWindow>
#include <memory>

class Canvas : public QOpenGLWindow, protected QOpenGLFunctions {
  Q_OBJECT
public:
  Canvas(QOpenGLContext *context, std::string file);
  void initializeGL() override;
  void resizeGL(int width, int height) override;
  void paintGL() override;

  // TODO
  // virtual void keyPressEvent( QKeyEvent *keyEvent );

private:
  void initializeProgram();
  void initializeVoxelGeometry();
  void initializeMeshGeometry();
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

  std::string file_name;
  bool is_voxel = false;

  std::unique_ptr<QOpenGLContext> gl_context;
  std::unique_ptr<QOpenGLShaderProgram> program;

  unsigned int vertexBuffer, faceBuffer, instanceBuffer,
      instanceBufferIntensity;
  std::vector<Vec3> vertexArray;
  std::vector<Vec3_base<unsigned int>> faceArray;
  std::vector<Vec3> positionArray;
  std::vector<float> intensityArray;
  QMatrix4x4 mView, mProj, mObj;
  VoxelMesh voxelMesh;
  QPoint mouse_prev_pos;
  QVector3D rotateX{0.0, 1.0, 0.0}, rotateY{1.0, 0.0, 0.0};
};

#endif
