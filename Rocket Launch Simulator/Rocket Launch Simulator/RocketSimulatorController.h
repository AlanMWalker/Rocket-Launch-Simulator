#pragma once

#include <stdbool.h>
#include "Physics.h"

extern bool bIsSimActive;
//#define LAUNCH_TIMER_ENGAGED

typedef enum MS
{
	Stats_State,
	LaunchSim_State
} SimState;


class RocketSimulatorController
{
public:

	void setupSimulator();

	void runSimulator();

	void cleanupSimulator();

private:

	void setSimState(SimState state);
	void runCurrentSimState();

	SimPhysics m_physics;


	bool m_bIsSimActive = true;
	bool m_bIsSafeToRun = false;

	SimState m_simulationMenuState;
	RocketSimmData m_simData;


};