#pragma once
#include "Windows.h"
#include <unordered_map>

struct MouseState
{
	bool leftDown;
	bool leftPrevFrameDown;

	bool middleDown;
	bool middlePrevFrameDown;

	bool rightDown;
	bool rightPrevFrameDown;

	//These numbers are not normalized for screen, this may be something that is done in the future
	int screenX;
	int screenY;
	int previousFrameScreenX;
	int previousFrameScreenY;

	int screenWidth;
	int screenHeight;
};

struct KeyState {
	bool isDown;
	bool wasDownPrevFrame;
};

class Input
{
public:
	//Things the main game should call
	static void SetMouseInfo(WPARAM btnState, int x, int y, int w, int h);
	//Update should be called really early in the frame for it to work best
	static void Update();
	//EndFrameUpdate MUST BE ONE OF THE LAST THINGS DONE IN THE FRAME, if you want input to work
	static void EndFrameUpdate();

	//Things to call if you want to get input
	//Mouse stuff
	static MouseState GetMouseState() { return GetInstance()->mouseState; }
	static bool GetMouseLeftBtnDown() { return GetInstance()->mouseState.leftDown; }
	static bool GetMouseLeftBtnDownThisFrame() { return GetInstance()->mouseState.leftDown && !GetInstance()->mouseState.leftPrevFrameDown; }
	static bool GetMouseMiddleBtnDown() { return GetInstance()->mouseState.middleDown; }
	static bool GetMouseMiddleBtnDownThisFrame() { return GetInstance()->mouseState.middleDown && !GetInstance()->mouseState.middlePrevFrameDown; }
	static bool GetMouseRightBtnDown() { return GetInstance()->mouseState.rightDown; }
	static bool GetMouseRightBtnDownThisFrame() { return GetInstance()->mouseState.rightDown && !GetInstance()->mouseState.rightPrevFrameDown; }
	static int GetMousePositionX() { return GetInstance()->mouseState.screenX; }
	static int GetMousePositionY() { return GetInstance()->mouseState.screenY; }
	static float GetMousePositionNormalizedX() { return GetInstance()->mouseState.screenX / GetInstance()->mouseState.screenWidth; }
	static float GetMousePositionNormalizedY() { return GetInstance()->mouseState.screenY / GetInstance()->mouseState.screenHeight; }
	static int GetMouseDeltaX() { return GetInstance()->mouseState.screenX - GetInstance()->mouseState.previousFrameScreenX; }
	static int GetMouseDeltaY() { return GetInstance()->mouseState.screenY - GetInstance()->mouseState.previousFrameScreenY; }
	//KeyboardStuff
	static bool IsKeyDown(int key);
	static bool IsKeyDownThisFrame(int key);
private:
	static Input instance;
	Input();
	~Input();
	static Input* GetInstance();

	//Stuff for mouse
	MouseState mouseState;
	//Stuff for keyboard
	std::unordered_map<int, KeyState> keyStates;
};

