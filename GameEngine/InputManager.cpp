#include "InputManager.h"

namespace GameEngine
{

	InputManager::InputManager(): _mouseCoords(0.0f)
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::update()
	{
		//loop though previous keymap
		//using for each loop
		for (auto& it : _keyMap)
		{
			//copies keymap to previouskeymap
			_previousKeyMap[it.first] = it.second;
		}
		//set to current key state

	}

	void InputManager::pressKey(unsigned int keyID)
	{
		//if key doesnt exist, creates it, then sets to true
		_keyMap[keyID] = true;
	}

	void InputManager::releaseKey(unsigned int keyID)
	{
		_keyMap[keyID] = false;
	}

	void InputManager::setMouseCoords(float x, float y)
	{
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}

	bool InputManager::isKeyDown(unsigned int keyID)
	{
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end())
		{
			return it->second;
		}
		else
		{
			return false;
		}
	}

	//returns true if key was just pressed
	bool InputManager::isKeyPressed(unsigned int keyID)
	{
		//if the key is down and was not down in the previous frame
		if (isKeyDown(keyID) == true && wasKeyDown(keyID) == false)
		{
			return true;
		}
		return false;
	}

	//returns true if key was held down last frame
	bool InputManager::wasKeyDown(unsigned int keyID)
	{
		auto it = _previousKeyMap.find(keyID);
		if (it != _previousKeyMap.end())
		{
			return it->second;
		}
		else
		{
			return false;
		}
	}
}
