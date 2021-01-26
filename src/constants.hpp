#pragma once
#include <math.h>
#include <imgui.h>

const std::string SHADER_DIR = "../shaders/";
const std::string BODY_SHADER_VERT = "body.vert";
const std::string BODY_SHADER_FRAG = "body.frag";

const std::string HIST_SHADER_VERT = "hist.vert";
const std::string HIST_SHADER_FRAG = "hist.frag";

const std::string SCENE_DIR = "../scenes/";
const std::string SAVE_DIR = "../saves/";
const std::string LOG_DIR = "../logs/";

const ImGuiDataType_ ImGuiDataType_Real = ImGuiDataType_Double;

typedef double real;
typedef glm::dvec3 vec3;
typedef glm::dvec2 vec2;
typedef glm::dmat4 mat4;

typedef glm::vec2 fvec2;
typedef glm::vec3 fvec3;
typedef glm::mat4 fmat4;

typedef glm::dvec2 dvec2;
typedef glm::dvec3 dvec3;
typedef glm::dmat4 dmat4;

const real G = 6.674 * pow(10, -11);
const unsigned int RESTART_INDEX = 0xFFFFFFFF;

const int SECONDS_PER_DAY = 86400;
const float DAYS_PER_YEAR = 365.25;
