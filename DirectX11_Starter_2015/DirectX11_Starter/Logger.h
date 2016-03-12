#pragma once
#include <iostream>
#include <string>

#define DEBUG_OUTPUT


//This system still needs a lot of work. I don't think I have decided how to do things yet.

#ifdef DEBUG_OUTPUT
#define logWideChar(out) { OutputDebugString(out); OutputDebugString(L"\n"); }
#define LogText_C(output) { \
	wchar_t buffer[256] = { 0 }; \
	MultiByteToWideChar(0, 0, output, strlen(output), buffer, strlen(output)); \
	logWideChar(buffer); \
}
#else
#define logWideChar(out)
#define LogText_C(output)
#endif // DEBUG_OUTPUT





/*static void LogText_C(const char* output) {
	wchar_t buffer[256] = { 0 };
	MultiByteToWideChar(0, 0, output, strlen(output), buffer, strlen(output));
	logWideChar(buffer);
}*/

static void LogText(std::string output) {
	LogText_C(output.c_str());
}

static void LogText(int output) {
	LogText(std::to_string(output));
}

static void LogText(unsigned int output) {
	LogText(std::to_string(output));
}

static void LogText(float output) {
	LogText(std::to_string(output));
}

static void LogText(DirectX::XMFLOAT3 output) {
	LogText("{" + std::to_string(output.x) + ", " + std::to_string(output.y) + ", " + std::to_string(output.z) + "}");
}