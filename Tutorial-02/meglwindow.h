#ifndef MEGLWINDOW_H
#define MEGLWINDOW_H

#include <QOpenGLWidget>

class MeGLWindow : public QOpenGLWidget {
  Q_OBJECT
public:
  MeGLWindow();

protected:
  void initializeGL() override;
  void paintGL() override;
};

#endif // MEGLWINDOW_H
