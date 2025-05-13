#pragma once
#include <unordered_map>
#include "GLFW/glfw3.h"
#include "keys.h"

class InputManager {
public:
    static void update(GLFWwindow* window);

    static KeyState getKeyState(int key);

    static bool isKeyDown(Key key);    // Just pressed
    static bool isKeyHeld(Key key);    // Being held
    static bool isKeyUp(Key key);      // Just released
    static bool isKeyDown(Btn key);    // Just pressed
    static bool isKeyHeld(Btn key);    // Being held
    static bool isKeyUp(Btn key);      // Just released

private:
    static std::unordered_map<int, bool> prevKeys;
    static std::unordered_map<int, bool> currKeys;
};