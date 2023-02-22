#include "GameEntity.h"
#include "Drawer.h"

GameEntity::GameEntity(const Vector2f& aPosition, Sprite* sprite)
:myPosition(aPosition)
, sprite(sprite)
, myIdMarkedForDeleteFlag(false)
{
}

GameEntity::~GameEntity(void)
{
	if (sprite != NULL)
	{
		delete sprite;
		sprite = NULL;
	}
}

bool GameEntity::Intersect(GameEntity* aGameEntity)
{
	if (myPosition == aGameEntity->GetPosition())
	{
		return true;
	}
	return false;
}

void GameEntity::Draw(Drawer* aDrawer)
{
	sprite->Draw(aDrawer, (int)myPosition.myX + 220, (int)myPosition.myY + 60);
}