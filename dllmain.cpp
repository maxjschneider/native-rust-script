#include <iostream>
#include <Windows.h>
#include <timeapi.h>
#include <vector>

#include "Mouse.h"
#include "Recoil.h"
#include "Auth.h"
#include "skCrypt.h"

#pragma comment (lib, "winmm.lib")

HANDLE console;
HMODULE module;

bool recoil = false;

struct weapon_data_entry {
	std::vector<vec2> angles;
	double time;
};

struct scope_data_entry {
	double multipler;
	double animation_mult;
};

struct barrel_data_entry {
	double mult;
};

int weapon_index = 0;
int scope_index = 0;
int barrel_index = 0;

std::vector<weapon_data_entry> weapons = {
	{ak, ak_time},
	{lr, lr_time},
	{mp5, mp5_time},
	{m2, m2_time},
	{custom, custom_time},
	{thompson, thompson_time}
};

std::vector<scope_data_entry> scopes = {
	{1.0, 1.0},
	{3.83721, 0.8},
	{7.65116, 0.8},
	{1.18605, 1.0},
	{0.8, 1.0},
};

std::vector<barrel_data_entry> barrels = {
	{1.0},
	{0.8}
};

void PlayWav(LPCSTR name) {
	HRSRC hRcs = FindResourceA(module, name, "WAVE");

	if (hRcs == 0) {
		std::cout << "[-] ERROR: failed to find resource" << std::endl;

		return;
	}

	HANDLE hRes = LoadResource(module, hRcs);

	if (hRes == NULL) {
		std::cout << "[-] ERROR: failed to load resource" << std::endl;

		return;
	}

	LPCSTR lpRes = (LPCSTR)LockResource(hRes);

	sndPlaySoundA(lpRes, SND_MEMORY | SND_SYNC | SND_NODEFAULT | SND_ASYNC);

	UnlockResource(hRes);
	FreeResource(hRes);
}

void printWeapon() {
	switch (weapon_index) {
	case 0: {
		PlayWav("ak");
		std::cout << skCrypt("\nassault rifle is now active\n");

		break;
	} case 1: {
		PlayWav("lr");
		std::cout << skCrypt("\nlr-300 is now active\n");

		break;
	} case 2: {
		PlayWav("mp5");
		std::cout << skCrypt("\nmp5 is now active\n");

		break;
	} case 3: {
		PlayWav("m2");
		std::cout << skCrypt("\nm249 is now active\n");

		break;
	} case 4: {
		PlayWav("thompson");
		std::cout << skCrypt("\nthompson is now active\n");
		Recoil::Fix();

		break;
	} case 5: {
		PlayWav("custom");
		std::cout << skCrypt("\ncustom smg is now active\n");
		Recoil::Fix();

		break;
	}
	}
}

void printScope() {
	switch (scope_index) {
	case 0: {
		PlayWav("none");
		std::cout << skCrypt("\nscope has been unequipped\n");

		break;
	}
	case 1: {
		PlayWav("eight");
		std::cout << skCrypt("\n8x scope is now active\n");

		break;
	} case 2: {
		PlayWav("sixteen");
		std::cout << skCrypt("\n16x scope is now active\n");

		break;
	} case 3: {
		PlayWav("holo");
		std::cout << skCrypt("\nholo sight is now active\n");

		break;
	} case 4: {
		PlayWav("simple");
		std::cout << skCrypt("\nsimple sight is now active\n");

		break;
	}
	}
}


void typeIt(const char* text) {
	int color = 1;

	for (int i = 0; i < strlen(text); i++) {
		if (color > 5) color = 1;

		SetConsoleTextAttribute(console, color);
		std::cout << text[i];

		Sleep(50);

		color++;
	}
}

bool down() {
	return GetAsyncKeyState(0x01) & 0x8000 && GetAsyncKeyState(0x02) & 0x8000;
}

void _main()
{
	MoveWindow(GetConsoleWindow(), 50, 50, 500, 300, true);

#pragma region text
	MMRESULT result = timeBeginPeriod(1);

	if (FAILED(result)) {
		std::cout << skCrypt("[-] failed to begin time period") << std::endl;
	}
	else {
		std::cout << skCrypt("[+] time period started") << std::endl;
	}

	if (!Authenticate()) {
		std::cout << skCrypt("\n[-] failed to authenticate\n");

		Sleep(500);

		exit(1);
	}

	std::cout << skCrypt("\nWelcome to ");

	Sleep(50);

	typeIt(skCrypt("Dufresne's"));
	Sleep(50);

	SetConsoleTextAttribute(console, 15);

	std::cout << skCrypt(" private script.\n") << std::endl;

	Sleep(1000);

	std::cout << skCrypt("Sensitivity: ");
	std::cin >> Recoil::sens;

	std::cout << skCrypt("\FOV: ");
	std::cin >> Recoil::fov;

	std::cout << skCrypt("\nUse LEFT and RIGHT arrow keys to cycle weapons.\nUse UP and DOWN arrow keys to cycle scopes.\nPress F2 to toggle recoil.\nPress F3 to toggle legit mode.\n\n");

	SetConsoleTextAttribute(console, 1);

	std::cout << skCrypt("Press END to quit.\n\n");

	SetConsoleTextAttribute(console, FOREGROUND_GREEN);
#pragma endregion text

	Recoil::angles = weapons[weapon_index].angles;
	Recoil::time = weapons[weapon_index].time;

	printWeapon();

	while (!GetAsyncKeyState(VK_END)) {

		if (GetAsyncKeyState(VK_F3)) {
			Recoil::legit = !Recoil::legit;

			if (Recoil::legit) {
				std::cout << skCrypt("\nlegit mode enabled\n");

				PlayWav("legiton");
			}
			else {
				std::cout << skCrypt("\nlegit mode disabled\n");

				PlayWav("legitoff");
			}

			while (GetAsyncKeyState(VK_F3)) Sleep(1);
		}

		if (GetAsyncKeyState(VK_F2)) {
			recoil = !recoil;

			if (recoil) {
				std::cout << skCrypt("\nrecoil enabled\n");

				PlayWav("enabled");
			}
			else {
				std::cout << "\nrecoil disabled\n";

				PlayWav("disabled");
			}

			while (GetAsyncKeyState(VK_F2)) Sleep(1);
		}

		if (GetAsyncKeyState(VK_LEFT)) {
			weapon_index--;

			if (weapon_index < 0) weapon_index = weapons.size() - 1;

			Recoil::angles = weapons[weapon_index].angles;
			Recoil::time = weapons[weapon_index].time;

			printWeapon();

			//Beep(300, 100);

			while (GetAsyncKeyState(VK_LEFT)) Sleep(1);
		}

		if (GetAsyncKeyState(VK_RIGHT)) {
			weapon_index++;

			if (weapon_index > weapons.size() - 1) weapon_index = 0;

			Recoil::angles = weapons[weapon_index].angles;
			Recoil::time = weapons[weapon_index].time;

			printWeapon();

			//Beep(300, 100);

			while (GetAsyncKeyState(VK_RIGHT)) Sleep(1);
		}

		if (GetAsyncKeyState(VK_UP)) {
			scope_index++;

			if (scope_index > scopes.size() - 1) scope_index = 0;

			Recoil::scopeMult = scopes[scope_index].multipler;
			Recoil::animationMult = scopes[scope_index].animation_mult;

			printScope();

			//Beep(300, 100);

			while (GetAsyncKeyState(VK_UP)) Sleep(1);
		}

		if (GetAsyncKeyState(VK_DOWN)) {
			scope_index--;

			if (scope_index < 0) scope_index = scopes.size() - 1;

			Recoil::scopeMult = scopes[scope_index].multipler;
			Recoil::animationMult = scopes[scope_index].animation_mult;

			printScope();

			//Beep(300, 100);

			while (GetAsyncKeyState(VK_DOWN)) Sleep(1);
		}

		if (down() && recoil) {
			for (int i = 0; i < Recoil::angles.size(); i++) {
				Recoil::Smooth(i);

				if (!down()) break;
			}

			while (GetAsyncKeyState(0x01)) Sleep(1);
		}

		Sleep(1);
	}

	timeEndPeriod(1);

	exit(0);
}



BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	module = hModule;

	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		console = GetStdHandle(STD_OUTPUT_HANDLE);

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)_main, 0, 0, 0);
	}

	return 1;
}