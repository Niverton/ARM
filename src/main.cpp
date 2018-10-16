#include "canvas.h"
#include <QApplication>
#include <cassert>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // Context
  QSurfaceFormat format{};
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setVersion(3, 3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QOpenGLContext gl_context{};
  gl_context.setFormat(format);
  gl_context.create();

  assert(gl_context.isValid());

  Canvas pCanvas{&gl_context};
  pCanvas.show();
  pCanvas.resize(640, 480);

  return app.exec();
}
