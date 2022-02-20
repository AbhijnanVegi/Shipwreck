#include <iostream>
#include <string>

#include "game.h"
#include "game_object.h"
#include "renderer.h"
#include "text_renderer.h"

Renderer *Ren;
TextRenderer *Text;

GameObject *Player;
GameObject *Water;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Width(width), Height(height), Score(0)
{   
}

Game::~Game()
{
    delete Ren;
    delete Text;
    delete Player;
}

void Game::Init()
{
   Shader ModelShader = Shader("shaders/model.vs", "shaders/model.fs");
   Ren = new Renderer(ModelShader); 
   Text = new TextRenderer(this->Width, this->Height);
   Text->Load("fonts/gameplay.ttf", 24);

    Player = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f),180.0f, "models/watercraftPack_023.obj");
    Water = new GameObject(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(10.0f, 10.0f, 10.0f), 0.0f, "models/water2.obj");
    Cam = Camera(glm::vec3(0.0f, 2.0f, 3.0f));
}

void Game::Render()
{
    Water->Draw(*Ren, Cam);
    Player->Draw(*Ren, Cam);
}

void Game::ProcessInput(float dt)
{
    const float player_w = 20.0f;
    glm::mat4 rot = glm::mat3(1.0f);
    if (Keys[GLFW_KEY_A])
    {
        float d = player_w * dt;
        rot = glm::rotate(rot, glm::radians(d), glm::vec3(0, 1, 0));
        Cam.Look = glm::vec3(rot * glm::vec4(Cam.Look, 1.0f));
        Cam.Position = glm::vec3(rot * glm::vec4(Cam.Position, 1.0f));
        Player->Rotation += d;
    }
    if (Keys[GLFW_KEY_D])
    {
        float d = - player_w * dt;
        rot = glm::rotate(rot, glm::radians(d), glm::vec3(0, 1, 0));
        Cam.Look = glm::vec3(rot * glm::vec4(Cam.Look, 1.0f));
        Cam.Position = glm::vec3(rot * glm::vec4(Cam.Position, 1.0f));
        Player->Rotation += d;
    }
}

void Game::Update(float dt)
{

}