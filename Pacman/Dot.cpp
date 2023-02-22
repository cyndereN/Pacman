#include "Dot.h"

Dot::Dot(Vector2f aPosition, Sprite* sprite)
: StaticGameEntity(aPosition, sprite)
{
	this->sprite->SetFrame(sprite->GetCurrentFrame());
}

Dot::~Dot(void)
{
}
