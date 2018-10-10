#include <iostream>
#include <unistd.h>

#include <QApplication>

#include "canvas.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  
  QGLFormat format{};
  format.setVersion(3, 1);
  format.setProfile(QGLFormat::CoreProfile);
  Canvas *pCanvas = new Canvas(format);
  pCanvas->show();

  pCanvas->resize(640, 480);

  return app.exec();
}
