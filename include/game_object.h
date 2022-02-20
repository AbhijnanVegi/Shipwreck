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
    glm::vec3   Position, Scale, Rotation;
    Model       ObjectModel;
    bool        Destroyed;
    // constructor(s)
    GameObject(glm::vec3 Postion, glm::vec3 Scale, glm::vec3 Rotation, string const &path): Position(Position), Scale(Scale), Rotation(Rotation), ObjectModel(path)
    {
    }
    // draw sprite
    void Draw(Renderer &renderer);
};

#endif