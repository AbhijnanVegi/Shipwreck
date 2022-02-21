#ifndef GAME_H
#define GAME_H

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "game_object.h"

enum GameState
{
    GAME_MENU,
    GAME_ACTIVE,
    GAME_WIN,
    GAME_OVER
};

class Game
{
    public:
        GameState State;
        bool Keys[1024];
        unsigned int Width, Height;
        unsigned int Score;
        Camera Cam;
        std::vector<GameObject> Enemies;
        std::vector<GameObject> Projectiles;
        std::vector<GameObject> Collectibles;

        Game(unsigned int width, unsigned int height);
        ~Game();

        void Init();
        void ProcessInput(float dt);
        void Update(float dt);
        void Render();
        void ToggleCamera();
};

#endif