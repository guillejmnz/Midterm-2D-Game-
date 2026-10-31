#include "App.h"

#include <raylib.h>

void App::Run()
{
	InitWindow(WIDTH, HEIGHT, TITLE);
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		m_Spirograph.Update();

		BeginDrawing();
		ClearBackground(RAYWHITE);
		m_Spirograph.DrawOuterCircle(GRAY);
		m_Spirograph.Draw(DARKBLUE);
		m_Spirograph.DrawGear(ORANGE, DARKGRAY);
		m_Spirograph.DrawPen(6.0f, RED);	// drawn last so it sits on top of the line
		EndDrawing();
	}

	CloseWindow();
}
