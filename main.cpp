#include "Platformer.h"

//////////////////////////////////////////////////////////////////////////
//                          MAIN FUNCTION                               //
//////////////////////////////////////////////////////////////////////////

int main()
{
	Platformer platformer;
	if (platformer.Construct(SCREEN_WIDTH, SCREEN_HEIGHT, 4, 4, false, true))
	platformer.Start();

	return 0;
}