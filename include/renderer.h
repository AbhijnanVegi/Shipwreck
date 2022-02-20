#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "model.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"


class Renderer
{
public:
   Renderer(const Shader &shader);
   ~Renderer();
   void Draw(Model &model, glm::vec3(position), glm::vec3(scale), float angle);
   void UpdateCamera(const Camera cam);

   private:
   Shader shader; 
};

#endif