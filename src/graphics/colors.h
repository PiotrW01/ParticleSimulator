#pragma once

struct Color
{
    float r, g, b, a;
    constexpr Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {};
};

struct Colors
{
    static constexpr Color BLACK = Color(0.0f, 0.0f, 0.0f);
    static constexpr Color WHITE = Color(1.0f, 1.0f, 1.0f);
    static constexpr Color RED = Color(1.0f, 0.0f, 0.0f);
    static constexpr Color GREEN = Color(0.0f, 1.0f, 0.0f);
    static constexpr Color BLUE = Color(0.0f, 0.0f, 1.0f);
    static constexpr Color SAND = Color(0.8f, 0.6f, 0.4f);
    static constexpr Color GRAY = Color(0.3f, 0.3f, 0.3f);

};