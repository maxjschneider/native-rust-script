#pragma once

#include "Tables.h"
#include "Mouse.h"

#include <math.h>
#include <chrono>

class Recoil
{
public:
	inline static std::vector<vec2> angles = { {0, 0} };
	inline static double time = 0;

	inline static double sens = 0.5;
	inline static double fov = 90;

	inline static bool legit = false;

	inline static double scopeMult = 1.0;
	inline static double barrelMult = 1.0;
	inline static double animationMult = 1.0;

	static void Smooth(int index);
	static void Fix();
};

