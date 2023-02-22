#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include <list>
#include <map>
#include <string>
#include "SDL_image.h"
#include "SDL_rect.h"

struct SDL_Texture;
class Drawer;

class Sprite
{
public:
	static Sprite* Create(std::map<std::string, std::string> assetPaths, Drawer* drawer, int sizeX, int sizeY);

	~Sprite(void);

	void SetFrame(std::string frameKey);

	void Draw(Drawer* drawer, int posX, int posY);

	std::string GetCurrentFrame();

private:
	Sprite(std::map<std::string, SDL_Texture*> frameCollection, SDL_Rect sizeRect);

	std::map<std::string, SDL_Texture*> textures;

	std::string currentFrame;

	SDL_Rect frame;
};

#endif // SPRITE_H