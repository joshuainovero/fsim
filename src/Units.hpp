#pragma once
#include "Constants.hpp"

namespace fsim
{
    namespace units
    {
        extern const float UNIT_SIZE_IN_PIXELS;
        
        extern float UNIT_DISTANCE;

        extern float STANDARD_HEAT_FLUX_RADIUS;

        extern float STANDARD_HEAT_FLUX_RADIUS_PIXELS;

        extern void changeMagHelper(float f1, float f2, float f3);
        
        extern void changeMagnitudes(FloorLabel floor);
    }
}