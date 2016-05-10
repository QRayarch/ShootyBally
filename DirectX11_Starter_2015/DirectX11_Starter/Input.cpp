#include "Input.h"

Input Input::instance = Input();

void Input::SetMouseInfo(WPARAM btnState, int x, int y, int w, int h)
{
	//Set the mouse position
	GetInstance()->mouseState.screenX = x;
	GetInstance()->mouseState.screenY = y;

	GetInstance()->mouseState.screenWidth = w;
	GetInstance()->mouseState.screenHeight = h;

	//Handle mouse buttons;
	switch (btnState)
	{
		case 1:
			GetInstance()->mouseState.leftDown = true;
			break;
		case 16:
			GetInstance()->mouseState.middleDown = true;
			break;
		case 2:
			GetInstance()->mouseState.leftDown = true;
			break;
		default:
			GetInstance()->mouseState.leftDown = false;
			GetInstance()->mouseState.middleDown = false;
			GetInstance()->mouseState.leftDown = false;
			break;
	}
}

void Input::Update()
{
	//Handle keyboard 
	//Loop through all the keys and update their states
	for (std::unordered_map<int, KeyState>::iterator iterator = GetInstance()->keyStates.begin(); iterator != GetInstance()->keyStates.end(); iterator++) {
		iterator->second.wasDownPrevFrame = iterator->second.isDown;//Set the previous frames value
		iterator->second.isDown = GetAsyncKeyState(iterator->first) & 0x8000;//Set the current frames value
	}
}

void Input::EndFrameUpdate()
{
	//Set the last frame info for mouse buttons
	GetInstance()->mouseState.leftPrevFrameDown = GetInstance()->mouseState.leftDown;
	GetInstance()->mouseState.middlePrevFrameDown = GetInstance()->mouseState.middleDown;
	GetInstance()->mouseState.rightPrevFrameDown = GetInstance()->mouseState.rightDown;
	//Handle last frame mouse pos
	GetInstance()->mouseState.previousFrameScreenX = GetInstance()->mouseState.screenX;
	GetInstance()->mouseState.previousFrameScreenY = GetInstance()->mouseState.screenY;
}

bool Input::IsKeyDown(int key)
{
	//TODO: Check to see if a key is valid?

	std::unordered_map<int, KeyState>::iterator result = GetInstance()->keyStates.find(key);
	//We didn't find the key so we need to add the key to the system
	if (result == GetInstance()->keyStates.end()) {
		GetInstance()->keyStates.insert(std::pair<int, KeyState>(key, KeyState()));
		result = GetInstance()->keyStates.find(key);//Refind the key
	}
	return result->second.isDown;
}

bool Input::IsKeyDownThisFrame(int key)
{
	bool isKeyDown = IsKeyDown(key);
	std::unordered_map<int, KeyState>::iterator result = GetInstance()->keyStates.find(key);
	//Didn't check to see if result is valid since is key down should add it to the list if it isn't
	return isKeyDown && !result->second.wasDownPrevFrame;
}

Input::Input()
{
	GetInstance()->mouseState.screenWidth = 1;
	GetInstance()->mouseState.screenHeight = 1;
}


Input::~Input()
{
}

Input* Input::GetInstance()
{
	return &instance;
}
