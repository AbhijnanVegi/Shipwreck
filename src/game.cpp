#include <iostream>
#include <string>

#include "game.h"
#include "game_object.h"
#include "renderer.h"
#include "text_renderer.h"

Renderer *Renderer;
TextRenderer *Text;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Width(width), Height(height), Score(0)
{   
}

Game::~Game()
{
    delete Renderer;
    delete Text;
}

void Game::Init()
{
    GameObject *Player = new GameObject(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        "models/watercraftPack_001.obj"
    );
}