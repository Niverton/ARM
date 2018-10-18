#include "canvas.h"
#include <QApplication>
#include <cassert>
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <file>\n";
    return 1;
  }

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

  Canvas pCanvas{&gl_context, argv[1]};
  pCanvas.show();
  pCanvas.resize(640, 480);

  return app.exec();
}
