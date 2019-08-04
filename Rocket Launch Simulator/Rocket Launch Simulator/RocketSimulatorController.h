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

void setup_rock_sim();

void run_rock_sim();

void cleanup_rock_sim();
