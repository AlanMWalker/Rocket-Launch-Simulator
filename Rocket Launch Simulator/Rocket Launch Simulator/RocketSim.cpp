#include <stdlib.h>

#include "RocketSimulatorController.h"
#include "UI.h"

#include "FileLoader.h"

#include <stdio.h>
#include <string.h>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#endif 

int main(int argc, char** argv)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	RocketSimulatorController AppController; 

	AppController.setupSimulator();
	AppController.runSimulator();
	AppController.cleanupSimulator();

	system("pause");
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif 
	return 0;
}