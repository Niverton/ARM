#ifndef CANVAS_H
#define CANVAS_H

#define GL_GLEXT_PROTOTYPES
#include "voxel_mesh.h"
#include <QGLFormat>
#include <QGLShader>
#include <QGLShaderProgram>
#include <QGLWidget>
#include <QMatrix4x4>
#include <cstdlib>
#include <memory>
#include "vec.h"

class Canvas : public QGLWidget {
  Q_OBJECT
public:
  explicit Canvas(const QGLFormat &gl_format, QWidget *parent = 0);
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();
  std::vector<Vec3> vertexArray;
  std::vector<Vec3_base<unsigned short>> faceArray;
  std::unique_ptr<QGLShaderProgram> program;
  // virtual void keyPressEvent( QKeyEvent *keyEvent );

private:
  void initializeProgram();
  void initializeGeometry();

  unsigned int vertexBuffer, faceBuffer, instanceBuffer, instanceBufferIntensity;
  std::vector<Vec3> positionArray;
  std::vector<float> intensityArray;
  std::unique_ptr<QGLShader> vertexShader, fragmentShader;
  QMatrix4x4 mView, mProj, mObj;
  VoxelMesh voxelMesh;
};

#endif
