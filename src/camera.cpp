#include "camera.hpp"

// Camera::Camera() : m_zoom_level(-4.0f * pow(10, 8)),
                   // m_translation(0.0f, 0.0f, 0.0f), m_center(0.0f, 0.0f, 0.0f)
Camera::Camera() : m_zoom_level(1.0f), m_translation(0.0f, 0.0f, 0.0f),
                   m_center(10.0f, 0.0f, 0.0f)
{

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* return_struct = glfwGetVideoMode(monitor);
    m_screen_width = return_struct->width;
    m_screen_height = return_struct->height;
    m_zoom_ratio = (float) m_screen_width / m_screen_height;
    m_zoomx = m_zoom_ratio * m_zoom_level;
    m_zoomy = m_zoom_level;

    Update();
}

void Camera::Resize() {

}


void Camera::Zoom(const int direction) {
    float scale;
    if (m_zoomx >= 0 && m_zoomx < 200) {
        scale = 10.0f;
    } else if (m_zoomx < 0 && m_zoomx > -200) {
        scale = 10.0f;
    }  else {
        scale = abs(m_zoomx * 0.05);
    }

    m_zoom_level += scale * direction;

    if (m_zoom_level > m_screen_height / 2) {
        m_zoom_level = (m_screen_height / 2) - 1;
    }

    Update();
}

void Camera::Fit(glm::vec2 lbound, glm::vec2 rbound) {

    float zoom_level;
    float zoomlevel_lbound = (lbound.x + m_translation.x) / m_zoom_ratio;
    float zoomlevel_rbound = -(rbound.x + m_translation.x) / m_zoom_ratio;


    zoom_level= fmin(zoomlevel_lbound, zoomlevel_rbound);

    zoomlevel_lbound = (lbound + m_translation.y).y;
    zoomlevel_rbound = -(rbound + m_translation.y).y;

    m_zoom_level = fmin(zoom_level, fmin(zoomlevel_lbound, zoomlevel_rbound));
    Update();
}

void Camera::Update() {
    m_zoomx = m_zoom_ratio * m_zoom_level;
    m_zoomy = m_zoom_level;

    m_proj = glm::ortho(m_zoomx, (float) m_screen_width - m_zoomx,
                        m_zoomy, (float) m_screen_height - m_zoomy, -1.0f, 1.0f);

    m_view = glm::translate(glm::mat4(1.0f), m_translation);
    m_model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
}

void Camera::Move(glm::vec3 translation) {
    float scalex, scaley;
    if (m_zoomx >= 0 && m_zoomx < 200) {
        scalex = 5.0f;
    } else if (m_zoomx < 0 && m_zoomx > -200) {
        scalex = 5.0f;
    }  else {
        scalex = abs(m_zoomx * 0.01);
    }

    if (m_zoomy >= 0 && m_zoomy < 200) {
        scaley = 5.0f;
    } else if (m_zoomx < 0 && m_zoomy > -200) {
        scaley = 5.0f;
    }  else {
        scaley = abs(m_zoomy * 0.01);
    }

    glm::vec3 scale(scalex, scaley, 0.0f);

    m_translation += translation * scale;
    Update();
};

void Camera::Info() {

    std::cout << "zoom level: " << m_zoom_level << std::endl;
    std::cout << "zoomx: " << m_zoomx << " | " << m_screen_width - m_zoomx << std::endl;
    std::cout << "zoomy: " << m_zoomy << " | " << m_screen_height - m_zoomy << std::endl;
    std::cout << "Translate: " << glm::to_string(m_translation) << std::endl;

    std::cout << std::endl;

}

void Camera::Center() {
    // std::cout << "translation: " << m_translation.x << " " << m_translation.y << std::endl;

    m_translation = m_center;
    Update();
}

void Camera::SetCenter(glm::vec3 center) {
//     std::cout << centervec.x << std::endl;
    m_center = -center;
    Update();
    Center();
}
