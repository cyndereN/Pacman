#ifndef MOVABLEGAMEENTITY_H
#define MOVABLEGAMEENTITY_H

#include "GameEntity.h"
#include "Vector2f.h"

class MovableGameEntity : public GameEntity
{
public:
	MovableGameEntity(const Vector2f& aPosition, Sprite* sprite);
	~MovableGameEntity(void);

	void Respawn(const Vector2f& aPosition);

	void SetNextTile(int anX, int anY);
	void SetNextTile(Vector2f next);

	int GetCurrentTileX() const { return myCurrentTileX; }
	int GetCurrentTileY() const { return myCurrentTileY; }

	int GetPreviousTileX() const { return myPreviousTileX; }
	int GetPreviousTileY() const { return myPreviousTileY; }

	void SetSpeed(float _speed) { speed = _speed; };
	float GetSpeed() { return speed; };

	bool IsAtDestination();
	Vector2f respawnPoint;


protected:
	
	float speed;

	int myCurrentTileX;
	int myCurrentTileY;

	int myNextTileX;
	int myNextTileY;

	int myPreviousTileX;
	int myPreviousTileY;
};

#endif // MOVABLEGAMEENTITY_H