#include "Mouse.h"

#pragma comment(lib, "ntdll.lib")

Mouse* mouse = new Mouse();

bool kernelmode = true;

struct io_input {
	char button;
	char x;
	char y;
	char wheel;
	char unk1;
};

HWND hWnd;

struct handle_data {
	unsigned long process_id;
	HWND window_handle;
};

BOOL is_main_window(HWND handle)
{
	return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}

BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lparam) {
	auto& data = *reinterpret_cast<handle_data*>(lparam);

	unsigned long process_id = 0;
	GetWindowThreadProcessId(handle, &process_id);

	if (data.process_id != process_id || !is_main_window(handle)) {
		return TRUE;
	}
	data.window_handle = handle;
	return FALSE;
}

HWND findMain() {
	handle_data data{};

	data.process_id = GetCurrentProcessId();
	data.window_handle = nullptr;
	EnumWindows(enum_windows_callback, reinterpret_cast<LPARAM>(&data));

	return data.window_handle;
}

void Mouse::Move(int x, int y) {
	if (kernelmode) {
		IO_STATUS_BLOCK block;

		io_input io_data;

		io_data.unk1 = 0;
		io_data.button = 0;
		io_data.wheel = 0;
		io_data.x = x;
		io_data.y = y;

		NtDeviceIoControlFile(input, 0, 0, 0, &block, 0x2a2010, &io_data, sizeof(io_input), 0, 0) == 0L;
	}
	else {
		std::cout << "mouse_event" << std::endl;
		mouse_event(MOUSEEVENTF_MOVE, x, y, 0, 0);
	}
}

Mouse::Mouse() {
	ShowWindow(findMain(), SW_HIDE);

	FILE* f;
	AllocConsole();

	freopen_s(&f, "CONOUT$", "w", stdout);
	freopen_s(&f, "CONIN$", "r", stdin);

	kernelmode = true;

	wchar_t device[] = L"\\??\\ROOT#SYSTEM#0002#{1abc05c0-c378-41b9-9cef-df1aba82b015}";

	UNICODE_STRING name;
	OBJECT_ATTRIBUTES attr;

	RtlInitUnicodeString(&name, device);
	InitializeObjectAttributes(&attr, &name, 0, NULL, NULL);

	NTSTATUS status = NtCreateFile(&input, 
		GENERIC_WRITE | SYNCHRONIZE, 
		&attr, 
		&iostatus, 
		0,
		FILE_ATTRIBUTE_NORMAL, 
		0, 
		3, 
		FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT, 
		0, 
		0
	);

	if (FAILED(status)) {
		std::cout << skCrypt("[-] failed to open driver, using usermode mouse movement") << std::endl;

		kernelmode = false;
	}
	else {
		std::cout << skCrypt("[+] driver connection established") << std::endl;
	}
}

Mouse::~Mouse() {
	CloseHandle(input);
}