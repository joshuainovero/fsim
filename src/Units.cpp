#include "Units.hpp"

namespace fsim
{
    namespace units
    {
        const float UNIT_SIZE_IN_PIXELS = 3.415f;
        
        float UNIT_DISTANCE;

        float STANDARD_HEAT_FLUX_RADIUS;

        float STANDARD_HEAT_FLUX_RADIUS_PIXELS;
        
        void changeMagHelper(float f1, float f2, float f3)
        {
            UNIT_DISTANCE = f1;
            STANDARD_HEAT_FLUX_RADIUS = f2;
            STANDARD_HEAT_FLUX_RADIUS_PIXELS = f3;
        }

        void changeMagnitudes(FloorLabel floor)
        {
            switch(floor)
            {
                case FloorLabel::GROUND : 
                    changeMagHelper(
                        FSIM_UNIT_DISTANCE_GROUND, FSIM_STANDARD_HEAT_FLUX_RADIUS_GROUND, FSIM_STANDARD_HEAT_FLUX_RADIUS_GROUND_PIXELS
                    );
                break;

                case FloorLabel::SECOND : break;
                case FloorLabel::THIRD : break;
                case FloorLabel::FOURTH : break;
                default: break;
            }
        }
    }
}