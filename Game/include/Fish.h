#ifndef FISH_H__
#define FISH_H__

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Obj.h>
#include <ngl/Transformation.h>

class Fishy
{
public :

       Fishy(ngl::Vec3 _pos, std::string _fname);

       void draw(const std::string &_shader, ngl::Camera *_cam );

       void move(float _x,	float _y	);

       void rotate(float _rot);

private :

        ngl::Vec3 m_pos;

        std::unique_ptr<ngl::Obj>m_mesh;

        const static float s_increment;

        ngl::Transformation m_transform;

        float m_rotation;
};

#endif

