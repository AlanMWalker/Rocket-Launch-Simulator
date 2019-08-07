#pragma once

#include <stdbool.h>

#include "Physics.h"

//returns true if loading was successful, false if otherwise
//todo UNIT TEST

class FileManager
{
public:

	FileManager();
	~FileManager();

	bool loadPlanetData();
	bool loadLaunchVehicleData();

	PlanetData& getPlanetData() noexcept { return m_planetData; }
	LaunchVehicleData& getLaunchVehicleData() noexcept { return m_launchVehicleData; }

private:

	PlanetData m_planetData;
	LaunchVehicleData m_launchVehicleData;

};
