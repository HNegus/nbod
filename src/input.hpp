#pragma once

#include "utils.hpp"
#include "simulation.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    Simulation *simulation = (Simulation *) glfwGetWindowUserPointer(window);
    glm::vec3 move(0.0f, 0.0f, 0.0f);


    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        move.x += 1;
    } else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        move.x -= 1;
    } else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        move.y -= 1;
    } else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        move.y += 1;
    } else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        simulation->TogglePlay();
    } else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        simulation->GuiToggle();
    } else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        simulation->CameraCenter();
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) {
        move.x +=  5;
    } else if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) {
        move.x -= 5;
    } else if (key == GLFW_KEY_UP && action == GLFW_REPEAT) {
        move.y -= 5;
    } else if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT) {
        move.y += 5;
    }

    simulation->CameraMove(move);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

    Simulation *simulation = (Simulation *) glfwGetWindowUserPointer(window);
    simulation->CameraZoom(yoffset);

    return;
}
