#ifndef PLAYER_H
#define PLAYER_H

//////////////////////////////////////////////////////////////////////////
//                          PLAYER CLASS                                //
//////////////////////////////////////////////////////////////////////////
class Player
{
public:
	float x;
	float y;
	float dx;
	float dy;
	float acceleration = 8.0f;
	float dessaceleration = 0.9f;
	float jump = 200.0f;

public:
	Player(float x, float y)
		: x(x), y(y), dx(0.0f), dy(0.0f)
	{

	}

	Player() = default;
};

#endif
