#include "Platformer.h"

void Platformer::CameraMove()
{
	int left = int(player.x - ScreenWidth() * 0.7f);
	int right = int(player.x - ScreenWidth() * 0.3f);
	int width = right - left;

	while (left > camera.x)
	{
		camera.x += camera.speed;
	}
	while (right < camera.x)
	{
		camera.x -= camera.speed;
	}

	//DrawRect(int(ScreenWidth() * 0.4f) + camera.x, 0, width, 127, olc::YELLOW);
}
