#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>	

#include <string.h>

#include "Physics.h"
#include "UI.h"

void SimPhysics::initialiseSimulationPhysicsModule(RocketSimmData* pSimData)
{
	/*
	Simulation on enter stages:
	A) set current prop mass to the mass of the propellent the launch vehicle is loaded with
	B) calculate burnout time, and create var to store that (Burnout_Time = Prop Mass / Flowrate)
	C) calculate burnout velocity ->
		burnoutVelocity = u * ln(m0/m) - gt
	*/
	m_pPlanetData = &pSimData->launch_planet;
	m_pLVD = &pSimData->launch_vehicle;
	m_pLaunchSim = &pSimData->launch_sim;

	const double combinedMass = m_pLVD->payloadMass + m_pLVD->propellantMass;

	m_pLaunchSim->launchTimer = 0.0f;
	m_pLaunchSim->currentPropMass = m_pLVD->propellantMass;
	m_pLaunchSim->burnoutTime = m_pLVD->propellantMass / m_pLVD->massEjectionRate;
	m_pLaunchSim->burnoutVelocity = (m_pLVD->exhaustVelocity * log(m_pLVD->payloadMass + m_pLVD->propellantMass / m_pLVD->payloadMass))
		- m_pPlanetData->gravityAcceleration * m_pLaunchSim->burnoutTime;

	print_initial_launch_calculations(m_pLaunchSim);

	pSimData->bLaunchSimComplete = false;

	if (isnan(m_pLaunchSim->burnoutTime) || isinf(m_pLaunchSim->burnoutTime) ||
		isnan(m_pLaunchSim->burnoutVelocity) || isinf(m_pLaunchSim->burnoutVelocity))
	{
		printf("CALC ERROR\n");
	}
}

void SimPhysics::setupPlanetConstants(PlanetData* pPlanetData)
{
	pPlanetData->escapeVelocity = sqrt(2 * BIG_G * pPlanetData->mass / pPlanetData->radius);
	pPlanetData->gravityAcceleration = (BIG_G * pPlanetData->mass) / (pPlanetData->radius * pPlanetData->radius);
}

void SimPhysics::setupLaunchVehicleConstants(LaunchVehicleData * pLvd, const PlanetData * pPlanetData)
{
	const double Total_Mass = pLvd->payloadMass * pow(M_E, pPlanetData->escapeVelocity / pLvd->exhaustVelocity);
	pLvd->propellantMass = Total_Mass - pLvd->payloadMass;
}

void SimPhysics::stepSimulation(double deltaTime)
{
	m_pLaunchSim->launchTimer += deltaTime;

	/*
	Simulation step stages:
	A) calcuate the acceleration due to the thrust of the rocket engines
	B)
	*/
	double acceleration = 0.0;

	//a = (u/m)*dm/dt-g
	//a = acceleration (m/s^2)
	//m = current mass of vehicle (kg)
	//dm/dt = fuel burn rate
	//g = acceleration due to gravity of the planet you're on

	const double relativeVelocity = -m_pLVD->exhaustVelocity;

	const double force = -relativeVelocity * (m_pLVD->massEjectionRate);

	acceleration = force * 1.0 / (m_pLaunchSim->currentPropMass + m_pLVD->payloadMass);

	m_pLaunchSim->currentPropMass -= m_pLVD->massEjectionRate * deltaTime;

	m_pLaunchSim->velocity += (acceleration * deltaTime);
	m_pLaunchSim->distanceFromLaunchpad += (m_pLaunchSim->velocity * deltaTime);

	if (m_pLaunchSim->currentPropMass < 0.0f)
	{
		deltaTime = true;
	}
}
