#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "FileLoader.h"	
#include "cJSON.h"
#include "Physics.h"

#define MAX_CHARS_PER_JSON_FILE_STR 10000

bool load_planet_data(PlanetData * pPlanetData)
{
	cJSON* pCJsonPOD = NULL;
	FILE* pPlanetFile = NULL;
	PlanetData tempData;
	char* pJsonFileString = NULL;

	if (pPlanetData == NULL)
	{
		printf("Error!: \nPlanetData data structure passed to %s is NULL!\n", __FUNCTION__);
		return false;
	}

	pJsonFileString = (char*)malloc(sizeof(char) * MAX_CHARS_PER_JSON_FILE_STR);

	if (pJsonFileString == NULL)
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

	if (pCJsonPOD == NULL)
	{
		printf("Error: \nUnable to parse json file.\n");
		goto end_fail;
	}

	cJSON* planetObj;
	planetObj = pCJsonPOD->child;

	if (planetObj == NULL)
	{
		printf("Error: \nUnable to find planet object within planet.json!\n");
		goto end_fail;
	}

	if (planetObj->child == NULL)
	{
		printf("Error: \nNo data found within planet object in planet.json!\n");
		goto end_fail;
	}

	if (strstr(planetObj->child->string, "mass") == NULL)
	{
		goto end_fail_formatting;
	}

	tempData.mass = planetObj->child->valuedouble;

	if (strstr(planetObj->child->next->string, "radius") == NULL)
	{
		goto end_fail_formatting;
	}

	tempData.radius = planetObj->child->next->valuedouble;
	cJSON* distToSpaceObj = planetObj->child->next->next;

	if (strstr(distToSpaceObj->string, "distance_to_space") == NULL)
	{
		goto end_fail_formatting;
	}

	tempData.distanceToSpace = distToSpaceObj->valuedouble;

	memcpy(pPlanetData, &tempData, sizeof(PlanetData));

	goto end_success;

end_fail_formatting:
	printf("Error: \nIncorrect formatting of data within planet.json!\nExpecting: mass->radius->distance_to_space\n");

end_fail:
	system("pause");

	fclose(pPlanetFile);
	free(pJsonFileString);
	return false;

end_success:
	return true;
}

bool load_rocket_data(LaunchVehicleData* pLVD);