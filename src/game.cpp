#include <iostream>
#include <string>
#include <random>
#include <time.h>

#include "game.h"
#include "game_object.h"
#include "renderer.h"
#include "text_renderer.h"

Renderer *Ren;
TextRenderer *Text;

GameObject *Player;
GameObject *Water;

float angle(glm::vec3 a, glm::vec3 b);
void handlePlayerChestCollisions(vector<GameObject> &Collectibles, unsigned int &Score);
void handlePlayerProjectileCollisions(vector<Projectile> &Projectiles, int &HP);
void handleEnemyProjectileCollisions(vector<Projectile> &Projectiles, vector<GameObject> &Enemies, unsigned int &Score);
void generateChests(vector<GameObject> &Collectibles);
void generateEnemies(vector<GameObject> &Enemies);
void updateEnemies(vector<GameObject> &Enemies, float dt, vector<Projectile> &Projectiles);

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Width(width), Height(height), Score(0), HP(100), Time(0.0f)
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
    srand(time(0));
    Shader ModelShader = Shader("shaders/model.vs", "shaders/model.fs");
    Ren = new Renderer(ModelShader);
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("fonts/gameplay.ttf", 20);

    Player = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), 180.0f, "models/watercraftPack_023.obj");
    Water = new GameObject(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(10.0f, 10.0f, 10.0f), 0.0f, "models/water2.obj");
    Cam = Camera(glm::vec3(0.0f, 2.0f, 3.0f));

    generateEnemies(Enemies);
    generateChests(Collectibles);
}

void Game::Render()
{
    if (State == GAME_MENU)
    {
        Text->RenderText("SHIPWRECK", this->Width / 2 - 150, this->Height / 2 - 100, 3, glm::vec3(1.0f, 1.0f, 1.0f));
        Text->RenderText("Press Enter to Start", this->Width / 2 - 100, this->Height / 2, 1, glm::vec3(1.0f, 1.0f, 1.0f));
    }
    if (State == GAME_ACTIVE)
    {
        Water->Draw(*Ren, Cam);
        Player->Draw(*Ren, Cam);
        Text->RenderText("Score: " + std::to_string(this->Score), 5.0f, 5.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
        Text->RenderText("Health: " + std::to_string(this->HP), 5.0f, 25.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Text->RenderText("Time: " + std::to_string((int)this->Time), 5.0f, 45.0f, 1.0f, glm::vec3(1.0f, 0.0f, 1.0f));
        for (int i = 0; i < Enemies.size(); i++)
        {
            if (glm::length(Enemies[i].Position) < 20.0f)
                Enemies[i].Draw(*Ren, Cam);
            else
                Enemies.erase(Enemies.begin() + i);
        }
        for (int i = 0; i < Collectibles.size(); i++)
        {
            if (glm::length(Collectibles[i].Position) < 20.0f)
                Collectibles[i].Draw(*Ren, Cam);
            else
            {
                Collectibles.erase(Collectibles.begin() + i);
            }
        }
        for (int i = 0; i < Projectiles.size(); i++)
        {
            Projectiles[i].Draw(*Ren, Cam);
        }
    }
    if (State == GAME_OVER)
    {
        Text->RenderText("GAME OVER", this->Width / 2 - 150, this->Height / 2 - 100, 3, glm::vec3(1.0f, 1.0f, 1.0f));
        Text->RenderText("Score: " + std::to_string(this->Score), this->Width / 2 - 10, this->Height / 2, 1, glm::vec3(1.0f, 1.0f, 1.0f));
        Text->RenderText("Time  : " + std::to_string((int)this->Time), this->Width / 2 - 10, this->Height / 2 + 20, 1, glm::vec3(1.0f, 1.0f, 1.0f));
        Text->RenderText("Press Enter to Exit", this->Width / 2 - 75, this->Height / 2 + 50, 1, glm::vec3(1.0f, 1.0f, 1.0f));
    }
}

void Game::ProcessInput(float dt)
{
    const float player_w = 40.0f;
    const float player_v = 3.0f;
    glm::mat4 rot = glm::mat3(1.0f);
    if (Keys[GLFW_KEY_A])
    {
        float d = player_w * dt;
        rot = glm::rotate(rot, glm::radians(d), glm::vec3(0, 1, 0));
        Cam.Look = glm::vec3(rot * glm::vec4(Cam.Look, 1.0f));
        Cam.Position = glm::vec3(rot * glm::vec4(Cam.Position, 1.0f));
        Player->Rotation += d;
    }
    else if (Keys[GLFW_KEY_D])
    {
        float d = -1 * player_w * dt;
        rot = glm::rotate(rot, glm::radians(d), glm::vec3(0, 1, 0));
        Cam.Look = glm::vec3(rot * glm::vec4(Cam.Look, 1.0f));
        Cam.Position = glm::vec3(rot * glm::vec4(Cam.Position, 1.0f));
        Player->Rotation += d;
    }

    if (Keys[GLFW_KEY_W])
    {
        float d = player_v * dt;
        glm::vec3 lookDir = glm::normalize(Cam.Look - Cam.Position);
        glm::vec3 direction = glm::normalize(glm::vec3(lookDir.x, 0, lookDir.z));
        for (int i = 0; i < Enemies.size(); i++)
        {
            Enemies[i].Position -= direction * d;
        }
        for (int i = 0; i < Collectibles.size(); i++)
        {
            Collectibles[i].Position -= direction * d;
        }
        for (int i = 0; i < Projectiles.size(); i++)
        {
            Projectiles[i].Position -= direction * d;
        }
    }
    if (Keys[GLFW_KEY_S])
    {
        float d = -1 * player_v * dt;
        glm::vec3 lookDir = glm::normalize(Cam.Look - Cam.Position);
        glm::vec3 direction = glm::normalize(glm::vec3(lookDir.x, 0, lookDir.z));
        for (int i = 0; i < Enemies.size(); i++)
        {
            Enemies[i].Position -= direction * d;
        }
        for (int i = 0; i < Collectibles.size(); i++)
        {
            Collectibles[i].Position -= direction * d;
        }
        for (int i = 0; i < Projectiles.size(); i++)
        {
            Projectiles[i].Position -= direction * d;
        }
    }
}

void Game::Update(float dt)
{
    if (State != GAME_ACTIVE)
        return;
    handlePlayerChestCollisions(Collectibles, Score);
    handlePlayerProjectileCollisions(Projectiles, HP);
    handleEnemyProjectileCollisions(Projectiles, Enemies, Score);
    if (HP <= 0)
    {
        State = GAME_OVER;
    }
    if (Collectibles.size() == 0)
    {
        generateChests(Collectibles);
    }
    if (Enemies.size() < 2)
    {
        generateEnemies(Enemies);
    }
    for (int i = 0; i < Projectiles.size(); i++)
    {
        if (glm::length(Projectiles[i].Position) > 20.0f)
        {
            Projectiles.erase(Projectiles.begin() + i);
        }
        else
        {
            Projectiles[i].Update(dt);
        }
    }

    updateEnemies(Enemies, dt, Projectiles);
    Time += dt;
}

void Game::ToggleCamera()
{
    if (Cam.Position.y == 2.0f)
        Cam.Position.y = 10.0f;
    else
        Cam.Position.y = 2.0f;
}

void Game::Shoot()
{
    if (Player->Cooldown <= 0)
    {
        Projectiles.push_back(Projectile(Cam.Look * 0.5f, glm::vec3(0.02f, 0.02f, 0.02f), 0.0f, "models/sphere.obj", glm::normalize(Cam.Look)));
    }
}

float angle(glm::vec3 a, glm::vec3 b)
{
    return glm::degrees(glm::acos(glm::dot(a, b) / (glm::length(a) * glm::length(b))));
}

void generateChests(vector<GameObject> &Collectibles)
{
    for (int i = 0; i < 3; i++)
    {
        float x = ((rand() % 2 == 0) ? -1 : 1) * ((rand() % 7) + 5);
        float z = ((rand() % 2 == 0) ? -1 : 1) * ((rand() % 7) + 5);
        GameObject *Chest = new GameObject(glm::vec3(x, -0.1f, z), glm::vec3(0.05f, 0.05f, 0.05f), 270.0f, "models/chest.obj");
        Collectibles.push_back(*Chest);
    }
}

void generateEnemies(vector<GameObject> &Enemies)
{
    for (int i = 0; i < 3; i++)
    {
        float x = ((rand() % 2 == 0) ? -1 : 1) * ((rand() % 7) + 7);
        float z = ((rand() % 2 == 0) ? -1 : 1) * ((rand() % 7) + 7);
        glm::vec3 Pos = glm::vec3(x, 0.0f, z);
        float theta = angle(Pos, glm::vec3(0.0f, 0.0f, -1.0f)) * (Pos.x < 0 ? 1 : -1);
        GameObject *Enemy = new GameObject(Pos, glm::vec3(0.2, 0.2, 0.2), theta, "models/watercraftPack_008.obj");
        Enemies.push_back(*Enemy);
    }
}

void updateEnemies(vector<GameObject> &Enemies, float dt, vector<Projectile> &Projectiles)
{
    const float enemy_v = 2.0f;
    for (int i = 0; i < Enemies.size(); i++)
    {
        glm::vec3 lookDir = glm::normalize(Enemies[i].Position - Player->Position);
        glm::vec3 direction = glm::normalize(glm::vec3(lookDir.x, 0, lookDir.z));
        float theta = angle(Enemies[i].Position, glm::vec3(0.0f, 0.0f, -1.0f)) * (Enemies[i].Position.x < 0 ? 1 : -1);
        Enemies[i].Rotation = theta;

        bool collision = false;
        // Prevent collisions
        for (int j = 0; j < Enemies.size(); j++)
        {
            if (i != j && glm::length(Enemies[i].Position - Enemies[j].Position) < 0.5f)
            {
                collision = true;
                break;
            }
        }

        if (!collision && glm::length(Enemies[i].Position) > 5.0f)
            Enemies[i].Position -= direction * enemy_v * dt;
        else
        {
            if (Enemies[i].Cooldown <= 0.0f)
            {
                Enemies[i].Cooldown = 4.0f;
                Projectile *ball = new Projectile(Enemies[i].Position  - glm::normalize(Enemies[i].Position) * 0.5f, glm::vec3(0.02f, 0.02f, 0.02f), 0.0f, "models/sphere.obj", -glm::normalize(Enemies[i].Position));
                Projectiles.push_back(*ball);
            }
        }

        if (Enemies[i].Cooldown > 0.0f)
            Enemies[i].Cooldown -= dt;
    }
}

bool CheckCollision(GameObject &a)
{
    bool onX = a.Position.x < 0.25f && a.Position.x > -0.25f;
    bool onZ = a.Position.z < 0.9f && a.Position.z > -0.9f;
    return onX && onZ;
}

void handlePlayerChestCollisions(vector<GameObject> &Collectibles, unsigned int &Score)
{
    for (int i = 0; i < Collectibles.size(); i++)
    {
        bool onX = Collectibles[i].Position.x < 0.25 && Collectibles[i].Position.x > -0.25f;
        bool onZ = Collectibles[i].Position.z < 0.9f && Collectibles[i].Position.z > -0.9f;
        if (onX && onZ)
        {
            Collectibles.erase(Collectibles.begin() + i);
            Score += 10;
            cout << "Score: " << Score << endl;
        }
    }
}

void handlePlayerProjectileCollisions(vector<Projectile> &Projectiles, int &HP)
{
    for (int i = 0; i < Projectiles.size(); i++)
    {
        bool onX = Projectiles[i].Position.x < 0.25 && Projectiles[i].Position.x > -0.25f;
        bool onZ = Projectiles[i].Position.z < 0.9f && Projectiles[i].Position.z > -0.9f;
        if (onX && onZ)
        {
            Projectiles.erase(Projectiles.begin() + i);
            HP -= 10;
        }
    }
}

void handleEnemyProjectileCollisions(vector<Projectile> &Projectiles, vector<GameObject> &Enemies, unsigned int &Score)
{
    for (int i = 0; i < Projectiles.size(); i++)
    {
        for (int j = 0; j < Enemies.size();j++)
        {
            bool onX = Projectiles[i].Position.x < Enemies[j].Position.x + 0.25 && Projectiles[i].Position.x > Enemies[j].Position.x - 0.25f;
            bool onZ = Projectiles[i].Position.z < Enemies[j].Position.z + 0.25 && Projectiles[i].Position.z > Enemies[j].Position.z - 0.25f;
            if (onX && onZ)
            {
                Projectiles.erase(Projectiles.begin() + i);
                Enemies.erase(Enemies.begin() + j);
                Score += 10;
            }
        }
    }
}