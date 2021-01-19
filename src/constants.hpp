#pragma once
#include <math.h>
#include <imgui.h>
// #include <limits>

const ImGuiDataType_ ImGuiDataType_Real = ImGuiDataType_Double;
// bool (*SliderReal)(const char *, float *, float, float, const char *, float) = ImGui::SliderFloat;

typedef double real;
// bool (*SliderReal)(const char *, real *, real, real, const char *, real) = ImGui::Slider;
typedef glm::vec<3, double, glm::highp> vec3;
typedef glm::dvec2 vec2;
typedef glm::dmat4 mat4;

typedef glm::vec2 fvec2;
typedef glm::vec3 fvec3;
typedef glm::mat4 fmat4;

typedef glm::dvec2 dvec2;
typedef glm::dvec3 dvec3;
typedef glm::dmat4 dmat4;


const real G = 6.674 * pow(10, -11);

const real FLOAT_ZERO = 0.0f;

const real MASS_EARTH = 5.972 * pow(10, 24);
const real MASS_MOON = 7.348 * pow(10, 22);
const real DISTANCE_MOON_EARTH = 384402 * pow(10, 3);
const real RADIUS_EARTH = 6371 * pow(10, 3);
const real RADIUS_MOON = 1737 * pow(10, 3);
const real VELOCITY_MOON = 1 * pow(10, 3);
// const real DELTA_TIME = 86400;
const real DELTA_TIME = 60*60;


// const real MAX_MASS = 10 * MASS_EARTH;
const real MAX_MASS = (FLT_MAX - 1)/2;
const real SLIDERFLOAT_MAX = FLT_MAX/2.0f;
const real SLIDERFLOAT_MIN = -FLT_MAX/2.0f;

// const real MAX_RADIUS = 10 * RADIUS_EARTH;
const real MAX_RADIUS = (FLT_MAX - 1)/2;
const unsigned int RESTART_INDEX = 0xFFFFFFFF;
