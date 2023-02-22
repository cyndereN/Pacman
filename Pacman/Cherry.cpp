#include "Cherry.h"

Cherry::Cherry(Vector2f aPosition, Sprite* sprite)
: StaticGameEntity(aPosition, sprite)
{
	this->sprite->SetFrame(sprite->GetCurrentFrame());
}


Cherry::~Cherry(void)
{
}
