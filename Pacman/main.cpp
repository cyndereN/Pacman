#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "assert.h"
#include "pacman.h"
#include "drawer.h"
#include <iostream>
#include "settings.h"

int main(int argc, char **argv)
{
	/* initialize SDL */
	
	// Check if initialization is sucessfull

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		assert(0 && "Failed to initialize video!");
		exit(-1);
	}


	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)
	{
		assert(0 && "Failed to initialize image types!");
		exit(-1);
	}


	if (TTF_Init() == -1)
	{
		assert(0 && "Failed to create ttf!");
		exit(-1);
	}


	// Declaration out of loop

	float lastFrame = (float) SDL_GetTicks() * 0.001f;
	float currentFrame = 0;
	float elapsedTime = 0;
	
	// Set-ups
	SDL_Window* window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_HEIGHT, WINDOW_WIDTH, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	Drawer* drawer = Drawer::Create(window, renderer);
	Pacman* pacman = Pacman::Create(drawer);
	SDL_Event event;

	srand((unsigned)time(NULL));

	if(!window)
	{
		assert(0 && "Failed to create window!");
		exit(-1);
	}

	if (!renderer)
	{
		assert(0 && "Failed to create renderer!");
		exit(-1);
	}

	if (!drawer)
	{
		assert(0 && "Failed to create drawer!");
		exit(-1);
	}

	if (!pacman)
	{
		assert(0 && "Failed to create game!");
		exit(-1);
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	// Game loop
	while (SDL_PollEvent(&event) >= 0)
	{
		// Synchronice the engine drawing with system ticks
		currentFrame = SDL_GetTicks() * 0.001f;
		elapsedTime = currentFrame - lastFrame;
		

		// Update

		if (!pacman->Update(elapsedTime))
			break;


		// Render

		pacman->Draw();	
		lastFrame = currentFrame;
		
		SDL_RenderPresent(renderer);
		SDL_Delay(1);
	}

	// Checking and deletion
	if (drawer != NULL)
	{
		delete drawer;
		drawer = NULL;
	}

	if (pacman != NULL)
	{
		delete pacman;
		pacman = NULL;
	}

	if (renderer != NULL)	renderer = NULL;

	if (window != NULL)		window = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}

