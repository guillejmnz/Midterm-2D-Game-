#pragma once

#include "Spirograph.h"

class App
{
private:
	const int WIDTH;
	const int HEIGHT;
	const char* TITLE;

	// Owned by value: created with the App, destroyed with the App.
	// Declared after WIDTH and HEIGHT, so they are already set when this initializes.
	Spirograph m_Spirograph{
		Hypotrochoid{ 300, 105, 90.0f },	// R, gear r, outermost hole distance (< r)
		Vector2{ WIDTH / 2.0f, HEIGHT / 2.0f },
		0.01f,	// theta increment per point
		10		// drawing speed
	};

public:
	App(int width, int height, const char* title) :
		WIDTH(width),
		HEIGHT(height),
		TITLE(title){}
	void Run();

};
