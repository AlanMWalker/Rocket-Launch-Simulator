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

	static void setSimState(SimState state);
	static void runCurrentSimState();

};