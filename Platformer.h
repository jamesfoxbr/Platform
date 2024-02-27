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
public:
	Platformer()
	{
		sAppName = "Super Jaminho";
	}

	bool OnUserCreate() override
	{
		player.x = 63;
		player.y = 63;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		ControlFrameRate(fElapsedTime);

		DrawScreen();

		DrawPLayer();

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
		MovePlayer(fElapsedTime);
		Dessacelerate(fElapsedTime);
		Collision(7, 0, 1, 0);
	}

	void MovePlayer(float fElapsedTime)
	{
		if (GetKey(olc::Key::RIGHT).bHeld) player.dx += player.acceleration;
		if (GetKey(olc::Key::LEFT).bHeld)  player.dx -= player.acceleration;
		if (GetKey(olc::Key::DOWN).bHeld)  player.dy += player.acceleration;
		if (GetKey(olc::Key::UP).bHeld)    player.dy -= player.acceleration;

		ClampPLayerToScreen();

		// Max speed
		player.dx = std::clamp(player.dx, -50.0f, 50.0f);
		player.dy = std::clamp(player.dy, -50.0f, 50.0f);

		player.x += player.dx * fElapsedTime;
		player.y += player.dy * fElapsedTime;
	}

	void ClampPLayerToScreen()
	{
		player.x = std::clamp(player.x, 0.0f, 127.0f - TILE_WIDTH);
	}

	void Dessacelerate(float fElapsedTime)
	{
		if (player.dx != 0) player.dx *= player.dessaceleration;
		if (player.dy != 0) player.dy *= player.dessaceleration;

	}

	void DrawPLayer()
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
	Player player;

	//controls FPS Variables and Functions
	float fTargetFrameTime = 1.0f / 100.0f; // Virtual FPS of 100fps
	float fAccumulatedTime = 0.0f;

	// Collisions
	void Collision(float adjustX, float adjustY, float moveToX, float moveToY)
	{
		int playerToMapPosX = (player.x / 8) + adjustX / 8;
		int playerToMapPosY = (player.y / 8) + adjustY / 8;

		FillRect(player.x + adjustX, player.y + adjustY, 1, 1, olc::RED);

		if (stage[playerToMapPosY][playerToMapPosX] == 1)
		{
			player.x += 1.0f;
		}
	}

	// Crete stage
	std::vector<std::vector<int>> stage =
	{
		{1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 1
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 2
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 3
		{0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 4
		{0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 5
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 6
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 7
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 8
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 9
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 10
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 11
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 12
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 13
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 14
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 15
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 16
	};
};

#endif