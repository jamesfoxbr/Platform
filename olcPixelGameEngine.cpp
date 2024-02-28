#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "Player.h"
#include "camera.h"

#include <iostream>
#include <algorithm>
#include <vector>

// Global Variables and Constants
constexpr unsigned int  TILE_WIDTH = 8;
constexpr unsigned int  TILE_HEIGHT = 8;

constexpr unsigned int SCREEN_WIDTH = 256;
constexpr unsigned int SCREEN_HEIGHT = 180;


//////////////////////////////////////////////////////////////////////////
//                        PLATFORM CLASS                                //
//////////////////////////////////////////////////////////////////////////

class Platformer : public olc::PixelGameEngine
{
private:
	Player player;
	Camera camera;

	//controls FPS Variables and Functions
	float fTargetFrameTime = 1.0f / 100.0f; // Virtual FPS of 100fps
	float fAccumulatedTime = 0.0f;

	// Collisions and Physics
	float gravity = 8.7f;
	bool onGround = false;

	// Stage
	int stageRows;
	int stageColumns;

public:
	Platformer()
	{
		sAppName = "Super Jaminho";
	}

	bool OnUserCreate() override
	{
		player.x = 63.0f;
		player.y = 63.0f;

		stageRows = int(stage.size());
		stageColumns = int(stage[0].size());

		std::cout << "Rows: " << stage.size() << std::endl;
		std::cout << "Columns: " << stage[0].size() << std::endl;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		ControlFrameRate(fElapsedTime);

		DrawScreen();

		PlayerFunctions(fElapsedTime);

		CameraMove();

		return true;
	}

	void CameraMove();

	void DrawScreen()
	{
		Clear(olc::BLACK);
		MakeStage();
	}

	void MakeStage()
	{
		for (int y = 0; y < stageRows; y++)
		{
			for (int x = 0; x < stageColumns; x++)
			{
				switch (stage[y][x])
				{
				case 1:
					FillRect(x * TILE_WIDTH - camera.x, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, olc::DARK_YELLOW);
					break;
				case 2:
					FillRect(x * TILE_WIDTH - camera.x, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, olc::DARK_BLUE);
					break;
				default:
					break;
				}
			}
		}
	}

	void PlayerFunctions(float fElapsedTime)
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

	void MovePlayer(float fElapsedTime)
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

	void Dessacelerate(float fElapsedTime)
	{
		if (player.dx != 0)
		{
			player.dx *= player.dessaceleration;
		}
	}

	void DrawPlayer()
	{
		FillRect((int)player.x - camera.x, (int)player.y, TILE_WIDTH, TILE_HEIGHT);
	}

	bool ControlFrameRate(float fElapsedTime)
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

private:
	void Collision(float adjustX, float adjustY, float moveToX, float moveToY, olc::Pixel color = olc::RED)
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
		FillRect(int(player.x + adjustX - camera.x), int(player.y + adjustY), 1, 1, color);
	}

	// Create stage
	std::vector<std::vector<int>> stage =
	{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 1
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 2
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 3
		{1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 4
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1}, // 5
		{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 6
		{1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 7
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 8
		{1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1}, // 9
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 10
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 11
		{1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1}, // 12
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1}, // 13
		{1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, // 14
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 15
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 16
	};
};

