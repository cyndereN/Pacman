#include "BigDot.h"

BigDot::BigDot(Vector2f aPosition, Sprite* sprite)
: StaticGameEntity(aPosition, sprite)
{
	this->sprite->SetFrame(sprite->GetCurrentFrame());
}

BigDot::~BigDot(void)
{
}
