#pragma once
#include <iostream>
#include <string>
#define logWideChar(out) { OutputDebugString(out); OutputDebugString(L"\n"); }

static void LogText_C(const char* output) {
	wchar_t buffer[256] = { 0 };
	MultiByteToWideChar(0, 0, output, strlen(output), buffer, strlen(output));
	logWideChar(buffer);
}

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