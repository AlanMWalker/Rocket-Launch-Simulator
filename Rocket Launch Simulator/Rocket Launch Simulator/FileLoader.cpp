#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "FileLoader.h"	
#include "cJSON.h"
#include "Physics.h"
#include "Macros.h"

#define MAX_CHARS_PER_JSON_FILE_STR 10000

FileManager::FileManager()
{
	memset(&m_planetData, 0, sizeof(PlanetData));
	memset(&m_launchVehicleData, 0, sizeof(LaunchVehicleData));
}

FileManager::~FileManager()
{
	memset(&m_planetData, 0, sizeof(PlanetData));
}

bool FileManager::loadPlanetData()
{
	cJSON* pCJsonPOD = nullptr;
	FILE* pPlanetFile = nullptr;
	PlanetData tempData;
	char* pJsonFileString = nullptr;

	pJsonFileString = new char[MAX_CHARS_PER_JSON_FILE_STR];//(char*)malloc(sizeof(char) * MAX_CHARS_PER_JSON_FILE_STR);

	if (pJsonFileString == nullptr)
	{// unable to allocate memory for the json string
		printf("Error: \nUnable to allocate memory for file!\n");
		return false;
	}

	const errno_t FILE_OPEN_RESULT = fopen_s(&pPlanetFile, "data/planet.json", "r");

	if (FILE_OPEN_RESULT != 0)
	{
		printf("Error: \nUnable to open json file!\n");
		goto end_fail;
	}

	fseek(pPlanetFile, 0, SEEK_END);
	//TODO: evaluate possiblility of long rollover
	const long FILE_LENGTH = ftell(pPlanetFile);

	if (FILE_LENGTH > MAX_CHARS_PER_JSON_FILE_STR)
	{
		printf("Error: \nJson file too long.\n");
		//return false;
		goto end_fail;
	}

	fseek(pPlanetFile, 0, SEEK_SET);

	fread_s(pJsonFileString, MAX_CHARS_PER_JSON_FILE_STR,
		sizeof(char), FILE_LENGTH, pPlanetFile);

	int closingBracerIdx;
	closingBracerIdx = FILE_LENGTH - 1;

	while (pJsonFileString[closingBracerIdx] != '}' && closingBracerIdx >= 0)
	{
		--closingBracerIdx;
	}

	pJsonFileString[closingBracerIdx + 1] = '\0';
	pCJsonPOD = cJSON_Parse(pJsonFileString);

	if (pCJsonPOD == nullptr)
	{
		printf("Error: \nUnable to parse json file.\n");
		goto end_fail;
	}

	cJSON* planetObj;
	planetObj = pCJsonPOD->child;

	if (planetObj == nullptr)
	{
		printf("Error: \nUnable to find planet object within planet.json!\n");
		goto end_fail;
	}

	if (planetObj->child == nullptr)
	{
		printf("Error: \nNo data found within planet object in planet.json!\n");
		goto end_fail;
	}

	if (strstr(planetObj->child->string, "mass") == nullptr)
	{
		goto end_fail_formatting;
	}

	tempData.mass = planetObj->child->valuedouble;

	if (strstr(planetObj->child->next->string, "radius") == nullptr)
	{
		goto end_fail_formatting;
	}

	tempData.radius = planetObj->child->next->valuedouble;
	cJSON* distToSpaceObj = planetObj->child->next->next;

	if (strstr(distToSpaceObj->string, "distance_to_space") == nullptr)
	{
		goto end_fail_formatting;
	}

	tempData.distanceToSpace = distToSpaceObj->valuedouble;

	m_planetData = tempData;
	goto end_success;
	
end_fail_formatting:
	printf("Error: \nIncorrect formatting of data within planet.json!\nExpecting: mass->radius->distance_to_space\n");

end_fail:
	system("pause");

	fclose(pPlanetFile);
	SAFE_FREE_ARRAY(pJsonFileString);
	cJSON_Delete(pCJsonPOD);
	return false;

end_success:
	fclose(pPlanetFile);
	SAFE_FREE_ARRAY(pJsonFileString);
	cJSON_Delete(pCJsonPOD);
	return true;
}

bool FileManager::loadLaunchVehicleData()
{
	FILE* pLaunchVehicleFile = nullptr;
	cJSON* pLaunchVehicleJSON = nullptr;
	char* pJsonString = nullptr;
	LaunchVehicleData tempData;

	pJsonString = new char[MAX_CHARS_PER_JSON_FILE_STR];//(char*)malloc(sizeof(char) * MAX_CHARS_PER_JSON_FILE_STR);

	const errno_t FILE_OPEN_RESULT = fopen_s(&pLaunchVehicleFile, "data/rocket.json", "r");

	if (FILE_OPEN_RESULT != 0)
	{
		printf("Error: \nUnable to open json file!\n");
		goto end_fail;
	}

	fseek(pLaunchVehicleFile, 0, SEEK_END);
	//TODO: evaluate possiblility of long rollover
	const long FILE_LENGTH = ftell(pLaunchVehicleFile);

	if (FILE_LENGTH > MAX_CHARS_PER_JSON_FILE_STR)
	{
		printf("Error: \nJson file too long.\n");
		//return false;
		goto end_fail;
	}

	fseek(pLaunchVehicleFile, 0, SEEK_SET);

	fread_s(pJsonString, MAX_CHARS_PER_JSON_FILE_STR,
		sizeof(char), FILE_LENGTH, pLaunchVehicleFile);

	int closingBracerIdx;
	closingBracerIdx = FILE_LENGTH - 1;

	while (pJsonString[closingBracerIdx] != '}' && closingBracerIdx >= 0)
	{
		--closingBracerIdx;
	}

	pJsonString[closingBracerIdx + 1] = '\0';
	pLaunchVehicleJSON = cJSON_Parse(pJsonString);

	if (pLaunchVehicleJSON == nullptr)
	{
		printf("Error: \nUnable to parse json file.\n");
		goto end_fail;
	}

	if (pLaunchVehicleJSON->child == nullptr)
	{
		printf("Error: \nUnable to find rocket object in rocket.json file!\n");
		goto end_fail;
	}

	if (strstr(pLaunchVehicleJSON->child->string, "rocket") == nullptr)
	{
		printf("Error: \nUnable to find rocket object in rocket.json file!\n");
		goto end_fail;
	}

	cJSON* pRocketObj = pLaunchVehicleJSON->child;

	if (pRocketObj->child == nullptr)
	{
		printf("Error: \nUnable to find data for rocket object in rocket.json file!\n");
		goto end_fail;
	}

	if (strstr(pRocketObj->child->string, "payload_mass") == nullptr)
	{
		goto end_fail_formatting;
	}

	tempData.payloadMass = pRocketObj->child->valuedouble;

	if (strstr(pRocketObj->child->next->string, "exhaust_velocity") == nullptr)
	{
		goto end_fail_formatting;
	}

	tempData.exhaustVelocity = pRocketObj->child->next->valuedouble;

	if (strstr(pRocketObj->child->next->next->string, "mass_flow_rate") == nullptr)
	{
		goto end_fail_formatting;
	}

	tempData.massEjectionRate = pRocketObj->child->next->next->valuedouble;

	m_launchVehicleData = tempData;

	goto end_success;

end_fail_formatting:
	printf("Error: \nIncorrect formatting of data within rocket.json!\nExpecting: payload_mass->exhaust_velocity->mass_flow_rate\n");

end_fail:
	system("pause");

	fclose(pLaunchVehicleFile);
	SAFE_FREE_ARRAY(pJsonString);
	cJSON_Delete(pLaunchVehicleJSON);
	return false;

end_success:
	fclose(pLaunchVehicleFile);
	SAFE_FREE_ARRAY(pJsonString);
	cJSON_Delete(pLaunchVehicleJSON);
	return true;
}
