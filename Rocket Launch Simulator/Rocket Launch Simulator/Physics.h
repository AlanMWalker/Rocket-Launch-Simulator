#pragma once

#define GRAVITY_STRENGTH 1.0f 
#define SIM_STEP 1.0/5000.0
#define BIG_G 6.67e-11

#include <stdbool.h>

typedef struct PD
{
	double mass; //kg 
	double radius; // m 
	double distanceToSpace; //m
	double escapeVelocity; //m/s 
	double gravityAcceleration; // m/s^2
} PlanetData;

typedef struct LVD
{
	double payloadMass; //kg
	double exhaustVelocity; // m/s 
	double propellantMass; // kg
	double massEjectionRate; // kg/s
} LaunchVehicleData;

typedef struct LSD
{
	double velocity; //m/s
	double distanceFromLaunchpad; //m
	double currentPropMass; //kg
	double burnoutTime; //s
	double burnoutVelocity; //m/s
	double launchTimer;
} LaunchSimulationData;

typedef struct RSD
{
	PlanetData launch_planet;
	LaunchVehicleData launch_vehicle;
	LaunchSimulationData launch_sim;
	double deltaTime;
	bool bLaunchSimComplete;
} RocketSimmData;


bool load_launch_vehicle_data_from_file(char * filePath, LaunchVehicleData * pLaunchVehicleData);

void setup_planet_constants(PlanetData* pPlanetData);

void setup_launch_vehicle_constants(LaunchVehicleData* pLvd, const PlanetData * pPlanetData);

void step_simulation(RocketSimmData* pSimData);