#include "Fish.h"
#include <ngl/ShaderLib.h>

Fishy::Fishy(ngl::Vec3 _pos, std::string _fname)
{
    m_pos = _pos;
    m_mesh.reset( new ngl::Obj(_fname));
    m_mesh->createVAO();
    m_rotation = 0;
}
//----------------------------------------------------------------------------------------------------------------------

void Fishy::draw(const std::string &_shader, ngl::Camera *_cam)
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    (*shader)[_shader]->use();
    m_transform.setPosition(m_pos);
    m_transform.setRotation(0, m_rotation, 0);
    ngl::Mat4 MVP = m_transform.getMatrix() * _cam -> getVPMatrix();
    shader -> setShaderParamFromMat4("MVP", MVP);

    m_mesh -> draw();
}

const static float s_xExtents=45;
const static float s_yExtents=30;
//----------------------------------------------------------------------------------------------------------------------

void Fishy::move( float _x,float _y )
{
    float currentX=m_pos.m_x;
    float currentY=m_pos.m_y;
    m_pos.m_x+=_x;
    m_pos.m_y+=_y;
    if(m_pos.m_x<=-s_xExtents || m_pos.m_x>=s_xExtents)
    {
        m_pos.m_x=currentX;
    }


    if(m_pos.m_y<=-s_yExtents || m_pos.m_y>=s_yExtents)
    {
        m_pos.m_y=currentY;
    }
}
//----------------------------------------------------------------------------------------------------------------------

void Fishy::rotate(float _rot )
{
    m_rotation+=_rot;
}
