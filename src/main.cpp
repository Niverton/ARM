#include <iostream>
#include <unistd.h>

#include <QApplication>

#include "canvas.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QWidget window;
  window.resize(320, 240);
  window.show();
  window.setWindowTitle(
      QApplication::translate("toplevel", "Top-level widget"));

  QGLFormat format{};
  format.setVersion(3, 3);
  format.setProfile(QGLFormat::CoreProfile);
  Canvas *pCanvas = new Canvas(format, &window);

  // Taille fixé en dur, à modifier si on a le temps
  pCanvas->resize(640, 480);
  window.resize(640, 480);

  pCanvas->show();

  return app.exec();
}
