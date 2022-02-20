#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "renderer.h"
#include "model.h"



class GameObject
{
public:
    // object state
    glm::vec3   Position, Scale;
    float       Rotation;
    Model       ObjectModel;
    bool        Destroyed;
    // constructor(s)
    GameObject(const glm::vec3 &Position,const glm::vec3 &Scale,const float Rotation, string const &path): ObjectModel(path), Destroyed(false)
    {
        this->Position = Position;
        this->Scale = Scale;
        this->Rotation = Rotation;
    }
    // draw sprite
    void Draw(Renderer &renderer, Camera &camera)
    {
        renderer.Draw(camera,ObjectModel, this->Position, this->Scale, this->Rotation);
    }
};

#endif