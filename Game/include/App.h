#pragma once

class App
{
private:
	const int WIDTH;
	const int HEIGHT;
	const char* TITLE;
	const Color BACKGROUND_COLOR;

public:
	App(int width, int height, const char* title, Color backgroundColor) : 
		WIDTH(width), 
		HEIGHT(height), 
		TITLE(title), 
		BACKGROUND_COLOR(backgroundColor) {}
	void Run();

};