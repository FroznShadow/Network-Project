#include "Input.h"



void Inputs::Input::update()
{
	for (int i = 0; i < NUM_KEYCODES; i++)
	{
		if (getKey(i) && !currentKeys(i))
		{
			downKeys(i);
		};
	}

	for (int i = 0; i < NUM_KEYCODES; i++)
	{
		if (getKey(i))
		{
			currentKeys(i);
		}
	}
	for (int i = 0; i < NUM_MOUSEBUTTON; i++)
	{
		if (!getMouse(i) && currentMouse(i))
		{
			downMouse(i);
		};
	}
	for (int i = 0; i < NUM_MOUSEBUTTON; i++)
	{
		if (getMouse(i))
		{
			currentMouse(i);
		}
	}
}

bool Inputs::Input::getKey(int keyCode)
{
	return(isKeyDown(keyCode) & 0x8000) != 0;
}
bool Inputs::Input::getKeyDown(int keyCode)
{
	return(downKeys(keyCode) & 0x8000) != 0;
}
bool Inputs::Input::getMouse(int mouseButton)
{
	return(isButtonDown(mouseButton) & 0x8000 != 0);
}
bool Inputs::Input::getMouseDown(int mouseButton)
{
	return(downMouse(mouseButton) & 0x8000 != 0);
}

unsigned int keyCode = 0;
unsigned int mouseButton = 0;

int Inputs::Input::downKeys(int dk)
{
	return(GetAsyncKeyState(dk));
}
int Inputs::Input::isKeyDown(int ikd)
{
	return(GetAsyncKeyState(ikd));
}
int Inputs::Input::downMouse(int dm)
{
	return(GetAsyncKeyState(dm));
}
int Inputs::Input::isButtonDown(int ibd)
{
	return(GetAsyncKeyState(ibd));
}
int Inputs::Input::currentMouse(int cm)
{
	cm = mouseButton;
	return cm;
}
int Inputs::Input::currentKeys(int ck)
{
	ck = keyCode;

	return ck;
}