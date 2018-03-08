#pragma once

#include <stdbool.h>

typedef struct PD PlanetData;
typedef struct LVD LaunchVehicleData;
typedef struct LSD LaunchSimulationData;

//returns true if loading was successful, false if otherwise
//todo UNIT TEST
bool load_planet_data(PlanetData* pPlanetData);

