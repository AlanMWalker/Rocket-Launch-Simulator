#include <stdlib.h>
#include "RockSimStateController.h"
#include "UI.h"

int main(int argc, char** argv)
{
	setup_rock_sim();
	run_rock_sim(); 
	cleanup_rock_sim();
	system("pause");
	return 0;
}