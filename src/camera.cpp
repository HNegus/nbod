#include "camera.hpp"

Camera::Camera() : m_zoom_level(-4.0f * pow(10, 8)), m_translation(0.0f, 0.0f, 0.0f) {

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* return_struct = glfwGetVideoMode(monitor);
    m_screen_width = return_struct->width;
    m_screen_height = return_struct->height;
    m_zoom_ratio = (float) m_screen_width / m_screen_height;
    m_zoomx = m_zoom_ratio * m_zoom_level;
    m_zoomy = m_zoom_level;

    Update();
}


void Camera::Zoom(const int direction) {
    float scale = m_zoomx * 0.05;

    m_zoom_level += scale * direction;

    if (m_zoom_level >= m_screen_height - m_zoomy - 1) {
        m_zoom_level -= scale * direction;
    } else {
        m_zoomx = m_zoom_ratio * m_zoom_level;
        m_zoomy = m_zoom_level;
    }

    Update();
}

void Camera::Update() {
    m_proj = glm::ortho(m_zoomx, (float) m_screen_width - m_zoomx,
                        m_zoomy, (float) m_screen_height - m_zoomy, -1.0f, 1.0f);

    m_view = glm::translate(glm::mat4(1.0f), m_translation);
    m_model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

}

// void ZoomInFast();
// void ZoomOutFast();
