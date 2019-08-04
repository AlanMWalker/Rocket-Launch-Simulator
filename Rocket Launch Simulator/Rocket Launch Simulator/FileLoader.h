#pragma once

#include <stdbool.h>

struct PlanetData;
struct LaunchVehicleData;

//returns true if loading was successful, false if otherwise
//todo UNIT TEST
bool load_planet_data(PlanetData* pPlanetData);

bool load_rocket_data(LaunchVehicleData* pLVD);