#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "model.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

class Renderer
{
public:
   Renderer(const Shader &shader);
   ~Renderer();
   void Draw(Camera &camera, Model &model, glm::vec3 &position, glm::vec3 &scale, float angle);

   private:
   Shader shader; 
};

#endif