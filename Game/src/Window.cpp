#include "Window.h"
#include <iostream>
#include <QMouseEvent>
#include <QGuiApplication>
//#include <QApplication>
#include <ngl/NGLInit.h>
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Transformation.h>
#include <ngl/Material.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>

//----------------------------------------------------------------------------------------------------------------------

constexpr static float s_fishUpdate=0.2;

const static float INCREMENT=0.01;

const static float ZOOM=0.1;

//----------------------------------------------------------------------------------------------------------------------

Window::Window()
{
    m_rotate=false;
    m_spinXFace=0;
    m_spinYFace=0;

    setTitle("Ruth's Window");

    m_updateFishTimer=startTimer(2);
    m_redrawTimer=startTimer(20);

    m_keysPressed.clear();
}

//----------------------------------------------------------------------------------------------------------------------

Window::~Window()
{
    std::cout<<"Shutting down\n";
}

//----------------------------------------------------------------------------------------------------------------------

void Window::resizeGL(QResizeEvent *_event)
{
    m_width=_event->size().width()*devicePixelRatio();
    m_height=_event->size().height()*devicePixelRatio();

    m_cam.setShape(45.0f,(float)width()/height(),0.05f,350.0f);
}
//----------------------------------------------------------------------------------------------------------------------

void Window::resizeGL(int _w , int _h)
{
  m_cam.setShape(45.0f,(float)_w/_h,0.05f,350.0f);
  m_width=_w*devicePixelRatio();
  m_height=_h*devicePixelRatio();
}
//----------------------------------------------------------------------------------------------------------------------

void Window::initializeGL()
{
    ngl::NGLInit::instance();
    glClearColor(0.3f, 0.6f, 0.6f, 0.2f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    shader->createShaderProgram("Phong");
    shader->attachShader("PhongVertex",ngl::ShaderType::VERTEX);
    shader->attachShader("PhongFragment",ngl::ShaderType::FRAGMENT);
    shader->loadShaderSource("PhongVertex","shaders/PhongVertex.glsl");
    shader->loadShaderSource("PhongFragment","shaders/PhongFragment.glsl");
    shader->compileShader("PhongVertex");
    shader->compileShader("PhongFragment");
    shader->attachShaderToProgram("Phong","PhongVertex");
    shader->attachShaderToProgram("Phong","PhongFragment");


    shader->linkProgramObject("Phong");
    (*shader)["Phong"]->use();

    ngl::Vec3 from(0,0,-80);
    ngl::Vec3 to(0,0,0);
    ngl::Vec3 up(0,1,0);
    m_cam.set(from,to,up);

    m_cam.setShape(45,(float)720.0/576.0,0.05,350);
    shader->setShaderParam3f("viewerPos",m_cam.getEye().m_x,m_cam.getEye().m_y,m_cam.getEye().m_z);

    ngl::Mat4 iv=m_cam.getViewMatrix();
    iv.transpose();
    ngl::Light light(ngl::Vec3(0,0,2),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::LightModes::DIRECTIONALLIGHT);
    light.setTransform(iv);
    light.loadToShader("light");

    ngl::Material m(ngl::STDMAT::GOLD);
    m.loadToShader("material");
    // create fish
    m_fish.reset( new Fishy(ngl::Vec3(0,0,0),"models/Fish.obj"));
}
//----------------------------------------------------------------------------------------------------------------------


void Window::loadMatricesToShader()
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    ngl::Mat4 MVP=m_cam.getVPMatrix()  ;
    shader->setShaderParamFromMat4("MVP",MVP);
}
//----------------------------------------------------------------------------------------------------------------------

void Window::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_fish->draw("Phong",&m_cam);
}


//void Window::paintGL()
//{
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//  glViewport(0,0,m_width,m_height);
//}

//----------------------------------------------------------------------------------------------------------------------
void Window::mouseMoveEvent (QMouseEvent * _event)
{
    if(m_rotate && _event->buttons() == Qt::LeftButton)
      {
        int diffx=_event->x()-m_origX;
        int diffy=_event->y()-m_origY;
        m_spinXFace += (float) 0.5f * diffy;
        m_spinYFace += (float) 0.5f * diffx;
        m_origX = _event->x();
        m_origY = _event->y();
        update();

      }

    else if(m_translate && _event->buttons() == Qt::RightButton)
    {
      int diffX = (int)(_event->x() - m_origXPos);
      int diffY = (int)(_event->y() - m_origYPos);
      m_origXPos=_event->x();
      m_origYPos=_event->y();
      m_modelPos.m_x += INCREMENT * diffX;
      m_modelPos.m_y -= INCREMENT * diffY;
      update();

     }
}

//----------------------------------------------------------------------------------------------------------------------
void Window::mousePressEvent ( QMouseEvent * _event)
{
    if(_event->button() == Qt::LeftButton)
      {
        m_origX = _event->x();
        m_origY = _event->y();
        m_rotate =true;
      }
    else if(_event->button() == Qt::RightButton)
      {
        m_origXPos = _event->x();
        m_origYPos = _event->y();
        m_translate=true;
      }
}

//----------------------------------------------------------------------------------------------------------------------
void Window::mouseReleaseEvent ( QMouseEvent * _event )
{
    if (_event->button() == Qt::LeftButton)
      {
        m_rotate=false;
      }

    if (_event->button() == Qt::RightButton)
      {
        m_translate=false;
      }
}

//----------------------------------------------------------------------------------------------------------------------
void Window::wheelEvent(QWheelEvent *_event)
{
    if(_event->delta() > 0)
        {
            m_modelPos.m_z+=ZOOM;
        }
    else if(_event->delta() <0 )
        {
            m_modelPos.m_z-=ZOOM;
        }
        update();
}
//----------------------------------------------------------------------------------------------------------------------

void Window::timerEvent( QTimerEvent *_event)
{
    if (_event->timerId() == m_updateFishTimer)
        {
            moveFishy();
        }
    if (_event->timerId() == m_redrawTimer)
        {
            update();
        }

}
//----------------------------------------------------------------------------------------------------------------------


void Window::moveFishy()
{
    /// first we reset the movement values
    float xDirection=0.0;
    float yDirection=0.0;
    float rotation=0.0;

    foreach(Qt::Key key, m_keysPressed)
    {
        switch (key)
        {
            case Qt::Key_Left :  { xDirection=s_fishUpdate; break;}
            case Qt::Key_Right : { xDirection=-s_fishUpdate; break;}
            case Qt::Key_Up :	 { yDirection=s_fishUpdate; break;}
            case Qt::Key_Down :  { yDirection=-s_fishUpdate; break;}
            case Qt::Key_R :     { rotation=1.0; break;}
            default : break;
        }
    }
    if(m_keysPressed.size() !=0)
    {
        m_fish->move(xDirection,yDirection);
        m_fish->rotate(rotation);
    }
}
//----------------------------------------------------------------------------------------------------------------------


void Window::keyPressEvent(QKeyEvent *_event)
{

        m_keysPressed += (Qt::Key)_event->key();

  switch (_event->key())
  {
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;

  default : break;
  }

    update();
}
//----------------------------------------------------------------------------------------------------------------------


void Window::keyReleaseEvent( QKeyEvent *_event	)
{
    m_keysPressed -= (Qt::Key)_event->key();
}

