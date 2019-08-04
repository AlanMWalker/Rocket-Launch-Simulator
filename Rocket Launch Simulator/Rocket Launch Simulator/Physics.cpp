#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>	

#include <string.h>

#include "Physics.h"
#include "UI.h"

void  SimPhysics::setupPlanetConstants(PlanetData* pPlanetData)
{
	pPlanetData->escapeVelocity = sqrt(2 * BIG_G * pPlanetData->mass / pPlanetData->radius);
	pPlanetData->gravityAcceleration = (BIG_G * pPlanetData->mass) / (pPlanetData->radius * pPlanetData->radius);
}

void  SimPhysics::setupLaunchVehicleConstants(LaunchVehicleData * pLvd, const PlanetData * pPlanetData)
{
	const double Total_Mass = pLvd->payloadMass * pow(M_E, pPlanetData->escapeVelocity / pLvd->exhaustVelocity);
	pLvd->propellantMass = Total_Mass - pLvd->payloadMass;
}

void SimPhysics::stepSimulation(RocketSimmData * pSimData)
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

	if (pLaunchSim->currentPropMass < 0.0f)
	{
		pSimData->bLaunchSimComplete = true;
	}
}
