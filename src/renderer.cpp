#include "renderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

Renderer::Renderer(const Shader &shader): shader(shader)
{
}

Renderer::~Renderer()
{
}

void Renderer::Draw(Model &model, glm::vec3(position), glm::vec3(scale), float angle)
{
    this->shader.use();
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, position);

    modelMat = glm::rotate(modelMat, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMat = glm::scale(modelMat, scale);

    this->shader.setMat4("model", modelMat);
    model.Draw(this->shader);
}
