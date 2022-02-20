#include "renderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer(const Shader &shader): shader(shader)
{
}

Renderer::~Renderer()
{
}

void Renderer::Draw(Camera &camera,Model &model, glm::vec3 &position, glm::vec3 &scale, float angle)
{
    this->shader.use();

    glm::mat4 view = camera.GetViewMatrix();
    this->shader.setMat4("view", view);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    this->shader.setMat4("projection", projection);

    glm::mat4 modelMat = glm::mat4(1.0f);
    glm::vec3 pos = position;
    modelMat = glm::translate(modelMat, pos);
    modelMat = glm::rotate(modelMat, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMat = glm::scale(modelMat, scale);
    this->shader.setMat4("model", modelMat);

    model.Draw(this->shader);
}
