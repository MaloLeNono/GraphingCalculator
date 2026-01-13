#include <complex>
#include <execution>
#include <iostream>
#include <SDL3/SDL.h>
#include "tinyexpr.h"

constexpr int WIDTH{800};
constexpr int HEIGHT{400};

float scaleX{};
float scaleY{};
float step{};
std::string expression{};
std::vector<SDL_FPoint> points{};

void drawAxes(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 50, 0, 255);
    SDL_RenderLine(renderer, WIDTH / 2.0f, 0.0f, WIDTH / 2.0f, HEIGHT);
    SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255);
    SDL_RenderLine(renderer, 0.0f, HEIGHT / 2.0f, WIDTH, HEIGHT / 2.0f);
}

float evaluateFunctionAt(te_parser& parser, const float x) {
    parser.set_variables_and_functions({{"x", x}});

    if (!parser.compile(expression)) {
        std::cerr << "Invalid expression\n" + expression;
        exit(EXIT_FAILURE);
    }

    const auto result = parser.evaluate();
    return static_cast<float>(result);
}

void toDisplayCoords(float& x, float& y) {
    x = x * scaleX + WIDTH / 2.0f;
    y = HEIGHT / 2.0f - y * scaleY;
}

void calculateFunction() {
    te_parser parser;
    const int steps = static_cast<int>(WIDTH / step);
    for (int i{0}; i < steps; i++) {
        float x{-WIDTH / 2.0f + static_cast<float>(i) * step};
        float y = evaluateFunctionAt(parser, x);
        toDisplayCoords(x, y);
        points.push_back(SDL_FPoint(x, y));
    }
}

void draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawAxes(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderLines(renderer, points.data(), static_cast<int>(points.size()));

    SDL_RenderPresent(renderer);
}

int main(const int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Incorrect Usage\n"
                     "Usage: <expression> <x scale> <y scale> <step>";
        return EXIT_FAILURE;
    }

    expression = argv[1];
    scaleX = std::stof(argv[2]);
    scaleY = std::stof(argv[3]);
    step = std::stof(argv[4]);

    calculateFunction();

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window{SDL_CreateWindow("Graphing Calculator", WIDTH, HEIGHT, 0)};
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    bool running{true};
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
            else if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    default:
                        break;
                }
            }
        }

        draw(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}