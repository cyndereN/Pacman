#include "Sprite.h"
#include "Drawer.h"
#include "SDL_image.h"
#include "SDL_rect.h"
#include "SDL.h"

// Static method, create a sprite with the texture 
Sprite* Sprite::Create(std::map<std::string, std::string> assetPaths, Drawer* drawer, int sizeX, int sizeY)
{
	std::map<std::string, SDL_Texture*> frameCollection;
	std::map<std::string, std::string>::iterator it;

	for (it = assetPaths.begin(); it != assetPaths.end(); it++)
	{
		SDL_Texture* texture = drawer->GetTextureResource(it->second);
		frameCollection[it->first] = texture;
	}

	SDL_Rect sizeRect;
	sizeRect.x = 0;
	sizeRect.y = 0;
	sizeRect.w = sizeX;
	sizeRect.h = sizeY;

	Sprite* newSprite = new Sprite(frameCollection, sizeRect);
	return newSprite;
}

Sprite::~Sprite(void)
{
}

// Set the texture
void Sprite::SetFrame(std::string frameKey)
{
	if (textures[frameKey] != NULL)
		currentFrame = frameKey;
}

// Get rid of hard-coding frame entry
std::string Sprite::GetCurrentFrame()
{
	if (currentFrame != "")
		return currentFrame;
	else
		return textures.begin()->first;
}

// Constructor
Sprite::Sprite(std::map<std::string, SDL_Texture*> frameCollection, SDL_Rect sizeRect)
: frame(sizeRect)
{
	textures.insert(frameCollection.begin(), frameCollection.end());
}

// Call drawer to render onto screen
void Sprite::Draw(Drawer* drawer, int posX, int posY)
{
	drawer->Draw(textures[currentFrame], frame, posX, posY);
}