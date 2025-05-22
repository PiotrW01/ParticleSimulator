#pragma once
#include <unordered_map>
#include "keys.h"
struct GLFWwindow;

class InputManager {
public:
    static void update(GLFWwindow* window);

    static KeyState getKeyState(int key);

    static bool isKeyJustPressed(Key key);    // Just pressed
    static bool isKeyDown(Key key);    // Being held
    static bool isKeyJustReleased(Key key);      // Just released
    static bool isKeyJustPressed(Btn key);    // Just pressed
    static bool isKeyDown(Btn key);    // Being held
    static bool isKeyJustReleased(Btn key);      // Just released

private:
    static std::unordered_map<int, bool> prevKeys;
    static std::unordered_map<int, bool> currKeys;
};