#ifndef AVATAR_H
#define AVATAR_H

#include "MovableGameEntity.h"
#include "Vector2f.h"
#include "settings.h"

class Avatar : public MovableGameEntity
{
public:
	Avatar(const Vector2f& aPosition, Sprite* sprite);
	~Avatar(void);

	void Update(float aTime);

	Vector2f GetMovementDirection() { return movementDirection; };

private:

	void SetMovementFrame(Vector2f direction);

	Vector2f movementDirection;

	float timer;

};

#endif //AVATAR_H