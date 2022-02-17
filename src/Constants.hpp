#ifndef FSIM_CONSTANTS_H
#define FSIM_CONSTANTS_H

// All units are in SI

#define FSIM_NOPATHDIST 0xA46B6

// Ground floor constants
#define FSIM_UNIT_DISTANCE_GROUND 0.5544342178f
#define FSIM_STANDARD_HEAT_FLUX_RADIUS_GROUND 9.144f
#define FSIM_STANDARD_HEAT_FLUX_RADIUS_GROUND_PIXELS 56.32184847f

enum FloorLabel { GROUND = 0, SECOND = 1, THIRD = 2, FOURTH = 3 };

#endif // FSIM_CONSTANTS_H