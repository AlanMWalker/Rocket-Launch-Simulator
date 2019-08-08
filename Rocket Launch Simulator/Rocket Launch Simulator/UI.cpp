#include "UI.h"

#include <stdio.h>

void print_planet_stats(const PlanetData * pPlanetData)
{
	if (pPlanetData == NULL)
	{
		return;
	}

	printf("-- PLANET DATA -- \n");
	printf("Planet Mass - %lf kg\n", pPlanetData->mass);
	printf("Planet Radius - %lf m\n", pPlanetData->radius);
	printf("Planet Small g - %lf m/s^2\n", pPlanetData->gravityAcceleration);
	printf("Planet Escape Velocity- %lf m/s\n", pPlanetData->escapeVelocity);
	printf("Distance to space - %lf m\n", pPlanetData->distanceToSpace);
	printf("\n\n");
}

void print_rocket_stats(const LaunchVehicleData * pLvd)
{
	if (pLvd == NULL)
	{
		return;
	}

	printf("-- ROCKET DATA -- \n");
	printf("Payload Mass - %lf kg\n", pLvd->payloadMass);
	printf("Prop Mass - %lf kg\n", pLvd->propellantMass);
	printf("Exhaust Vel - %lf m/s\n", pLvd->exhaustVelocity);
	printf("Burn rate - %lf kg/s\n", pLvd->massEjectionRate);
	printf("\n\n");
}

void print_initial_launch_calculations(const LaunchSimulationData * pLSD)
{
	if (pLSD == NULL)
	{
		return;
	}

	printf("-- INITIAL MISSION CALCULATIONS --\n");
	printf("Expected burnout time - %lf\n", pLSD->burnoutTime);
	printf("Expected burnout velocity- %lf\n", pLSD->burnoutVelocity);
	printf("\n\n");
}
