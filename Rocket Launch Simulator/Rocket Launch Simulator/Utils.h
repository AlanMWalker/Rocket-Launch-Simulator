#pragma once

#include <Windows.h>

bool IsKeyPressed(int key)
{
	return (GetKeyState(key) & 0xF000);
}

bool IsKeyToggled(int key)
{

	return (GetKeyState(key) & 0x000F);
}