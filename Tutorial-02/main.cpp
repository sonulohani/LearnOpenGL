#include "meglwindow.h"
#include <QApplication>
#include <QResource>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QResource::registerResource("resource.qrc");

  MeGLWindow meWindow;
  meWindow.show();

  return a.exec();
}
