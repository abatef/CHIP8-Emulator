#include <SDL2/SDL.h>
#include <stdexcept>

class Display
{
public:
    SDL_Window   *window;
    SDL_Renderer *renderer;
    int           width, height, scale;
    uint8_t       pixels[64][32] = {0};
    Display(int width, int height, int scale)
        : width(width)
        , height(height)
        , scale(scale)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            throw std::runtime_error(SDL_GetError());
        }

        window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  width * scale, height * scale, SDL_WINDOW_SHOWN);
        if (!window) {
            throw std::runtime_error(SDL_GetError());
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            throw std::runtime_error(SDL_GetError());
        }
    }

    ~Display()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void clear()
    {
        memset(pixels, 0, sizeof(pixels));
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    void update()
    {
        for (int y = 0; y < 32; ++y) {
            for (int x = 0; x < 64; ++x) {
                uint8_t wrappedX = x % 64;
                uint8_t wrappedY = y % 32;
                drawPixel(wrappedX, wrappedY, pixels[wrappedX][wrappedY]);
            }
        }
    }

    void drawPixel(int x, int y, uint8_t on)
    {
        SDL_Rect rect = {x * scale, y * scale, scale, scale};
        SDL_SetRenderDrawColor(renderer, on ? 255 : 0, on ? 255 : 0, on ? 255 : 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    void render()
    {
        SDL_RenderPresent(renderer);
    }
};
