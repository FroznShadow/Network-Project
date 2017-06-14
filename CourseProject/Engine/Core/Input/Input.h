#ifndef INPUT_H
#define INPUT_H
#include <vector>
#include <iostream>
#include <string>
#include <Windows.h>
//#include "Vector2f.h"

namespace Inputs
{
	class Input
	{
	public:
		int NUM_KEYCODES = 256;
		int NUM_MOUSEBUTTON = 5;

		void update();
		bool getKey(int keyCode);
		bool getKeyDown(int keyCode);
		bool getMouse(int mouseButton);
		bool getMouseDown(int mouseButton);
		static bool getMousePosition();
	private:
		static int currentKeys(int i);
		int downKeys(int i);
		int isKeyDown(int i);
		static int currentMouse(int i);
		int isButtonDown(int i);
		int downMouse(int i);
		int keyIsDown;
		int keyPressed = 0;
		int keyCheck;
		int keyreleased;
		int isKeyPressed;
		Input* inp;
	};
}
#endif