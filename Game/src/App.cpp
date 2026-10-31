#include "App.h"
#include <raylib.h>
void App::Run()
{
	InitWindow(WIDTH, HEIGHT, TITLE);
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("Raylib linked", 20, 20, 20, DARKGRAY);
		EndDrawing();
	}
	CloseWindow();
}
