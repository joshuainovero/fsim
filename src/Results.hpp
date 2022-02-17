#pragma once
#include <string>

namespace fsim
{
    struct Results
    {
        std::string point_name;

        float distance_traveled;
        float danger_indicator_average;
        float safe_path_proportion;
        float risky_path_proportion;

        uint32_t node_count;
        uint32_t obstructions_count;
    };
}