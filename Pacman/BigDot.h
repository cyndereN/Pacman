#ifndef BIGDOT_H
#define BIGDOT_H

#include "StaticGameEntity.h"
#include "Vector2f.h"

class BigDot : public StaticGameEntity
{
public:
	BigDot(Vector2f aPosition, Sprite* sprite);
	~BigDot(void);
};

#endif // BIGDOT_H