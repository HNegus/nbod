#pragma once
#include "utils.hpp"

class Camera {

private:
    unsigned int m_screen_width, m_screen_height;
    float m_zoom_level, m_zoom_ratio, m_zoomx, m_zoomy;

    glm::mat4 m_proj, m_view, m_model;
    glm::vec3 m_translation;

public:

    Camera();
    ~Camera() {};


    void Do() {
        std::cout << "!!!!" << m_screen_width << std::endl;
    }
    void Zoom(const int direction);
    void ZoomIn() { Zoom(1); }
    void ZoomOut() { Zoom(-1); };
    void ZoomInFast();
    void ZoomOutFast();

    void Update();
    void Center();
    void Info();

    void Move(glm::vec3 translation);
    int ScreenWidth() const { return m_screen_width; };
    int ScreenHeight() const { return m_screen_height; };



    glm::mat4 MVP() const { return m_proj * m_view * m_model; };
};
