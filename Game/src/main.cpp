#include <QtGui/QGuiApplication>
#include <iostream>
#include "Window.h"

int main(int argc, char **argv)
{
  QGuiApplication app(argc, argv);
  QSurfaceFormat format;

  format.setSamples(4);
  #if defined( __APPLE__)

    format.setMajorVersion(4);
    format.setMinorVersion(1);
  #else
    format.setMajorVersion(4);
    format.setMinorVersion(3);
  #endif
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setDepthBufferSize(24);
  Window window;
  window.setFormat(format);
  std::cout<<"Profile is "<<format.majorVersion()<<" "<<format.minorVersion()<<"\n";
  window.resize(1024, 720);
  window.show();

  return app.exec();
}
