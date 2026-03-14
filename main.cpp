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

void Player::handleInput(const bool* keys, float deltaTime, const int map[][MAP_SIZE]) {
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
    Player player(100.0f, 100.0f);
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

        for (int i = 0; i < SCREEN_WIDTH; i++) {
            float cameraX = 2.0f * i / (float)SCREEN_WIDTH - 1.0f;
            float rayDX = player.dirX + player.planeX * cameraX;
            float rayDY = player.dirY + player.planeY * cameraX;

            int mX = (int)(pCX / TILE_SIZE), mY = (int)(pCY / TILE_SIZE);
            float dDX = (rayDX == 0) ? 1e30 : fabsf(1 / rayDX);
            float dDY = (rayDY == 0) ? 1e30 : fabsf(1 / rayDY);

            float sDX, sDY;
            int stepX, stepY;

            if (rayDX < 0) { stepX = -1; sDX = (pCX / TILE_SIZE - mX) * dDX; }
            else { stepX = 1; sDX = (mX + 1.0f - pCX / TILE_SIZE) * dDX; }
            if (rayDY < 0) { stepY = -1; sDY = (pCY / TILE_SIZE - mY) * dDY; }
            else { stepY = 1; sDY = (mY + 1.0f - pCY / TILE_SIZE) * dDY; }

            int hit = 0, side;
            while (hit == 0) {
                if (sDX < sDY) { sDX += dDX; mX += stepX; side = 0; }
                else { sDY += dDY; mY += stepY; side = 1; }
                if (mX < 0 || mX >= MAP_SIZE || mY < 0 || mY >= MAP_SIZE) break;
                if (map[mY][mX] > 0) hit = 1;
            }

            float pWD = (side == 0) ? (sDX - dDX) : (sDY - dDY);
            if (pWD < 0.1f) pWD = 0.1f;
            float h = SCREEN_HEIGHT / pWD;

            float intensity = 1.0f / (1.0f + pWD * pWD * 0.03f);
            Uint8 r = (Uint8)(255 * intensity);
            if (side == 1) r *= 0.7; // Side shading

            SDL_SetRenderDrawColor(renderer, r, 0, 0, 255);
            SDL_FRect slice = {(float)i, (SCREEN_HEIGHT - h) / 2, 1, h};
            SDL_RenderFillRect(renderer, &slice);
        }

        float ms = 0.12f, mt = TILE_SIZE * ms, mar = 20.0f;
        for (int y = 0; y < MAP_SIZE; y++) {
            for (int x = 0; x < MAP_SIZE; x++) {
                if (map[y][x] > 0) {
                    SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
                    SDL_FRect w = {mar + x * mt, mar + y * mt, mt, mt};
                    SDL_RenderFillRect(renderer, &w);
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_FRect mp = {mar + player.x * ms - 4, mar + player.y * ms - 4, 7, 7};
        SDL_RenderFillRect(renderer, &mp);

        SDL_RenderPresent(renderer);
    }
    SDL_Quit();
    return 0;
}


