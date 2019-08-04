#pragma once

#define GRAVITY_STRENGTH 1.0f 
#define SIM_STEP 1.0/5000.0
#define BIG_G 6.67e-11

#include <stdbool.h>
#include "Macros.h"

struct PlanetData
{
	double mass; //kg 
	double radius; // m 
	double distanceToSpace; //m
	double escapeVelocity; //m/s 
	double gravityAcceleration; // m/s^2
};

struct LaunchVehicleData
{
	double payloadMass; //kg
	double exhaustVelocity; // m/s 
	double propellantMass; // kg
	double massEjectionRate; // kg/s
};

struct LaunchSimulationData
{
	double velocity; //m/s
	double distanceFromLaunchpad; //m
	double currentPropMass; //kg
	double burnoutTime; //s
	double burnoutVelocity; //m/s
	double launchTimer;
};

struct RocketSimmData
{
	PlanetData launch_planet;
	LaunchVehicleData launch_vehicle;
	LaunchSimulationData launch_sim;
	double deltaTime;
	bool bLaunchSimComplete;
};


class SimPhysics
{
public:
	//bool load_launch_vehicle_data_from_file(char* filePath, LaunchVehicleData* pLaunchVehicleData);

	void initialiseSimulationPhysicsModule(RocketSimmData* pSimData);

	void setupPlanetConstants(PlanetData* pPlanetData);

	void setupLaunchVehicleConstants(LaunchVehicleData* pLvd, const PlanetData* pPlanetData);

	void stepSimulation(double deltaTime);

private:

	PlanetData* m_pPlanetData;
	LaunchVehicleData* m_pLVD;

	LaunchSimulationData* m_pLaunchSim;
};
