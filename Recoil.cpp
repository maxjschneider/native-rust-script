#include "Recoil.h"

int excess = 0;
int pIndex = 0;

void Recoil::Smooth(int index) {
	if (index > angles.size() - 1 || time == 0) return;

	if (index < pIndex) {
		excess = 0;
	}

	auto start = std::chrono::high_resolution_clock::now();

	vec2 angle = angles[index];

	double mult = -0.03 * (sens * 3.0) * (fov / 100.0);

	vec2 delta;

	if (index == 0)
		delta = angles[index];
	else {
		delta.x = angles[index].x - angles[index - 1].x;
		delta.y = angles[index].y - angles[index - 1].y;
	}

	vec2 pixels;

	pixels.x = (delta.x / mult) * barrelMult * scopeMult;
	pixels.y = (delta.y / mult) * barrelMult * scopeMult;
	
	double animation;

	if (legit) 
		animation = time;
	else 
		animation = sqrt(pow(delta.x, 2) + pow(delta.y, 2)) / 0.02;

	int previousX = 0;
	int previousY = 0;

	int elapsed = 0;

	if (time > animation)
		time -= excess;
	else 
		animation -= excess;
	
	while (elapsed < animation) {
		double t = (double)elapsed / animation;

		vec2 lerp;

		int lerpX = (int)(pixels.x * t);
		int lerpY = (int)(pixels.y * t);

		mouse->Move(lerpX - previousX, lerpY - previousY);
		 
		previousX = lerpX;
		previousY = lerpY;

		Sleep(1);

		elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
	}

	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();

	if (elapsed < time) {
		Sleep(time - elapsed);

		elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();

		excess = elapsed - time;
	}
	else {
		elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();

		excess = elapsed - animation;
	}
}

void Recoil::Fix() {

	if (scopeMult == 1.0) {
		for (int i = 0; i < angles.size(); i++) {
			angles[i].x *= 0.83;
			angles[i].y *= 0.83;
		}
	}
}