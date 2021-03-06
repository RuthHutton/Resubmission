#ifndef WINDOW_H__
#define WINDOW_H__
#include "Fish.h"
#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Transformation.h>
#include <ngl/Text.h>
#include <QOpenGLWindow>
#include <QSet>
#include <memory>
//----------------------------------------------------------------------------------------------------------------------

class Window : public QOpenGLWindow
{

public:

    Window();
    //----------------------------------------------------------------------------------------------------------------------
    ~Window();
    //----------------------------------------------------------------------------------------------------------------------
    void initializeGL();
    //----------------------------------------------------------------------------------------------------------------------
    void paintGL();

private:

    int m_spinXFace;
    //----------------------------------------------------------------------------------------------------------------------
    int m_spinYFace;
    //----------------------------------------------------------------------------------------------------------------------
    bool m_rotate;
    //----------------------------------------------------------------------------------------------------------------------
    bool m_translate;
    //----------------------------------------------------------------------------------------------------------------------
    int m_origX;
    //----------------------------------------------------------------------------------------------------------------------
    int m_origY;
    //----------------------------------------------------------------------------------------------------------------------
    int m_origXPos;
    //----------------------------------------------------------------------------------------------------------------------
    int m_origYPos;
    //----------------------------------------------------------------------------------------------------------------------
    void resizeGL(QResizeEvent *_event);
    void resizeGL(int _w, int _h);
    //----------------------------------------------------------------------------------------------------------------------
    int m_width=1024;
    //----------------------------------------------------------------------------------------------------------------------
    int m_height=720;
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Camera m_cam;
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_modelPos;
    //----------------------------------------------------------------------------------------------------------------------
    int m_updateFishTimer;
    //----------------------------------------------------------------------------------------------------------------------
    int m_redrawTimer;
    //----------------------------------------------------------------------------------------------------------------------
    std::unique_ptr<Fishy> m_fish;
    //----------------------------------------------------------------------------------------------------------------------
    QSet<Qt::Key> m_keysPressed;
    //----------------------------------------------------------------------------------------------------------------------
    void loadMatricesToShader();
    //----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent (QMouseEvent * _event );
    //----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent ( QMouseEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent ( QMouseEvent *_event );
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent( QWheelEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    void keyReleaseEvent(QKeyEvent *_event );
    //----------------------------------------------------------------------------------------------------------------------
    void timerEvent( QTimerEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    void moveFishy();
};

#endif
