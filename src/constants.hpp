#pragma once
#include <math.h>
// #include <limits>


const double G = 6.674 * pow(10, -11);
const float FLOAT_ZERO = 0.0f;

const float MASS_EARTH = 5.972 * pow(10, 24);
const float MASS_MOON = 7.348 * pow(10, 22);
const float DISTANCE_MOON_EARTH = 384402 * pow(10, 3);
const float RADIUS_EARTH = 6371 * pow(10, 3);
const float RADIUS_MOON = 1737 * pow(10, 3);
const float VELOCITY_MOON = 1 * pow(10, 3);
const float DELTA_TIME = 86400 / 60;
// const float DELTA_TIME = 0.01;


// const float MAX_MASS = 10 * MASS_EARTH;
const float MAX_MASS = (FLT_MAX - 1)/2;
const float SLIDERFLOAT_MAX = FLT_MAX/2.0f;
const float SLIDERFLOAT_MIN = -FLT_MAX/2.0f;

// const float MAX_RADIUS = 10 * RADIUS_EARTH;
const float MAX_RADIUS = (FLT_MAX - 1)/2;
