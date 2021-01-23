#pragma once

class Color {
public:
    float r;
    float g;
    float b;
    float a;

    Color(float r, float g, float b, float a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    };
    Color(int r, int g, int b, int a) {
        this->r = r / 255.0f;
        this->g = g / 255.0f;
        this->b = b / 255.0f;
        this->a = a / 255.0f;
    };
    ~Color() {};
};
