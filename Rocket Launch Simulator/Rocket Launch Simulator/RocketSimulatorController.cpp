#include "RocketSimulatorController.h"
#include "Utils.h"
#include "UI.h"

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define CLEAR_SCREEN system("cls")

void RocketSimulatorController::setupSimulator()
{
	bool result = m_fileManager.loadPlanetData();
	if (!result)
	{
		return;
	}

	m_physics.setupPlanetConstants(&m_fileManager.getPlanetData());

	result = m_fileManager.loadLaunchVehicleData();
	if (!result)
	{
		return;
	}

	m_physics.setupLaunchVehicleConstants(&m_fileManager.getLaunchVehicleData(), &m_fileManager.getPlanetData());
	m_bIsSafeToRun = true;

	m_simData.launch_planet = m_fileManager.getPlanetData();
	m_simData.launch_vehicle = m_fileManager.getLaunchVehicleData();

	m_physics.initialiseSimulationPhysicsModule(&m_simData);
}

void RocketSimulatorController::runSimulator()
{
	bool dbS = false;
	bool dbR = false;

	if (!m_bIsSafeToRun)
	{
		return;
	}

	bool bUpdateScreen = false;
	setSimState(Stats_State);
	clock_t tick = clock();
	clock_t tock;
	while (m_bIsSimActive)
	{
		tock = tick;
		tick = clock();
		if (IsKeyPressed(VK_ESCAPE))
		{
			m_bIsSimActive = false;
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
		m_simData.deltaTime = static_cast<double>(ms) / 1000.0;

		const int SleepTime = static_cast<int>(SIM_STEP * 1000) - ms;
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
		print_planet_stats(&m_simData.launch_planet);
		print_rocket_stats(&m_simData.launch_vehicle);
	}
	break;

	case LaunchSim_State:
	{
		memset(&m_simData.launch_sim, 0, sizeof(LaunchSimulationData));
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

	m_simulationMenuState = state;
}

void RocketSimulatorController::runCurrentSimState()
{
	switch (m_simulationMenuState)
	{
	default:
	case Stats_State:
	{
		return;
	}
	break;

	case LaunchSim_State:
	{
		if (!m_simData.bLaunchSimComplete)
		{
			m_physics.stepSimulation(m_simData.deltaTime);
		}
		else
		{
			printf("Velocity at time %lf s - %lf m/s\n", m_simData.launch_sim.launchTimer, m_simData.launch_sim.velocity);
			system("pause");
		}
	}
	break;

	}
}
