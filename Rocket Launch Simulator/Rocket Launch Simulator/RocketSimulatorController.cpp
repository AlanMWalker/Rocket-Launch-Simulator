#include "RocketSimulatorController.h"
#include "Utils.h"
#include "UI.h"

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "FileLoader.h"

static bool bIsSimActive = true;
static bool bIsSafeToRun = false;

static SimState g_simulationMenuState;
static RocketSimmData g_simData;

#define CLEAR_SCREEN system("cls")



void RocketSimulatorController::setupSimulator()
{
	bool result = load_planet_data(&g_simData.launch_planet);
	if (!result)
	{
		return;
	}

	setup_planet_constants(&g_simData.launch_planet);

	result = load_rocket_data(&g_simData.launch_vehicle);
	if (!result)
	{
		return;
	}

	setup_launch_vehicle_constants(&g_simData.launch_vehicle, &g_simData.launch_planet);
	bIsSafeToRun = true;
}

void RocketSimulatorController::runSimulator()
{
	bool dbS = false;
	bool dbR = false;

	if (!bIsSafeToRun)
	{
		return;
	}

	bool bUpdateScreen = false;
	setSimState(Stats_State);
	clock_t tick = clock();
	clock_t tock;
	while (bIsSimActive)
	{
		tock = tick;
		tick = clock();
		if (IsKeyPressed(VK_ESCAPE))
		{
			bIsSimActive = false;
		}

		if (IsKeyPressed('S'))
		{
			if (!dbS)
			{
				setupSimulator();
				setSimState(Stats_State);
				bUpdateScreen = true;
				dbS = true;
			}

		}
		else
		{
			dbS = false;
		}

		if (IsKeyPressed('R'))
		{
			if (!dbR)
			{
				dbR = true;
				setSimState(LaunchSim_State);
			}
		}
		else
		{
			dbR = false;
		}
		runCurrentSimState();


		const int ms = (int)(((double)(tick)-(double)(tock) / CLOCKS_PER_SEC) * 1000);
		g_simData.deltaTime = (double)(ms) / 1000.0;

		const int SleepTime = (int)(SIM_STEP * 1000) - ms;
		if (SleepTime > 0)
		{
			Sleep(SleepTime);
		}
	}
}

void RocketSimulatorController::cleanupSimulator()
{
}

void RocketSimulatorController::setSimState(SimState state)
{
	CLEAR_SCREEN;
	switch (state)
	{
	default:
	case Stats_State:
	{
		print_planet_stats(&g_simData.launch_planet);
		print_rocket_stats(&g_simData.launch_vehicle);
	}
	break;

	case LaunchSim_State:
	{
		memset(&g_simData.launch_sim, 0, sizeof(LaunchSimulationData));
		printf("-- ENGAGING LAUNCH -- \n");
#ifdef LAUNCH_TIMER_ENGAGED
		Sleep(1000);
		printf("3...\n");
		Sleep(1000);
		printf("2...\n");
		Sleep(1000);
		printf("1...\n");
#endif
	}
	break;

	}

	g_simulationMenuState = state;
}

void RocketSimulatorController::runCurrentSimState()
{
	switch (g_simulationMenuState)
	{
	default:
	case Stats_State:
	{
		return;
	}
	break;

	case LaunchSim_State:
	{
		if (!g_simData.bLaunchSimComplete)
		{
			step_simulation(&g_simData);
		}
		else
		{
			printf("Velocity at time %lf s - %lf m/s\n", g_simData.launch_sim.launchTimer, g_simData.launch_sim.velocity);
			system("pause");
		}
	}
	break;

	}
}
