#pragma once
#include "utils.hpp"

class Camera {

private:
    unsigned int m_screen_width, m_screen_height;
    float m_zoom_level, m_zoom_ratio, m_zoomx, m_zoomy;

    mat4 m_proj, m_view, m_model;
    vec3 m_translation, m_center;

public:

    Camera();
    ~Camera() {};

    void Update();
    void Clear();


    void Zoom(const int direction);
    void ZoomIn() { Zoom(1); }
    void ZoomOut() { Zoom(-1); };

    void SetCenter(vec3 center);
    void Center();
    void Fit(vec2 lbound, vec2 rbound);
    void Move(const vec3 translation);


    void Info();

    vec3 GetPosition() const { return -m_translation; };
    int ScreenWidth() const { return m_screen_width; };
    int ScreenHeight() const { return m_screen_height; };
    real HorizontalDistance() const { return (real) m_screen_width - 2*m_zoomx; };
    real VerticalDistance() const { return (real) m_screen_height - 2*m_zoomy; };

    fmat4 MVP() const { return fmat4(m_proj * m_view * m_model); };

    friend std::ostream& operator<<(std::ostream& os, const Camera& camera);
    friend std::istream& operator>>(std::istream& is, Camera& camera);
};
