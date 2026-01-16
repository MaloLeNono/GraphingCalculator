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
float offsetX{};
float offsetY{};
int gridOpacity{50};
int axesOpacity{75};
std::string expressionString{};

std::vector<SDL_FPoint> points{};

void drawAxes(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, axesOpacity);
    SDL_RenderLine(renderer, WIDTH / 2.0f + offsetX, 0.0f, WIDTH / 2.0f + offsetX, HEIGHT);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, axesOpacity);
    SDL_RenderLine(renderer, 0.0f, HEIGHT / 2.0f - offsetY, WIDTH, HEIGHT / 2.0f - offsetY);
}

void drawGrid(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, gridOpacity);

    const int verticalLineAmount{static_cast<int>(WIDTH / scaleX)};
    for (int i{}; i < verticalLineAmount; i++) {
        const float x{static_cast<float>(i) * scaleX};
        SDL_RenderLine(renderer, x, 0.0f, x, HEIGHT);
    }

    const int horizontalLineAmount{static_cast<int>(HEIGHT / scaleY)};
    for (int i{}; i < horizontalLineAmount; i++) {
        const float y{static_cast<float>(i) * scaleY};
        SDL_RenderLine(renderer, 0.0f, y, WIDTH, y);
    }
}

float evaluateFunctionAt(exprtk::parser<float>& parser, float x) {
    exprtk::symbol_table<float> symbol_table;
    symbol_table.add_variable("x", x);
    exprtk::expression<float> expression;
    expression.register_symbol_table(symbol_table);
    parser.compile(expressionString, expression);
    return expression.value();
}

SDL_FPoint getDisplayCoords(float x, float y) {
    x = x * scaleX + offsetX + WIDTH / 2.0f;
    y = HEIGHT / 2.0f - y * scaleY - offsetY;
    return SDL_FPoint(x, y);
}

void calculateFunction() {
    exprtk::parser<float> parser;
    const int steps = static_cast<int>(WIDTH / step);
    for (int i{}; i < steps; i++) {
        const float x{-WIDTH / 2.0f + static_cast<float>(i) * step};
        const float y = evaluateFunctionAt(parser, x);
        points.emplace_back(getDisplayCoords(x, y));
    }
}

bool isValidLine(const SDL_FPoint point1, const SDL_FPoint point2) {
    return point1.y > -MAX_DRAW_OVERFLOW
        && point1.y < HEIGHT + MAX_DRAW_OVERFLOW
        && point2.y > -MAX_DRAW_OVERFLOW
        && point2.y < HEIGHT + MAX_DRAW_OVERFLOW;
}

void drawFunction(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255 ,255);
    SDL_FPoint lastPoint = points.at(0);
    for (const SDL_FPoint point : points) {
        if (isValidLine(lastPoint, point))
            SDL_RenderLine(renderer, lastPoint.x, lastPoint.y, point.x, point.y);
        lastPoint = point;
    }
}

void draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawGrid(renderer);
    drawAxes(renderer);
    drawFunction(renderer);

    SDL_RenderPresent(renderer);
}

void logError(const std::string& error) {
    std::cerr << error;
    exit(EXIT_FAILURE);
}

void verifyOpacityArg(const std::string& arg, int& opacity) {
    try {
        opacity = std::stoi(arg);
    }
    catch ([[maybe_unused]] std::exception& e) {
        logError("Enter an opacity that represents a whole number");
    }

    if (opacity < 0 || opacity > 255)
        logError("Opacity must be a whole number between 0 and 100");
}

int main(const int argc, char* argv[]) {
    constexpr int requiredArgumentAmount{7};
    if (argc < requiredArgumentAmount || argc > 9) {
        std::cerr << "Incorrect Usage\n"
                     "Usage: <expression> <xscale> <yscale> <step> <xoffset> <yoffset> [gridopacity(0-255)] [axesopacity(0-255)]";
        return EXIT_FAILURE;
    }

    expressionString = argv[1];

    try {
        scaleX = std::stof(argv[2]);
        scaleY = std::stof(argv[3]);
        step = std::stof(argv[4]);
    }
    catch ([[maybe_unused]] std::exception& e) {
        logError("Step and scale values must be numeric");
    }

    try {
        offsetX = std::stof(argv[5]);
        offsetY = std::stof(argv[6]);
    }
    catch ([[maybe_unused]] std::exception& e) {
        logError("Offset values must be numeric");
    }

    for (int i{requiredArgumentAmount}; i < argc; i++) {
        const std::string arg = argv[i];

        switch (i) {
            case 7:
                verifyOpacityArg(argv[i], gridOpacity);
                break;
            case 8:
                verifyOpacityArg(argv[i], axesOpacity);
                break;
            default:
                break;
        }
    }

    if (scaleX < 0 || scaleY < 0 || step < 0) {
        std::cerr << "ERROR: Scale and step arguments cannot be negative";
        return EXIT_FAILURE;
    }

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