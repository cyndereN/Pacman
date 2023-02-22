#include "MovableGameEntity.h"

MovableGameEntity::MovableGameEntity(const Vector2f& aPosition, Sprite* sprite)
: GameEntity(aPosition, sprite)
{
	respawnPoint = aPosition;
	Respawn(aPosition);
}

MovableGameEntity::~MovableGameEntity(void)
{
}

void MovableGameEntity::Respawn(const Vector2f& aPosition)
{
	SetPosition(aPosition);
	myCurrentTileX = myNextTileX = myPosition.myX / 22;
	myCurrentTileY = myNextTileY = myPosition.myY / 22;
}

bool MovableGameEntity::IsAtDestination()
{
	if (myCurrentTileX == myNextTileX && myCurrentTileY == myNextTileY)
		return true;
	return false;
}

void MovableGameEntity::SetNextTile(int anX, int anY)
{
	myNextTileX = anX;
	myNextTileY = anY;
}

void MovableGameEntity::SetNextTile(Vector2f nextTile)
{
	myNextTileX = nextTile.myX;
	myNextTileY = nextTile.myY;
}