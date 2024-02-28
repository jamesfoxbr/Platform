#ifndef _PLATFORMER_H_
#define _PLATFORMER_H_

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "Player.h"

#include <iostream>
#include <algorithm>
#include <vector>

// Global Variables and Constants
constexpr unsigned int  TILE_WIDTH   = 8;
constexpr unsigned int  TILE_HEIGHT  = 8;

constexpr unsigned int SCREEN_WIDTH  = 128;
constexpr unsigned int SCREEN_HEIGHT = 128;

constexpr unsigned int STAGE_WIDTH   = 16;
constexpr unsigned int STAGE_HEIGHT  = 16;

//////////////////////////////////////////////////////////////////////////
//                        PLATFORM CLASS                                //
//////////////////////////////////////////////////////////////////////////

class Platformer : public olc::PixelGameEngine
{
private:
	Player player;
	
	//controls FPS Variables and Functions
	float fTargetFrameTime = 1.0f / 100.0f; // Virtual FPS of 100fps
	float fAccumulatedTime = 0.0f;

	// Collisions and Physics
	float gravity = 8.7f;
	bool onGround = false;

public:
	Platformer()
	{
		sAppName = "Super Jaminho";
	}

	bool OnUserCreate() override
	{
		player.x = 63.0f;
		player.y = 63.0f;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		ControlFrameRate(fElapsedTime);

		DrawScreen();

		PlayerFunctions(fElapsedTime);

		return true;
	}

	void DrawScreen()
	{
		Clear(olc::BLACK);
		MakeStage();
	}

	void MakeStage()
	{
		for (int y = 0; y < STAGE_HEIGHT; y++)
		{
			for (int x = 0; x < STAGE_WIDTH; x++)
			{
				if (stage[y][x] == 1)
					FillRect(x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, olc::DARK_YELLOW);
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
		Collision(0, 4, 1,  0, olc::RED);    // Right Side  
		Collision(7, 4, -1, 0, olc::RED);    // Left Side
		Collision(3, 0, 0,  1, olc::BLUE);   // Head 
		Collision(4, 0, 0,  1, olc::BLUE);   // Head 
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
		FillRect((int)player.x, (int)player.y, TILE_WIDTH, TILE_HEIGHT);
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

		playerToMapPosX = std::clamp(playerToMapPosX, 0, 15);
		playerToMapPosY = std::clamp(playerToMapPosY, 0, 15);

		while (stage[playerToMapPosY][playerToMapPosX] == 1)
		{
			// Break blocks
			if (player.dy < 0 && color == olc::BLUE)
			{
				stage[playerToMapPosY][playerToMapPosX] = 0;
			}

			player.y += moveToY;
			player.x += moveToX;

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

		// Draw collision points
		FillRect(int(player.x + adjustX), int(player.y + adjustY), 1, 1, color);
	}

	// Create stage
	std::vector<std::vector<int>> stage =
	{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 1
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 2
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 3
		{1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 4
		{1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1}, // 5
		{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 6
		{1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 7
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 8
		{1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1}, // 9
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 10
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 11
		{1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1}, // 12
		{1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1}, // 13
		{1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, // 14
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 15
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 16
	};
};

#endif