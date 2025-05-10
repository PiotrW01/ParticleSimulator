#pragma once

enum class Key
{
    SPACE = 32,
    APOSTROPHE = 39,
    COMMA = 44,
    MINUS,
    PERIOD,
    SLASH,
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    SEMICOLON = 59,
    EQUAL = 61,
    A = 65,
    B, C, D, E, F, G, H, I, J, K, L, M, N, 
    O, P, Q, R, S, T, U, V, W, X, Y, Z,
    LEFT_BRACKET,
    BACKSLASH,
    RIGHT_BRACKET,
    GRAVE_ACCENT = 96,
    WORLD_1 = 161, WORLD_2 = 162,
    ESCAPE = 256, ENTER, TAB, BACKSPACE, INSERT, DELETE,
    RIGHT, LEFT, DOWN, UP, PAGE_UP, PAGE_DOWN, HOME, END,
    CAPSLOCK = 280, SCROLLOCK, NUMLOCK, PRINTSCREEN, PAUSE,
    F1 = 290, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23,
    F24, F25
};

enum class Btn
{
    LEFT = 0,
    RIGHT = 1,
    MIDDLE = 2
};

enum class KeyState
{
    Released,
    JustPressed,
    Held,
    JustReleased
};