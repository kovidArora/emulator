
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        
        "CHIP-8 Emulator",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 320,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
    std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
    SDL_Quit();
    return 1;
}
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
    std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
}
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}