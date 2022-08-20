#pragma once

#include <Windows.h>
#include <winternl.h>
#include <iostream>

#include "skCrypt.h"

class Mouse
{
private:
	HANDLE input;
	IO_STATUS_BLOCK iostatus;

public:
	Mouse();
	~Mouse();

	void Move(int x, int y);
};

extern Mouse* mouse;

