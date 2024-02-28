#include "Platformer.h"

void Platformer::CameraMove()
{
	int left  = int(player.x - ScreenWidth()  * 0.6f);
	int right = int(player.x - ScreenWidth()  * 0.4f);
	int down  = int(player.y - ScreenHeight() * 0.8f);
	int top   = int(player.y - ScreenHeight() * 0.2f);

	while (left > camera.x)
	{
		camera.x += camera.speed;
		camera.offsetX += camera.speed;
	}
	while (right < camera.x)
	{
		camera.x -= camera.speed;
		camera.offsetX -= camera.speed;
	}
	while (down > camera.y)
	{
		camera.y += camera.speed;
		camera.offsetY += camera.speed;
	}
	while (top < camera.y)
	{
		camera.y -= camera.speed;
		camera.offsetY -= camera.speed;
	}

	DrawRect(camera.x - camera.offsetX + int(ScreenWidth() * 0.4f), camera.y - camera.offsetY + int(ScreenHeight() * 0.2f), ScreenWidth() - int(ScreenWidth() * 0.8f) + TILE_WIDTH, ScreenHeight() - int(ScreenHeight() * 0.4f) + TILE_HEIGHT);
}

void Platformer::DrawScreen()
{
	Clear(olc::BLACK);
	MakeStage();
}

void Platformer::MakeStage()
{
	for (int y = 0; y < stageRows; y++)
	{
		for (int x = 0; x < stageColumns; x++)
		{
			switch (stage[y][x])
			{
			case 1:
				FillRect(x * TILE_WIDTH - camera.x, y * TILE_HEIGHT - camera.y, TILE_WIDTH, TILE_HEIGHT, olc::DARK_YELLOW);
				break;
			case 2:
				FillRect(x * TILE_WIDTH - camera.x, y * TILE_HEIGHT - camera.y, TILE_WIDTH, TILE_HEIGHT, olc::DARK_BLUE);
				break;
			default:
				break;
			}
		}
	}
}

void Platformer::PlayerFunctions(float fElapsedTime)
{
	DrawPlayer();
	MovePlayer(fElapsedTime);
	Dessacelerate(fElapsedTime);

	// Player collision points
	Collision(3, 7, 0, -1, olc::GREEN);  // Left Foot
	Collision(4, 7, 0, -1, olc::GREEN);  // Right Foot
	Collision(0, 4, 1, 0, olc::RED);    // Right Side  
	Collision(7, 4, -1, 0, olc::RED);    // Left Side
	Collision(3, 0, 0, 1, olc::BLUE);   // Head 
	Collision(4, 0, 0, 1, olc::BLUE);   // Head 
}

void Platformer::MovePlayer(float fElapsedTime)
{
	// Player running movement
	if (GetKey(olc::Key::RIGHT).bHeld)
	{
		player.dx += player.acceleration;
	}
	if (GetKey(olc::Key::LEFT).bHeld)
	{
		player.dx -= player.acceleration;
	}

	// Player Jump
	if (GetKey(olc::Key::SPACE).bPressed && onGround && player.dy < 75.0f)
	{
		player.dy -= player.jump;
		onGround = false;
	}

	// Applyer player physics
	player.dy += gravity;
	player.dx = std::clamp(player.dx, -200.0f, 200.0f);
	player.dy = std::clamp(player.dy, -200.0f, 200.0f);

	player.x += player.dx * fElapsedTime;
	player.y += player.dy * fElapsedTime;
}

void Platformer::Dessacelerate(float fElapsedTime)
{
	if (player.dx != 0)
	{
		player.dx *= player.dessaceleration;
	}
}

void Platformer::DrawPlayer()
{
	FillRect((int)player.x - camera.x, (int)player.y - camera.y, TILE_WIDTH, TILE_HEIGHT);
}

bool Platformer::ControlFrameRate(float fElapsedTime)
{
	//Control framerate
	fAccumulatedTime += fElapsedTime;
	if (fAccumulatedTime >= fTargetFrameTime)
	{
		fAccumulatedTime -= fTargetFrameTime;
		fElapsedTime = fTargetFrameTime;
		return false;
	}
	else
		return true; // Don't do anything this frame
}

void Platformer::Collision(float adjustX, float adjustY, float moveToX, float moveToY, olc::Pixel color)
{
	int playerToMapPosX = int((player.x / 8) + adjustX / 8);
	int playerToMapPosY = int((player.y / 8) + adjustY / 8);

	// This if statement is to avoid the vector go out of fluffyness (bounds)
	if (playerToMapPosX >= 0 && playerToMapPosX < stageColumns && playerToMapPosY >= 0 && playerToMapPosY < stageRows)
	{
		while (stage.at(playerToMapPosY).at(playerToMapPosX) > 0)
		{
			// Break blocks
			if (player.dy < 0 && color == olc::BLUE && stage.at(playerToMapPosY).at(playerToMapPosX) == 1)
			{
				stage.at(playerToMapPosY).at(playerToMapPosX) = 0;
			}

			// Correct player position after detec collision
			player.y += moveToY;
			player.x += moveToX;

			// Detects if the player is touching the ground
			if (moveToY < 0)
			{
				onGround = true;
			}

			if (moveToX != 0)
			{
				player.dx = 0;
				playerToMapPosX = int(player.x / 8);
			}
			if (moveToY != 0)
			{
				player.dy = 0;
				playerToMapPosY = int(player.y / 8);
			}
		}
	}

	// Draw collision points
	FillRect(int(player.x + adjustX - camera.x), int(player.y + adjustY - camera.y), 1, 1, color);
}
