#ifndef DOT_H
#define DOT_H

#include "StaticGameEntity.h"
#include "Vector2f.h"

class Dot : public StaticGameEntity
{
public:
	Dot(Vector2f aPosition, Sprite* sprite);
	~Dot(void);
};

#endif // DOT_H