#pragma once
#include "utils.hpp"


enum E_ErrorLevels {
    NONE = 0,
    NOTIFICATION = GL_DEBUG_SEVERITY_NOTIFICATION,
    LOW = GL_DEBUG_SEVERITY_LOW,
    MEDIUM = GL_DEBUG_SEVERITY_MEDIUM,
    HIGH = GL_DEBUG_SEVERITY_HIGH
};

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char *message,
                            const void *userParam);

void ErrorInit(E_ErrorLevels level);
void PrintInfo();
