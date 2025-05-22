#include "inputmanager.h"
#include "GLFW/glfw3.h"

std::unordered_map<int, bool> InputManager::prevKeys;
std::unordered_map<int, bool> InputManager::currKeys;

void InputManager::update(GLFWwindow* window) {
    prevKeys = currKeys; // copy last frame states


    // mouse
    for (int key = GLFW_MOUSE_BUTTON_1; key <= GLFW_MOUSE_BUTTON_LAST; ++key) {
        int state = glfwGetMouseButton(window, key);
        currKeys[key] = state == GLFW_PRESS;
    }

    // keyboard
    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
        int state = glfwGetKey(window, key);
        currKeys[key] = state == GLFW_PRESS;
    }
}

KeyState InputManager::getKeyState(int key) {
    bool wasPressed = prevKeys.count(key) ? prevKeys.at(key) : false;
    bool isPressed  = currKeys.count(key) ? currKeys.at(key) : false;

    if (!wasPressed && isPressed)
        return KeyState::JustPressed;
    if (wasPressed && isPressed)
        return KeyState::Held;
    if (wasPressed && !isPressed)
        return KeyState::JustReleased;
    return KeyState::Released;
}

bool InputManager::isKeyJustPressed(Key key) {
    return getKeyState((int)key) == KeyState::JustPressed;
}

bool InputManager::isKeyDown(Key key) {
    return getKeyState((int)key) == KeyState::Held;
}

bool InputManager::isKeyJustReleased(Key key) {
    return getKeyState((int)key) == KeyState::JustReleased;
}

bool InputManager::isKeyJustPressed(Btn key) {
    return getKeyState((int)key) == KeyState::JustPressed;
}

bool InputManager::isKeyDown(Btn key) {
    return getKeyState((int)key) == KeyState::Held;
}

bool InputManager::isKeyJustReleased(Btn key) {
    return getKeyState((int)key) == KeyState::JustReleased;
}