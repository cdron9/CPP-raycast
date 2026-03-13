#include <cmath>
#include <chrono>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

constexpr int MAP_SIZE = 20;
constexpr int TILE_SIZE = 64;
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 960;

class Player {
public:
    float x, y;
    float dirX, dirY;
    float planeX, planeY;
    float speed, rotSpeed;

    Player(float startX, float startY);

    void handleInput(const bool* keys, float deltaTime, const int map[][MAP_SIZE]);
};

Player::Player(float startX, float startY) {
    x = startX;
    y = startY;
    dirX = 1.0f;
    dirY = 0.0f;
    planeX = 0.0f;
    planeY = 0.66f;
    speed = 300.0f;
    rotSpeed = 3.0f;
}

Player::handleInput(const bool* keys, float deltaTime, const int map[][MAP_SIZE]) {
    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_S]) {
        float moveDir = keys[SDL_SCANCODE_W] ? 1.0f : -1.0f;
        float stepX = dirX * speed * moveDir * deltaTime;
        float stepY = dirY * speed * moveDir * deltaTime;
        if (map[(int)((y+4)/TILE_SIZE)][(int)((x+4+stepX)/TILE_SIZE)] == 0) x += stepX;
        if (map[(int)((y+4+stepY)/TILE_SIZE)][(int)((x+4)/TILE_SIZE)] == 0) y += stepY;
    }
}



int map[MAP_SIZE][MAP_SIZE] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1},
    {1,0,1,1,0,0,0,1,0,0,0,0,0,0,1,0,1,1,0,1},
    {1,0,0,0,0,1,0,1,0,1,1,1,1,0,1,0,0,0,0,1},
    {1,1,1,0,1,1,0,1,0,1,0,0,1,0,1,1,1,0,1,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,0,1,0,0,1,1,1,1,1,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,0,1,1,0,1,1,0,0,1,1,0,1,1,0,1,0,1},
    {1,0,0,0,1,1,0,1,1,0,0,1,1,0,1,1,0,0,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1},
    {1,0,0,0,0,1,0,1,0,0,0,0,1,0,1,0,0,0,0,1},
    {1,0,1,1,0,0,0,1,0,1,1,0,1,0,0,0,1,1,0,1},
    {1,0,1,1,0,1,1,1,0,1,1,0,1,1,1,0,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int main() {
    // initialise SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("CPP Raycast", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);

    bool quit = false;
    SDL_Event e;
    // initialise chrono
    auto lastTime = std::chrono::high_resolution_clock::now();

    while(!quit) {
        // set deltaTime using std::chrono
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        while (SDL_PollEvent(&e)) if (e.type == SDL_EVENT_QUIT) quit = true;

        const bool* keys = SDL_GetKeyboardState(nullptr);

        // handle input
        player.handleInput(keys, deltaTime, map);
        
        // rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_FRect ceiling = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT/2};
        SDL_RenderFillRect(renderer, &ceiling);

        float pCX = player.x + 4;
        float pCY = player.y + 4;

    }
}
    



