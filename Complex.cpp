#include <SDL2/SDL.h>
#include <vector>
#include <complex>

const int WIDTH = 800;
const int HEIGHT = 600;

int get_iterations(double x, double y, int max_iter) {
    std::complex<double> c(x, y);
    std::complex<double> z(0, 0);
    int iter = 0;
    while (std::abs(z) < 2.0 && iter < max_iter) {
        z = z * z + c;
        iter++;
    }
    return iter;
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Complex Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

    std::vector<uint32_t> pixels(WIDTH * HEIGHT);
    double zoom = 1.0, moveX = -0.5, moveY = 0.0;
    bool running = true;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_UP) zoom *= 1.1;
                if (e.key.keysym.sym == SDLK_DOWN) zoom /= 1.1;
                if (e.key.keysym.sym == SDLK_w) moveY -= 0.1 / zoom;
                if (e.key.keysym.sym == SDLK_s) moveY += 0.1 / zoom;
            }
        }

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                double pr = 1.5 * (x - WIDTH / 2) / (0.5 * zoom * WIDTH) + moveX;
                double pi = (y - HEIGHT / 2) / (0.5 * zoom * HEIGHT) + moveY;
                int iter = get_iterations(pr, pi, 100);
                
                uint8_t color = (uint8_t)(iter * 255 / 100);
                pixels[y * WIDTH + x] = (color << 16) | (color << 8) | 255; // Blue-ish tint
            }
        }

        SDL_UpdateTexture(texture, NULL, pixels.data(), WIDTH * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
