#include <chrono>
#include <execution>
#include <iostream>
#include <SDL3/SDL.h>
#include "exprtk.hpp"

constexpr int WIDTH{800};
constexpr int HEIGHT{400};
constexpr int MAX_DRAW_OVERFLOW{100};

float scaleX{};
float scaleY{};
float step{};
std::string expressionString{};
std::vector<SDL_FPoint> points{};

void drawAxes(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 50, 0, 255);
    SDL_RenderLine(renderer, WIDTH / 2.0f, 0.0f, WIDTH / 2.0f, HEIGHT);
    SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255);
    SDL_RenderLine(renderer, 0.0f, HEIGHT / 2.0f, WIDTH, HEIGHT / 2.0f);
}

float evaluateFunctionAt(exprtk::parser<float>& parser, float x) {
    exprtk::symbol_table<float> symbol_table;
    symbol_table.add_variable("x", x);
    exprtk::expression<float> expression;
    expression.register_symbol_table(symbol_table);
    parser.compile(expressionString, expression);
    return expression.value();
}

void toDisplayCoords(float& x, float& y) {
    x = x * scaleX + WIDTH / 2.0f;
    y = HEIGHT / 2.0f - y * scaleY;
}

void calculateFunction() {
    exprtk::parser<float> parser;
    const int steps = static_cast<int>(WIDTH / step);
    for (int i{0}; i < steps; i++) {
        float x{-WIDTH / 2.0f + static_cast<float>(i) * step};
        float y = evaluateFunctionAt(parser, x);
        toDisplayCoords(x, y);
        points.push_back(SDL_FPoint(x, y));
    }
}

bool isValidLine(const SDL_FPoint point1, const SDL_FPoint point2) {
    return point1.y > -MAX_DRAW_OVERFLOW
        && point1.y < HEIGHT + MAX_DRAW_OVERFLOW
        && point2.y > -MAX_DRAW_OVERFLOW
        && point2.y < HEIGHT + MAX_DRAW_OVERFLOW;
}

void drawFunction(SDL_Renderer* renderer) {
    SDL_FPoint lastPoint = points.at(0);
    for (const SDL_FPoint point : points) {
        if (isValidLine(lastPoint, point))
            SDL_RenderLine(renderer, lastPoint.x, lastPoint.y, point.x, point.y);
        lastPoint = point;
    }
}

void draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawAxes(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    drawFunction(renderer);

    SDL_RenderPresent(renderer);
}

int main(const int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Incorrect Usage\n"
                     "Usage: <expression> <x scale> <y scale> <step>";
        return EXIT_FAILURE;
    }

    expressionString = argv[1];
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