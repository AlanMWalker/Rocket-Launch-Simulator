#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>	

#include <string.h>

#include "Physics.h"
#include "UI.h"

bool load_planet_data_from_file(char * filePath, PlanetData * pPlanetData)
{
	FILE* pFile = NULL;

	if (pPlanetData == NULL)
	{
		return false;
	}

	const errno_t result = fopen_s(&pFile, filePath, "r");

	if (result != 0)
	{
		printf("Unable to open file for planet data! Error code: %d\n", result);
	}

	memset(pPlanetData, 0, sizeof(PlanetData));

	fscanf_s(pFile, "%lf", &pPlanetData->mass);
	fscanf_s(pFile, "%lf", &pPlanetData->radius);
	fscanf_s(pFile, "%lf", &pPlanetData->distanceToSpace);

	fclose(pFile);

	setup_planet_constants(pPlanetData);
	return true;
}

bool load_launch_vehicle_data_from_file(char * filePath, LaunchVehicleData * pLaunchVehicleData)
{
	FILE* pFile = NULL;

	if (pLaunchVehicleData == NULL)
	{
		return false;
	}

	const errno_t fopenResult = fopen_s(&pFile, filePath, "r");

	if (fopenResult != 0)
	{
		printf_s("Unable to open file for launch vehicle data! Error code: %d\n", fopenResult);
		return false;
	}

	memset(pLaunchVehicleData, 0, sizeof(LaunchVehicleData));

	fscanf_s(pFile, "%lf", &pLaunchVehicleData->payloadMass);
	fscanf_s(pFile, "%lf", &pLaunchVehicleData->exhaustVelocity);
	fscanf_s(pFile, "%lf", &pLaunchVehicleData->massEjectionRate);

	fclose(pFile);
	return true;
}

void setup_planet_constants(PlanetData * pPlanetData)
{
	pPlanetData->escapeVelocity = sqrt(2 * BIG_G* pPlanetData->mass / pPlanetData->radius);
	pPlanetData->gravityAcceleration = (BIG_G * pPlanetData->mass) / (pPlanetData->radius* pPlanetData->radius);
}

void setup_launch_vehicle_constants(LaunchVehicleData * pLvd, const PlanetData *  pPlanetData)
{
	const double Total_Mass = pLvd->payloadMass * pow(M_E, pPlanetData->escapeVelocity / pLvd->exhaustVelocity);
	pLvd->propellantMass = Total_Mass - pLvd->payloadMass;
}

void step_simulation(RocketSimmData* pSimData)
{
	static bool bIsFirstExecution = true;
	const PlanetData* pPlanetData = &pSimData->launch_planet;
	const LaunchVehicleData* pLVD = &pSimData->launch_vehicle;
	LaunchSimulationData* pLaunchSim = &pSimData->launch_sim;
	const double combinedMass = pLVD->payloadMass + pLVD->propellantMass;

	/*
	Simulation on enter stages:
	A) set current prop mass to the mass of the propellent the launch vehicle is loaded with
	B) calculate burnout time, and create var to store that (Burnout_Time = Prop Mass / Flowrate)
	C) calculate burnout velocity ->
		burnoutVelocity = u * ln(m0/m) - gt
	*/
	if (bIsFirstExecution)
	{
		pLaunchSim->launchTimer = 0.0f;
		bIsFirstExecution = false;
		pLaunchSim->currentPropMass = pLVD->propellantMass;
		pLaunchSim->burnoutTime = pLVD->propellantMass / pLVD->massEjectionRate;
		pLaunchSim->burnoutVelocity = (pLVD->exhaustVelocity * log(pLVD->payloadMass + pLVD->propellantMass / pLVD->payloadMass))
			- pPlanetData->gravityAcceleration * pLaunchSim->burnoutTime;

		print_initial_launch_calculations(pLaunchSim);

		pSimData->bLaunchSimComplete = false;

		if (isnan(pLaunchSim->burnoutTime) || isinf(pLaunchSim->burnoutTime) ||
			isnan(pLaunchSim->burnoutVelocity) || isinf(pLaunchSim->burnoutVelocity))
		{
			printf("CALC ERROR\n");
		}


		return;
	}

	pLaunchSim->launchTimer += pSimData->deltaTime;

	/*
	Simulation step stages:
	A) calcuate the acceleration due to the thrust of the rocket engines
	B)
	*/
	double acceleration;

	//a = (u/m)*dm/dt-g
	//a = acceleration (m/s^2)
	//m = current mass of vehicle (kg)
	//dm/dt = fuel burn rate
	//g = acceleration due to gravity of the planet you're on

	const double relativeVelocity = -pLVD->exhaustVelocity;

	double force = -relativeVelocity * (pLVD->massEjectionRate);

	acceleration = force * 1.0 / (pLaunchSim->currentPropMass + pLVD->payloadMass);

	pLaunchSim->currentPropMass -= pLVD->massEjectionRate * pSimData->deltaTime;

	pLaunchSim->velocity += (acceleration * pSimData->deltaTime);
	pLaunchSim->distanceFromLaunchpad += (pLaunchSim->velocity * pSimData->deltaTime);

	//printf("Velocity : %lf\n", pLaunchSim->velocity);
	//printf("Relative Velocity : %lf\n", relativeVelocity);
	//printf("Force: %lf\n", force);
	//printf("Prop Mass: %lf\n", pLaunchSim->currentPropMass);

	if (pLaunchSim->currentPropMass < 0.0f)
	{
		pSimData->bLaunchSimComplete = true;
	}

	//printf("Distance : %lf\n", pLaunchSim->distanceFromLaunchpad);
}
