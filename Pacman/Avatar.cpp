#include "Avatar.h"

Avatar::Avatar(const Vector2f& aPosition, Sprite* sprite)
: MovableGameEntity(aPosition, sprite)
{
	SetSpeed(150.f);
	sprite->SetFrame("openLeft");
}

Avatar::~Avatar(void)
{
}

void Avatar::Update(float aTime)
{
	float distanceToMove = aTime * speed;
	timer += aTime;

	Vector2f destination(myNextTileX * TILESIZE, myNextTileY * TILESIZE);
	Vector2f direction = destination - myPosition;


	// There is a tube in the middle
	if (distanceToMove > direction.Length() ||
		(myCurrentTileX == 0 && myNextTileX == 25) ||  
		(myCurrentTileX == 25 && myNextTileX == 0))
	{
		myPosition = destination;
		myPreviousTileX = myCurrentTileX;
		myPreviousTileY = myCurrentTileY;
		myCurrentTileX = myNextTileX;
		myCurrentTileY = myNextTileY;
	}
	else
	{
		direction.Normalize();
		myPosition += direction * distanceToMove;

		if (timer > 0.1f)
		{
			// Change sprite
			SetMovementFrame(direction);
			timer = 0.f;
		}
	}
}
void Avatar::SetMovementFrame(Vector2f direction)
{
	if (direction.myX == 0 && direction.myY == 0)
	{	// Avatars is not moving
		return;
	}
	else if (direction.myX == 0)
	{	
		if (direction.myY > 0) // going down
		{
			movementDirection = Vector2f(0.f, 1.0f);
			if (sprite->GetCurrentFrame() == "closedDown")
				sprite->SetFrame("openDown");
			else
				sprite->SetFrame("closedDown");
		}
		else if (direction.myY < 0) // going up
		{
			movementDirection = Vector2f(0.f, -1.0f);
			if (sprite->GetCurrentFrame() == "closedUp")
				sprite->SetFrame("openUp");
			else
				sprite->SetFrame("closedUp");
		}
	}
	else if (direction.myY == 0)
	{ 
		if (direction.myX > 0)
		{
			movementDirection = Vector2f(1.0f, 0.f);  // going right
			if (sprite->GetCurrentFrame() == "closedRight")
				sprite->SetFrame("openRight");
			else
				sprite->SetFrame("closedRight");
		}
		else if (direction.myX < 0)
		{
			movementDirection = Vector2f(-1.0f, 0.f);   // going left
			if (sprite->GetCurrentFrame() == "closedLeft")
				sprite->SetFrame("openLeft");
			else
				sprite->SetFrame("closedLeft");
		}
	}
}